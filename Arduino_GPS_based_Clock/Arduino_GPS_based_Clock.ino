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
int timesetinterval = 5; //set microcontroller time every 5 seconds

// Função para centralizar texto na tela LCD
int centerTextPosition(const char* text, int width) {
  int len = strlen(text);  // Obtém o comprimento do texto
  int pos = (width - len) / 2;  // Calcula a posição central
  return pos > 0 ? pos : 0;  // Garante que a posição não seja negativa
}

bool triggerActivated = true;

struct Alarm {
  int hour;
  int minute;
  bool days[7]; // Dias da semana para o alarme: Dom, Seg, Ter, Qua, Qui, Sex, Sáb
  int outputPin; // Pino de saída para o alarme
  bool triggered;
};

// Definir os alarmes desejados
Alarm alarms[] = {
    // Matutino (SEG, TER, QUA, SEX) - Fundamental e Médio
    {7, 30, {false, true, true, true, false, true, false}, 2, true},  // 1ª aula
    {8, 15, {false, true, true, true, false, true, false}, 2, true},  // 2ª aula
    {9, 0, {false, true, true, true, false, true, false}, 2, true},   // Recreio
    {9, 15, {false, true, true, true, false, true, false}, 2, true},  // 3ª aula
    {10, 0, {false, true, true, true, false, true, false}, 2, true},  // 4ª aula
    {10, 45, {false, true, true, true, false, true, false}, 2, true}, // 5ª aula
    {11, 30, {false, true, true, true, false, true, false}, 2, true}, // Fim da aula

    // Matutino de Quinta-Feira - Fundamental
    {7, 30, {false, false, false, false, true, false, false}, 2, true},  // 1ª aula
    {8, 8,  {false, false, false, false, true, false, false}, 2, true},  // 2ª aula
    {8, 46, {false, false, false, false, true, false, false}, 2, true},  // 3ª aula
    {9, 24, {false, false, false, false, true, false, false}, 2, true},  // Recreio
    {9, 39, {false, false, false, false, true, false, false}, 2, true},  // 4ª aula
    {10, 16, {false, false, false, false, true, false, false}, 2, true}, // 5ª aula
    {10, 53, {false, false, false, false, true, false, false}, 2, true}, // 6ª aula
    {11, 30, {false, false, false, false, true, false, false}, 2, true}, // Fim da aula

    // Matutino de Quinta-Feira - Médio 
    {8, 15, {false, false, false, false, true, false, false}, 3, true},  // 2ª aula
    {9, 0, {false, false, false, false, true, false, false}, 3, true},   // Recreio
    {9, 15, {false, false, false, false, true, false, false}, 3, true},  // 3ª aula
    {10, 0, {false, false, false, false, true, false, false}, 3, true},  // 4ª aula
    {10, 45, {false, false, false, false, true, false, false}, 3, true}, // 5ª aula

    // Vespertino (SEG, TER, QUA, SEX) - Fundamental
    {13, 0, {false, true, true, true, false, true, false}, 2, true},  // 1ª aula
    {13, 45, {false, true, true, true, false, true, false}, 2, true}, // 2ª aula
    {14, 30, {false, true, true, true, false, true, false}, 2, true}, // 3ª aula
    {15, 15, {false, true, true, true, false, true, false}, 2, true}, // Recreio
    {15, 30, {false, true, true, true, false, true, false}, 2, true}, // 4ª aula
    {16, 15, {false, true, true, true, false, true, false}, 2, true}, // 5ª aula
    {17, 0, {false, true, true, true, false, true, false}, 2, true},  // Fim da aula

    // Vespertino de Quinta-Feira - Fundamental
    {13, 0, {false, false, false, false, true, false, false}, 2, true},   // 1ª aula
    {13, 37, {false, false, false, false, true, false, false}, 2, true},  // 2ª aula
    {14, 14, {false, false, false, false, true, false, false}, 2, true},  // 3ª aula
    {14, 51, {false, false, false, false, true, false, false}, 2, true},  // Recreio
    {15, 6, {false, false, false, false, true, false, false}, 2, true},   // 4ª aula
    {15, 44, {false, false, false, false, true, false, false}, 2, true},  // 5ª aula
    {16, 22, {false, false, false, false, true, false, false}, 2, true},  // 6ª aula
    {17, 0, {false, false, false, false, true, false, false}, 2, true},   // Fim da aula

    // Noturno (SEG, TER, QUA, QUI, SEX)
    {18, 30, {false, true, true, true, true, true, false}, 2, true},  // 1ª aula
    {19, 10, {false, true, true, true, true, true, false}, 2, true},  // 2ª aula
    {19, 50, {false, true, true, true, true, true, false}, 2, true},  // 3ª aula
    {20, 27, {false, true, true, true, true, true, false}, 2, true},  // Recreio
    {20, 42, {false, true, true, true, true, true, false}, 2, true},  // 4ª aula
    {21, 20, {false, true, true, true, true, true, false}, 2, true},  // 5ª aula
    {22, 0, {false, true, true, true, true, true, false}, 2, true}    // Fim da aula
};

const int numAlarms = sizeof(alarms) / sizeof(alarms[0]);




void setup() {
  lcd.begin(20, 4);  // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.clear();
  lcd.backlight();
  pinMode(2, OUTPUT);  // Define o pino 2 como saída
  pinMode(3, OUTPUT);  // Define o pino 2 como saída

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
  lcd.clear();
}


void loop() {
    local = brasiliaTimezone.toLocal(now());  // Atualiza a hora local
    int todayIndex = weekday(local) - 1;  // Converte a saída de weekday() para índice base-0

    if (now() - prev_set > timesetinterval && gps.time.isValid()) {
        setthetime();
        prev_set = now();
    }
    displaythetime();

    for (int i = 0; i < numAlarms; i++) {
        // Verifica se o alarme deve disparar baseado na hora, minuto, dia da semana e se está ativado
        if (hour(local) == alarms[i].hour && minute(local) == alarms[i].minute && alarms[i].days[todayIndex] && alarms[i].triggered) {
            digitalWrite(alarms[i].outputPin, HIGH);  // Aciona o pino configurado para o alarme
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("      ALARME!");
            smartDelay(4000);  // Tempo de toque do alarme em ms
            digitalWrite(alarms[i].outputPin, LOW);  // Desliga o pino
            alarms[i].triggered = false;  // Desativa o gatilho do alarme
            lcd.clear();
            updateDisplayInfo();
        }

        // Reativa o gatilho se não for a hora exata do alarme
        if (hour(local) != alarms[i].hour || minute(local) != alarms[i].minute) {
            alarms[i].triggered = true;
        }
    }
    smartDelay(1000);  // Atualiza a cada segundo
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

void displaythetime(void) {
    utc = now();  // Obtém a hora atual
    local = brasiliaTimezone.toLocal(utc);  // Converte para hora local

    // Atualiza a hora, minuto e segundo constantemente
    char buffer[21];
    sprintf(buffer, "Hora: %02d:%02d:%02d", hour(local), minute(local), second(local));
    lcd.setCursor(centerTextPosition(buffer, 20), 0);
    lcd.print(buffer);

    // Atualiza o dia da semana e a data em linhas separadas
    static int lastDay = -1;
    if (day(local) != lastDay) {
        lastDay = day(local);
        char dayOfWeekBuffer[21];
        char dateBuffer[21];
        const char* daysOfTheWeek[8] = {"Err", "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

        // Formata e exibe o dia da semana
        sprintf(dayOfWeekBuffer, "%s", daysOfTheWeek[weekday(local)]);
        lcd.setCursor(0, 1); // Usa toda a linha para limpar quaisquer caracteres antigos
        lcd.print("                    ");  // Limpa a linha antes de atualizar
        lcd.setCursor(centerTextPosition(dayOfWeekBuffer, 20), 1);
        lcd.print(dayOfWeekBuffer);

        // Formata e exibe a data
        sprintf(dateBuffer, "%02d/%02d/%04d", day(local), month(local), year(local));
        lcd.setCursor(0, 2); // Usa toda a linha para limpar quaisquer caracteres antigos
        lcd.print("                    ");  // Limpa a linha antes de atualizar
        lcd.setCursor(centerTextPosition(dateBuffer, 20), 2);
        lcd.print(dateBuffer);
    }

    // Verifica e atualiza o número de satélites apenas quando mudar
    static int lastSatCount = -1;
    int currentSatCount = gps.satellites.value();
    if (currentSatCount != lastSatCount) {
        lastSatCount = currentSatCount;
        char satelliteBuffer[21];

        //Se o número de satélites for menor que 9 adiciona um espaço depois dos dois pontos
        if(currentSatCount < 10) {
          sprintf(satelliteBuffer, "Satelites: %d", currentSatCount);
        } else {
          sprintf(satelliteBuffer, "Satelites:%d", currentSatCount);
        }

        lcd.setCursor(0, 3);  // Limpa toda a linha antes de atualizar
        lcd.print("                    ");
        lcd.setCursor(centerTextPosition(satelliteBuffer, 20), 3);
        lcd.print(satelliteBuffer);
    }
}


void updateDisplayInfo() {
    // Atualiza e exibe a hora
    char buffer[21];
    sprintf(buffer, "Hora: %02d:%02d:%02d", hour(local), minute(local), second(local));
    lcd.setCursor(centerTextPosition(buffer, 20), 0);
    lcd.print(buffer);

    // Exibe o dia da semana
    char dayOfWeekBuffer[21];
    const char* daysOfTheWeek[8] = {"Err", "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
    sprintf(dayOfWeekBuffer, "%s", daysOfTheWeek[weekday(local)]);
    lcd.setCursor(centerTextPosition(dayOfWeekBuffer, 20), 1);
    lcd.print(dayOfWeekBuffer);

    // Exibe a data
    char dateBuffer[21];
    sprintf(dateBuffer, "%02d/%02d/%04d", day(local), month(local), year(local));
    lcd.setCursor(centerTextPosition(dateBuffer, 20), 2);
    lcd.print(dateBuffer);

    // Exibe o número de satélites
    int currentSatCount = gps.satellites.value();
    char satelliteBuffer[21];
    sprintf(satelliteBuffer, "Satelites: %d", currentSatCount);
    lcd.setCursor(centerTextPosition(satelliteBuffer, 20), 3);
    lcd.print(satelliteBuffer);
}

