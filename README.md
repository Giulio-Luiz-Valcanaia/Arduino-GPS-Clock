# Arduino-GPS-Clock

1. Instale a IDE do arduino pelo site oficial: https://www.arduino.cc/en/software
2. Abra o arquivo .ino e instale as bibliotecas:
   
  a. https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

  b. https://github.com/johnrickman/LiquidCrystal_I2C
  
  c. https://playground.arduino.cc/Code/Time/
  
  d. https://github.com/JChristensen/Timezone
  
  e. https://github.com/mikalhart/TinyGPSPlus
  
4. Faça as modificações necessárias e compile o código;
5. Faça upload do novo código.


COMO DEFINIR OS HORÁRIOS


1. Vá até a linha 47
2. Modifique o seguinte array conforme a necessidade:
```
    //Formatação: {hora, minuto, {DOM, SEG, TER, QUA, QUI, SEX, SAB,}, tipo_do_alarme_2_ou_3, ativado_ou_desativado}
    // Matutino (SEG, TER, QUA, QUI, SEX) - Fundamental e Médio
    {7, 30, {false, true, true, true, true, true, false}, 2, true},  // 1ª aula
    {8, 15, {false, true, true, true, true, true, false}, 2, true},  // 2ª aula
    {9, 0, {false, true, true, true, true, true, false}, 2, true},   // Recreio
    {9, 15, {false, true, true, true, true, true, false}, 2, true},  // 3ª aula
    {10, 0, {false, true, true, true, true, true, false}, 2, true},  // 4ª aula
    {10, 45, {false, true, true, true, true, true, false}, 2, true}, // 5ª aula
    {11, 30, {false, true, true, true, true, true, false}, 2, true}, // Fim da aula

    // Matutino de Quinta-Feira - Fundamental
    {8, 8,  {false, false, false, false, true, false, false}, 3, true},  // 2ª aula
    {8, 46, {false, false, false, false, true, false, false}, 3, true},  // 3ª aula
    {9, 24, {false, false, false, false, true, false, false}, 3, true},  // Recreio
    {9, 39, {false, false, false, false, true, false, false}, 3, true},  // 4ª aula
    {10, 16, {false, false, false, false, true, false, false}, 3, true}, // 5ª aula
    {10, 53, {false, false, false, false, true, false, false}, 3, true}, // 6ª aula

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
    {20, 30, {false, true, true, true, true, true, false}, 2, true},  // Recreio
    {20, 40, {false, true, true, true, true, true, false}, 2, true},  // 4ª aula
    {21, 20, {false, true, true, true, true, true, false}, 2, true},  // 5ª aula
    {22, 0, {false, true, true, true, true, true, false}, 2, true}    // Fim da aula
```
