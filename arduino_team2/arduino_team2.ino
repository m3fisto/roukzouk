#include <Keyboard.h>

// Ορίζουμε το pin όπου θα συνδεθεί το κουμπί/buzzer
const int buttonPin = 2; 

// Μεταβλητές για το state του κουμπιού και το debounce
int buttonState;             
int lastButtonState = HIGH;   

unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

void setup() {
  // Χρησιμοποιούμε INPUT_PULLUP
  // Συνδέστε το ένα άκρο του κουμπιού στο Pin 2 και το άλλο στο GND.
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Αρχικοποίηση της βιβλιοθήκης Keyboard
  Keyboard.begin();
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      // Αν το κουμπί πατήθηκε
      if (buttonState == LOW) {
        // Η Ομάδα 2 στέλνει το χαρακτήρα 'L'
        Keyboard.press('L');
      } else {
        // Όταν αφήνουμε το κουμπί, αφήνουμε και το πλήκτρο
        Keyboard.release('L');
      }
    }
  }

  lastButtonState = reading;
}
