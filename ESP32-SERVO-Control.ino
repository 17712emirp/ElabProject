#include <ESP32Servo.h>
#include <Arduino.h>

Servo myServo1;  // First servo (Servo 1)
Servo myServo2;  // Second servo (Servo 2)

const int servoPin1 = 18;  // Servo 1 connected to GPIO 18
const int servoPin2 = 19;  // Servo 2 connected to GPIO 19
const int Button_A = 25;   // Button_A: Counterclockwise for Servo 1
const int Button_B = 26;   // Button_B: Clockwise for Servo 1
const int Button_C = 27;   // Button_C: Counterclockwise for Servo 2
const int Button_D = 32;   // Button_D: Clockwise for Servo 2

int currentAngle1 = 90;  // Track current position of Servo 1
int currentAngle2 = 90;  // Track current position of Servo 2

void setup() {
  myServo1.attach(servoPin1);
  myServo2.attach(servoPin2);
  
  pinMode(Button_A, INPUT_PULLUP);  // Button_A connected to GPIO 25
  pinMode(Button_B, INPUT_PULLUP);  // Button_B connected to GPIO 26
  pinMode(Button_C, INPUT_PULLUP);  // Button_C connected to GPIO 27
  pinMode(Button_D, INPUT_PULLUP);  // Button_D connected to GPIO 32
  
  myServo1.write(currentAngle1);  // Start at middle position for Servo 1
  myServo2.write(currentAngle2);  // Start at middle position for Servo 2
}

void loop() {
  bool buttonAState = digitalRead(Button_A) == LOW;  // Button pressed = LOW
  bool buttonBState = digitalRead(Button_B) == LOW;  // Button pressed = LOW
  bool buttonCState = digitalRead(Button_C) == LOW;  // Button pressed = LOW
  bool buttonDState = digitalRead(Button_D) == LOW;  // Button pressed = LOW
  
  // Control for Servo 1
  if (buttonAState && buttonBState) {
    // If both buttons for Servo 1 are pressed, hold position
    return;
  } 
  else if (buttonAState && currentAngle1 < 180) {  
    // Move counterclockwise if not at 180°
    currentAngle1 += 1;
    if (currentAngle1 > 180) currentAngle1 = 180;  // Prevent going above 180°
    myServo1.write(currentAngle1);
    delay(100);
  } 
  else if (buttonBState && currentAngle1 > 0) {  
    // Move clockwise if not at 0°
    currentAngle1 -= 1;
    if (currentAngle1 < 0) currentAngle1 = 0;  // Prevent going below 0°
    myServo1.write(currentAngle1);
    delay(100);
  }

  // Control for Servo 2
  if (buttonCState && buttonDState) {
    // If both buttons for Servo 2 are pressed, hold position
    return;
  } 
  else if (buttonCState && currentAngle2 < 180) {  
    // Move counterclockwise for Servo 2 if not at 180°
    currentAngle2 += 1;
    if (currentAngle2 > 180) currentAngle2 = 180;  // Prevent going above 180°
    myServo2.write(currentAngle2);
    delay(100);
  } 
  else if (buttonDState && currentAngle2 > 0) {  
    // Move clockwise for Servo 2 if not at 0°
    currentAngle2 -= 1;
    if (currentAngle2 < 0) currentAngle2 = 0;  // Prevent going below 0°
    myServo2.write(currentAngle2);
    delay(100);
  }
}
