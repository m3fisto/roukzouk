#include <Keyboard.h>

// Ορισμός των Pins για τα Κουμπιά (Είσοδοι)
const int buttonTeam1 = 2; 
const int buttonTeam2 = 3; 
const int buttonReset = 4; // Το κουμπί του παρουσιαστή για επαναφορά

// Ορισμός των Pins για τα LEDs (Έξοδοι)
const int ledTeam1 = 5; 
const int ledTeam2 = 6; 

// Μεταβλητή που κρατάει το σύστημα "κλειδωμένο" όταν κάποιος πατήσει πρώτος
bool isLocked = false;

void setup() {
  // Ενεργοποιούμε τις εσωτερικές αντιστάσεις pull-up. 
  // Το ρεύμα είναι HIGH από προεπιλογή και πέφτει στο LOW όταν πατηθεί το κουμπί.
  pinMode(buttonTeam1, INPUT_PULLUP);
  pinMode(buttonTeam2, INPUT_PULLUP);
  pinMode(buttonReset, INPUT_PULLUP);
  
  // Ορίζουμε τα LEDs ως εξόδους
  pinMode(ledTeam1, OUTPUT);
  pinMode(ledTeam2, OUTPUT);
  
  // Αρχικοποίηση λειτουργίας Πληκτρολογίου
  Keyboard.begin();
}

void loop() {
  // ΦΑΣΗ 1: Το σύστημα είναι ελεύθερο. Περιμένουμε το πρώτο πάτημα.
  if (!isLocked) {
    if (digitalRead(buttonTeam1) == LOW) {
      isLocked = true;              // Κλειδώνει το σύστημα (αγνοεί την άλλη ομάδα)
      digitalWrite(ledTeam1, HIGH); // Ανάβει το LED της Ομάδας 1
      Keyboard.write('a');          // Στέλνει το γράμμα 'a' στο παιχνίδι
      delay(200);                   // Μικρή καθυστέρηση για αποφυγή διπλού πατήματος (debounce)
    } 
    else if (digitalRead(buttonTeam2) == LOW) {
      isLocked = true;              // Κλειδώνει το σύστημα
      digitalWrite(ledTeam2, HIGH); // Ανάβει το LED της Ομάδας 2
      Keyboard.write('l');          // Στέλνει το γράμμα 'l' στο παιχνίδι
      delay(200);
    }
  } 
  
  // ΦΑΣΗ 2: Κάποιος πάτησε. Περιμένουμε ΜΟΝΟ τον παρουσιαστή να κάνει Reset.
  else {
    if (digitalRead(buttonReset) == LOW) {
      isLocked = false;             // Ξεκλειδώνει το σύστημα για τον επόμενο γύρο
      digitalWrite(ledTeam1, LOW);  // Σβήνει το LED της Ομάδας 1
      digitalWrite(ledTeam2, LOW);  // Σβήνει το LED της Ομάδας 2
      
      // Στέλνει το πλήκτρο Space (Κενό) στο PC για να καθαρίσει 
      // τα χρώματα και από την οθόνη του προτζέκτορα!
      Keyboard.write(' ');          
      delay(200);
    }
  }
}