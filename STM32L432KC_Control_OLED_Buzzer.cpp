#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // First OLED (0x3C)

#define LDR_PIN A0  // Connect LDR to A0 (PA0 on STM32L432KC)
#define BUZZER_PIN D3 // Connect Buzzer to D3 (PB3 on STM32L432KC)
#define BUTTON_A_PIN D9 // Button A to increase threshold
#define BUTTON_B_PIN D10 // Button B to decrease threshold
#define COUNT_BUTTON_PIN D6 // Button to start countdown

int threshold = 1200; // Initial threshold value
bool counting = false; // Flag to track counting state
int countValue = 0;  // Countdown value
unsigned long previousMillis = 0; // Timer variable
const int countInterval = 1000; // Interval for counting (1 second)

void setup() {
    pinMode(LDR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(COUNT_BUTTON_PIN, INPUT_PULLUP);
    digitalWrite(BUZZER_PIN, LOW); // Ensure the buzzer is off initially
    Serial.begin(115200);

    // Initialize the OLED display
    if (!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("First OLED not found!");
        while (true);
    }
    display1.display();
    delay(2000);
}

void loop() {
    // If the count button is pressed, start counting
    if (digitalRead(COUNT_BUTTON_PIN) == LOW && !counting) {
        counting = true;
        countValue = 0;  // Reset count
        previousMillis = millis(); // Start the timer
    }

    // Read LDR value
    int ldrValue = analogRead(LDR_PIN);

    // Button handling for threshold adjustment
    if (digitalRead(BUTTON_A_PIN) == LOW) {
        threshold += 50;
        delay(200); // Debounce delay
    }
    if (digitalRead(BUTTON_B_PIN) == LOW) {
        threshold -= 50;
        delay(200); // Debounce delay
    }

    // Control the buzzer based on LDR value
    if (ldrValue < threshold) {
        digitalWrite(BUZZER_PIN, LOW);
    } else {
        digitalWrite(BUZZER_PIN, HIGH);
    }

    if (counting) {
        if (millis() - previousMillis >= countInterval) {
            previousMillis = millis(); // Reset timer
            countValue++;  // Increment count
        }
        if (ldrValue > threshold) {
            countValue = 30;
        }
        // Display counting + LDR value on OLED
        display1.clearDisplay();
        display1.setTextSize(2.5);
        display1.setTextColor(WHITE);
        display1.setCursor(10, 10);
        display1.print("Times: ");
        display1.print(countValue);
        display1.setTextSize(1);
        display1.setCursor(10, 40);
        display1.print("LDR: ");
        display1.print(ldrValue);
        display1.setCursor(10, 50);
        display1.print("Threshold: ");
        display1.print(threshold);
        display1.display();

        // If count reaches 30, display "Time Out" for 3 seconds
        if (countValue >= 30) {
            display1.clearDisplay();
            display1.setTextSize(2.5);
            display1.setCursor(20, 20);
            display1.print("! Game !");
            display1.setCursor(20,40);
            display1.print("! Over !");
            display1.display();
            delay(3000);
            countValue = 0; // Reset counter
            counting = false;
        }

    } else {
        // Normal LDR display
        display1.clearDisplay();
        display1.setTextSize(1.5);
        display1.setTextColor(WHITE);
        display1.setCursor(10, 30);
        if (ldrValue > threshold) {
            display1.print("Light ");
            digitalWrite(BUZZER_PIN, HIGH);
        } else {
            display1.print("Dark  ");
            digitalWrite(BUZZER_PIN, LOW);
        }
        // display1.setCursor(10, 30);
        // display1.print("LDR: ");
        display1.setTextSize(2);
        display1.print(ldrValue);
        display1.setTextSize(1);
        display1.setCursor(10, 50);
        display1.print("Threshold:");
        display1.print(threshold);
        display1.display();
    }

    Serial.print("LDR: ");
    Serial.print(ldrValue);
    Serial.print(" | Threshold: ");
    Serial.print(threshold);
    Serial.print(" | Count: ");
    Serial.println(countValue);

    delay(200); // Small delay to prevent flickering
}
