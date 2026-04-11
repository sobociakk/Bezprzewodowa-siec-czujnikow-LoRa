#include <Arduino.h>
#include <RadioLib.h>

//  Seeed Studio XIAO nRF52840 + Wio-SX1262 
const int PIN_NSS = 4;
const int PIN_DIO1 = 1;
const int PIN_RESET = 2;
const int PIN_BUSY = 3;
const int PIN_RF_SW = 5;


SX1262 radio = new Module(PIN_NSS, PIN_DIO1, PIN_RESET, PIN_BUSY);

void setup() {
  Serial.begin(115200);
  
  long startTime = millis();
  while (!Serial && millis() - startTime < 3000) { delay(10); }

  Serial.println("\nTest komunikacji radiowej na nRF52840");
  delay(4000); 

  Serial.print("Inicjalizacja modulu Wio-SX1262... ");
  
  int state = radio.begin(868.0, 125.0, 9, 7); 
  
  if(state == RADIOLIB_ERR_NONE) {
    radio.setDio2AsRfSwitch(true);
    radio.setRfSwitchPins(PIN_RF_SW, RADIOLIB_NC);

    Serial.println("SUKCES");
    Serial.println("Modul radiowy zostal pomyslnie wykryty\n");
  } else {
    Serial.print("Blad: ");
    Serial.println(state);
    while (true); 
  }
}

void loop() {
  Serial.print("Testowy pakiet... ");
  
  int state = radio.transmit("PING");
  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("Wyslano poprawnie pakiet");
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println("Bledny pakiet");
  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    Serial.println("Timeout");
  } else {
    Serial.print("Blad nadawania: ");
    Serial.println(state);
  }
  
  delay(5000); 
}
