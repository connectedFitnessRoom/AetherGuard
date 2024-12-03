#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) SCL on D1 & SDA on D2
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("Échec de l'initialisation de l'écran OLED!"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void displayDataOnScreen(float temperature, float humidity, float ppm) {
  display.clearDisplay();
  
  // Temp
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(4, 14);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  // Hum
  display.setCursor(4, 24);
  display.print("Hum: ");
  display.print(humidity);
  display.println(" %");

  // PPM
  display.setCursor(4, 34);
  display.println("Qualite de l'air:");
  display.print(" ");
  display.print(ppm);
  display.println(" PPM");
  if (ppm <CAUTION_THRESHOLD) {
      display.println(" Sain");
    } else if (ppm <DANGEROUS_THRESHOLD) {
      display.println(" Mauvaise qualite");
    } else{
      display.println(" Dangereux");
    }

  display.display();
}