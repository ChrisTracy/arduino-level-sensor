#include <Wire.h>
#include <LCD_I2C.h>
#include <EEPROM.h>

const int buttonPin = 4;
const int triggerPin = 2;
const int echoPin = 3;

LCD_I2C lcd(0x27, 16, 2);

int storedDistance = 0; // Initialize stored distance

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin();
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  
  // Load the stored distance from EEPROM
  storedDistance = EEPROM.read(0);
  displayDistance(storedDistance);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    // Button is pressed, measure distance and update stored distance
    storedDistance = measureDistance();
    
    // Store the updated distance in EEPROM
    EEPROM.write(0, storedDistance);
    
    // Display the updated distance
    displayDistance(storedDistance);
    delay(500); // Add a delay to avoid multiple rapid button presses
  }

  // Measure distance continuously and update the display
  int currentDistance = measureDistance();
  displayDistance(currentDistance);
}

int measureDistance() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert to centimeters

  return distance;
}

void displayDistance(int distance) {
  int percentage = map(distance, 0, storedDistance, 100, 0);
  percentage = constrain(percentage, 0, 100); // Ensure percentage is within bounds
  
  lcd.clear(); // Clear the entire LCD
  lcd.setCursor(0, 0);
  lcd.print("Level: " + String(percentage) + "%");
  lcd.setCursor(0, 1);
  lcd.print("Distance: " + String(distance));
}
