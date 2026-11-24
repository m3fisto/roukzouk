#include <BleKeyboard.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin Definitions for ESP32-C3 ---
const int buttonTeam1 = 0; 
const int buttonTeam2 = 1; 
const int buttonReset = 3; 

const int ledTeam1 = 6; 
const int ledTeam2 = 7; 

// --- OLED Display Setup ---
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Bluetooth Keyboard Setup ---
// Name: "Christmas Buzzer", Manufacturer: "Santa", Battery Level: 100%
BleKeyboard bleKeyboard("Christmas Buzzer", "Santa", 100);

bool isLocked = false;
bool wasConnected = false;

void setup() {
  Serial.begin(115200);

  // Initialize Pins
  pinMode(buttonTeam1, INPUT_PULLUP);
  pinMode(buttonTeam2, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);
  pinMode(ledTeam1, OUTPUT);
  pinMode(ledTeam2, OUTPUT);

  // Initialize OLED
  // Address 0x3C is standard for 128x64 OLEDs
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  updateOLED("Starting BLE...", 2);
  
  // Start Bluetooth Keyboard
  bleKeyboard.begin();
}

void loop() {
  // Check Bluetooth connection status
  if (bleKeyboard.isConnected()) {
    
    // Update screen once when newly connected
    if (!wasConnected) {
      updateOLED("BLE Connected!\nReady for game.", 1.5);
      wasConnected = true;
    }

    // PHASE 1: System is free. Wait for a team to press.
    if (!isLocked) {
      if (digitalRead(buttonTeam1) == LOW) {
        isLocked = true;
        digitalWrite(ledTeam1, HIGH);
        bleKeyboard.write('a');
        updateOLED("TEAM 1 !", 3);
        delay(300); // Debounce
      } 
      else if (digitalRead(buttonTeam2) == LOW) {
        isLocked = true;
        digitalWrite(ledTeam2, HIGH);
        bleKeyboard.write('l');
        updateOLED("TEAM 2 !", 3);
        delay(300); // Debounce
      }
    } 
    
    // PHASE 2: System is locked. Wait for the host to Reset.
    else {
      if (digitalRead(buttonReset) == LOW) {
        isLocked = false;
        digitalWrite(ledTeam1, LOW);
        digitalWrite(ledTeam2, LOW);
        bleKeyboard.write(' '); // Send Space to reset the HTML game
        updateOLED("READY", 3);
        delay(300); // Debounce
      }
    }
  } 
  else {
    // If Bluetooth disconnects
    if (wasConnected) {
      updateOLED("Waiting for\nBLE pairing...", 1.5);
      wasConnected = false;
    }
  }
}

// Helper function to easily update the OLED screen
void updateOLED(String text, float textSize) {
  display.clearDisplay();
  display.setTextSize(textSize);
  
  // Simple centering logic (approximate)
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  int y = (SCREEN_HEIGHT - h) / 2;
  
  display.setCursor(x, y);
  display.print(text);
  display.display();
}