/*
   Written by Giulio Luiz Valcanaia, April 2024
   Arduino Nano
   E.E.B. "Frei Lucínio Korte"
*/

#include <Timezone.h>
#include "TinyGPS++.h"
#include "AltSoftSerial.h"
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);  // Address 0x27, 20 columns, 4 lines

//define serial connection for GPS
AltSoftSerial GPS_serial;  // RX = 8 (to TX op GPS) , TX = 9 (to RX of GPS) PWM 10 is not available

//define GPS variable
TinyGPSPlus gps;

// Change these two rules corresponding to your timezone, see https://github.com/JChristensen/Timezone
TimeChangeRule BRT = {"BRT", Last, Sun, Mar, 2, -180}; // UTC-3 hours
Timezone brasiliaTimezone(BRT, BRT);

// time variables
time_t local, utc, prev_set;
int timesetinterval = 30; //set microcontroller time every 30 seconds

void setup() {
  lcd.begin(20, 4);  // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print(" Relogio esperando");
  lcd.setCursor(0, 1); //Start at character 0 on line 0
  lcd.print("     sinal GPS");
  lcd.setCursor(0, 3);
  lcd.print("  By Giulio L. V.");
  GPS_serial.begin(9600); //This opens up communications to the GPS
  smartDelay(1000);
  while (!gps.time.isValid()) // wait a valid GPS UTC time (valid is defined in the TinyGPS++ library)
  {
    smartDelay(1000);
  }
  setthetime();
  prev_set = now();
}

void loop()
{
  if (now() - prev_set > timesetinterval && gps.time.isValid())  // set the microcontroller time every interval, only if there is a valid GPS time
  {
    setthetime();
    prev_set = now();
    //lcd.clear();
    //lcd.setCursor(0, 0); //Start at character 0 on line 0
    //lcd.print(" Horario ajustado");
    //smartDelay(1000);
  }
  displaythetime();
  smartDelay(1000);     // update the time every half second
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    // If data has come in from the GPS module
    while (GPS_serial.available())
      gps.encode(GPS_serial.read()); // Send it to the encode function
    // tinyGPS.encode(char) continues to "load" the tinGPS object with new
    // data coming in from the GPS module. As full NMEA strings begin to come in
    // the tinyGPS library will be able to start parsing them for pertinent info
  } while (millis() - start < ms);
}

void setthetime(void)
{
  int Year = gps.date.year();
  byte Month = gps.date.month();
  byte Day = gps.date.day();
  byte Hour = gps.time.hour();
  byte Minute = gps.time.minute();
  byte Second = gps.time.second();
  // Set Time from GPS data string
  setTime(Hour, Minute, Second, Day, Month, Year);  // set the time of the microcontroller to the UTC time from the GPS
}
void displaythetime(void)
{
  utc = now();  // read the time in the correct format to change via the TimeChangeRules
  local = brasiliaTimezone.toLocal(utc);
  lcd.clear();

//display UTC time
/*  lcd.setCursor(4, 2); //Start at character 0 on line 1
  lcd.print("UTC:");
  lcd.setCursor(8, 2);
  // display UTC time from the GPS
  if (hour(utc) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(hour(utc));
  lcd.print(":");
  if (minute(utc) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(minute(utc));
  lcd.print(":");
  if (second(utc) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(second(utc));
  */

  // display the local time
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("Hora:");
  lcd.setCursor(6, 0);

  if (hour(local) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(hour(local));
  lcd.print(":");
  if (minute(local) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(minute(local));
  lcd.print(":");
  if (second(local) < 10) // add a zero if minute is under 10
    lcd.print("0");
  lcd.print(second(local));


  //Display satelites
  lcd.setCursor(0, 3); //Start at character 0 on line 3
  lcd.print("Satelites:");
  lcd.setCursor(10, 3);
  lcd.print(gps.satellites.value());  // display the number of satellites


  //Display day of the week
  lcd.setCursor(0, 1); //Start at character 0 on line 1
  const char* daysOfTheWeek[8] = {"Err", "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
  lcd.print(daysOfTheWeek[weekday(local)]);

  //Display today's date
  lcd.setCursor(0, 2); 
  if (day(local) < 10) lcd.print("0");  // Add leading zero if the day is less than 10
  lcd.print(day(local));
  lcd.print("/");
  if (month(local) < 10) lcd.print("0");  // Add leading zero if the month is less than 10
  lcd.print(month(local));
  lcd.print("/");
  lcd.print(year(local));  // Year is displayed as is
}