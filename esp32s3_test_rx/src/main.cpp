#include <Arduino.h>
#include <RadioLib.h>

//  Seeed Studio XIAO ESP32S3 + Wio-SX1262 
const int PIN_NSS = 41;
const int PIN_DIO1 = 39;
const int PIN_RESET = 42;
const int PIN_BUSY = 40;

SX1262 radio = new Module(PIN_NSS, PIN_DIO1, PIN_RESET, PIN_BUSY);

void setup() {
  Serial.begin(115200);
  while(!Serial) { delay(10); }

  Serial.println("\nTest RX LoRa na węźle ESP32S3");
  delay(3000); 

  Serial.print("Inicjalizacja modulu Wio-SX1262... ");
  
  // Parametry muszą być identyczne jak w nadajniku nRF52 aby fale mogły się zgrać w częstotliwości 868 MHz i SF 9
  int state = radio.begin(868.0, 125.0, 9, 7); 
  
  if(state == RADIOLIB_ERR_NONE) {
    Serial.println("SUKCES");
  } else {
    Serial.println("BLAD");
    while (true); 
  }
}

void loop() {
  String str;
  // Funkcja receive domyślnie nasłuchuje 100 pakietów w przestrzeni
  int state = radio.receive(str);

  if(state == RADIOLIB_ERR_NONE) {
    // Jeżeli CRC (Suma Kontrolna) się zgadza, pakiet odczytuje się z układu radiowego
    Serial.println("------------------------------------------");
    Serial.println("OTRZYMANO PAKIET");
    Serial.print("MSG: ");
    Serial.println(str);
    
    // Siła sygnału (RSSI - im bliżej zera tym silniejszy)
    Serial.print("RSSI: ");
    Serial.print(radio.getRSSI());
    Serial.println(" dBm");
    
    // Współczynnik sygnał/szum (SNR - im wyżej tym czystszy sygnał w stosunku do zakłóceń)
    Serial.print("SNR: ");
    Serial.print(radio.getSNR());
    Serial.println(" dB");
    Serial.println("------------------------------------------");
  } 
  else if(state == RADIOLIB_ERR_CRC_MISMATCH) {
    Serial.println("Otrzymano pakiet, ale niekompletny. Blad CRC");
  }
}
