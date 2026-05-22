# 🕐 Arduino GPS-Based School Bell Clock

> Relógio de precisão sincronizado por GPS com sistema automatizado de campainha e sirene para ambientes escolares — desenvolvido para a **E.E.B. "Frei Lucínio Korte"**.

---

## 📋 Sobre o Projeto

Este projeto implementa um relógio digital de alta precisão em um **Arduino Nano**, com sincronização de tempo via sinal GPS e exibição em display LCD 20×4. O sistema gerencia automaticamente os toques de campainha e sirene nos horários de aula dos períodos **matutino**, **vespertino** e **noturno**, com suporte a grades horárias diferenciadas por dia da semana.

### Funcionalidades principais

- Sincronização automática de data e hora via GPS (sem necessidade de ajuste manual)
- Exibição de hora, data, dia da semana e número de satélites conectados no LCD
- Disparo automático de campainha (pino 2) ou sirene (pino 3) conforme o horário configurado
- Suporte a múltiplos perfis de horários por dia da semana
- Fuso horário configurável (padrão: BRT, UTC-3)
- Reconexão automática ao GPS a cada 5 segundos para manter a precisão

---

## 🛠️ Hardware Necessário

| Componente | Especificação |
|---|---|
| Microcontrolador | Arduino Nano |
| Display | LCD I2C 20×4 (endereço `0x27`) |
| Módulo GPS | Qualquer módulo NMEA (ex: NEO-6M) |
| Saída 1 | Campainha — pino digital **2** |
| Saída 2 | Sirene — pino digital **3** |

### Conexões do GPS

| GPS | Arduino Nano |
|---|---|
| TX | Pino 8 (RX via AltSoftSerial) |
| RX | Pino 9 (TX via AltSoftSerial) |
| VCC | 3.3V ou 5V |
| GND | GND |

> ⚠️ O pino PWM 10 **não está disponível** quando se utiliza a biblioteca AltSoftSerial.

---

## 📦 Dependências

Instale as seguintes bibliotecas na Arduino IDE antes de compilar:

| Biblioteca | Finalidade | Link |
|---|---|---|
| **AltSoftSerial** | Comunicação serial com o GPS | [pjrc.com](https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html) |
| **LiquidCrystal_I2C** | Controle do display LCD | [GitHub](https://github.com/johnrickman/LiquidCrystal_I2C) |
| **Time** | Gerenciamento de tempo no microcontrolador | [Arduino Playground](https://playground.arduino.cc/Code/Time/) |
| **Timezone** | Conversão de fuso horário | [GitHub](https://github.com/JChristensen/Timezone) |
| **TinyGPS++** | Parsing dos dados NMEA do GPS | [GitHub](https://github.com/mikalhart/TinyGPSPlus) |

---

## 🚀 Instalação e Upload

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software)
2. Instale todas as bibliotecas listadas acima (via Gerenciador de Bibliotecas ou manualmente)
3. Abra o arquivo `Arduino_GPS_based_Clock.ino`
4. Realize as configurações necessárias (fuso horário, alarmes — veja abaixo)
5. Selecione a placa **Arduino Nano** e a porta serial correta
6. Compile e faça o upload

---

## ⚙️ Configuração

### Fuso Horário

O fuso horário está definido nas linhas 22–23 do sketch. O padrão é **BRT (UTC-3)**. Para outras regiões, consulte a documentação da biblioteca [Timezone](https://github.com/JChristensen/Timezone) e ajuste as regras conforme necessário.

```cpp
TimeChangeRule BRT = {"BRT", Last, Sun, Mar, 2, -180}; // UTC-3 horas
Timezone brasiliaTimezone(BRT, BRT);
```

---

### Configuração dos Alarmes

Os alarmes são definidos a partir da **linha 47** do sketch, no array `alarms[]`.

**Formato de cada entrada:**

```cpp
{hora, minuto, {DOM, SEG, TER, QUA, QUI, SEX, SAB}, pino_de_saida, ativado}
```

| Campo | Tipo | Descrição |
|---|---|---|
| `hora` | `int` | Hora do disparo (formato 24h) |
| `minuto` | `int` | Minuto do disparo |
| `{DOM...SAB}` | `bool[7]` | Dias da semana em que o alarme está ativo |
| `pino_de_saida` | `int` | `2` = campainha, `3` = sirene |
| `ativado` | `bool` | `true` para ativar o alarme |

**Exemplo:**

```cpp
// Toca a campainha às 07:30, de segunda a sexta
{7, 30, {false, true, true, true, true, true, false}, 2, true}
```

---

### Grade Horária Atual

O sistema está pré-configurado com os seguintes períodos:

| Período | Dias | Turno |
|---|---|---|
| Matutino – Fundamental e Médio | Seg, Ter, Qua, Sex | 07:30 – 11:30 |
| Matutino – Quinta-Feira (Fundamental) | Qui | 07:30 – 11:30 |
| Matutino – Quinta-Feira (Médio) | Qui | 08:15 – 10:45 |
| Vespertino – Fundamental | Seg, Ter, Qua, Sex | 13:00 – 17:00 |
| Vespertino – Quinta-Feira (Fundamental) | Qui | 13:00 – 17:00 |
| Noturno | Seg a Sex | 18:30 – 22:00 |

---

## 📺 Display LCD

O display 20×4 exibe as seguintes informações em tempo real:

```
      Hora: 07:29:45      <- Linha 1: Hora atual
         Segunda          <- Linha 2: Dia da semana
       23/04/2024         <- Linha 3: Data
      Satelites: 8        <- Linha 4: Satélites GPS
```

Ao disparar um alarme, o display exibe brevemente `ALARME!` durante o tempo de toque (4 segundos), retornando em seguida à exibição normal.

---

## 📁 Estrutura do Projeto

```
Arduino_GPS_based_Clock/
├── Arduino_GPS_based_Clock.ino   # Código principal
└── README.md                     # Documentação
```

---

## 👤 Autor

**Giulio Luiz Valcanaia** — Abril de 2024  
E.E.B. "Frei Lucínio Korte"
