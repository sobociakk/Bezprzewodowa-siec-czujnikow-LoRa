#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // OLED -> 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Nie udało sie zainicjalizować ekranu OLED");
    while(1) delay(10); 
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Inicjalizacja...");
  display.display();

  //  BME280 -> 0x76 
  if(!bme.begin(0x76)) {
    Serial.println("Nie znaleziono czujnika BME280");
    display.setCursor(0, 20);
    display.print("Blad BME280");
    display.display();
    while (1) delay(10);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() / 100.0F; //  Pa -> hPa

  Serial.printf("Temperatura: %.2f *C, Wilgotność: %.2f %%, Ciśnienie: %.2f hPa\n", temp, hum, pres);

  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("LoRa");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 36);
  display.print("Wilg: ");
  display.print(hum);
  display.println(" %");

  display.setCursor(0, 52);
  display.print("Cisn: ");
  display.print(pres);
  display.println(" hPa");

  display.display();

  delay(2000);
}
