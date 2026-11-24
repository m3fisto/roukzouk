#include <BleKeyboard.h>

// --- Pin Definitions for ESP32-C3 ---
const int buttonTeam1 = 0; 
const int buttonTeam2 = 1; 
const int buttonReset = 3; 

const int ledTeam1 = 6; 
const int ledTeam2 = 7; 

// --- Bluetooth Keyboard Setup ---
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

  Serial.println("Starting BLE Keyboard...");
  bleKeyboard.begin();
}

void loop() {
  if (bleKeyboard.isConnected()) {
    if (!wasConnected) {
      Serial.println("BLE Connected! Ready for game.");
      wasConnected = true;
    }

    // PHASE 1: System is free. Wait for a team to press.
    if (!isLocked) {
      if (digitalRead(buttonTeam1) == LOW) {
        isLocked = true;
        digitalWrite(ledTeam1, HIGH);
        bleKeyboard.write('a');
        Serial.println("Team 1 pressed -> Sent 'a'");
        delay(300); // Debounce
      } 
      else if (digitalRead(buttonTeam2) == LOW) {
        isLocked = true;
        digitalWrite(ledTeam2, HIGH);
        bleKeyboard.write('l');
        Serial.println("Team 2 pressed -> Sent 'l'");
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
        Serial.println("Reset pressed -> Sent 'Space'");
        delay(300); // Debounce
      }
    }
  } 
  else {
    if (wasConnected) {
      Serial.println("BLE Disconnected. Waiting for pairing...");
      wasConnected = false;
    }
  }
}