#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // Change the address (0x27) to your LCD address

const int loopPin = 7;          // Digital pin for the loop wire
const int buzzerPin = 8;        // Digital pin for the buzzer
const int startButtonPin = 6;   // Digital pin for the start button
const int overButtonPin = 9;
const unsigned long gameTime = 12000;  // 120 seconds (2 minutes) in milliseconds
const int rledPin = 2;
const int gledPin = 3;

unsigned long gameStartTime;
unsigned long lastTime;
bool gameRunning = false;
unsigned long elapsedTime = 0;
int loopCount = 0;

void setup() {
  pinMode(loopPin, INPUT_PULLUP);       // Set loopPin as input with pull-up resistor
  pinMode(buzzerPin, OUTPUT);
  pinMode(startButtonPin, INPUT_PULLUP); // Set startButtonPin as input with pull-up resistor
  pinMode(rledPin, OUTPUT);
  pinMode(gledPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Wire Loop Game");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (!gameRunning) {
    lcd.setCursor(0, 0);
    
    lcd.print("Raise the wand");
    lcd.setCursor(0, 1);
    lcd.print("to start!");

    if (digitalRead(startButtonPin) == HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game started!");
      digitalWrite(gledPin, HIGH);
      delay(1000);
      digitalWrite(gledPin, LOW);
      lcd.clear();
      startGame(); // Start the game when the button is pressed
    }
  } else {
    unsigned long currentTime = millis();
    elapsedTime = currentTime - gameStartTime;
    
    if (elapsedTime >= gameTime) {
      endGame();
    }

    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print((gameTime - elapsedTime) / 1000);  // Display remaining time in seconds
    lcd.print("s ");
    
    lcd.setCursor(0, 1);
    lcd.print("Wire the loop! ");
    lcd.print("Count: ");
    lcd.print(loopCount);

    if (digitalRead(loopPin) == LOW) {
      loopCount++;
      lcd.clear();
      digitalWrite(rledPin, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Good job!");
      lcd.setCursor(0, 1);
      lcd.print("Count: ");
      lcd.print(loopCount);
      delay(1000);
      digitalWrite(rledPin, LOW);
      lcd.clear();
    }
  }
}

void startGame() {
  gameStartTime = millis();  // Record the start time
  gameRunning = true;
  loopCount = 0;
}

void endGame() {
  gameRunning = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game over!");
  lcd.setCursor(0, 1);
  lcd.print("Count: ");
  lcd.print(loopCount);
  digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
  delay(1000);
  digitalWrite(buzzerPin, LOW);   // Turn off the buzzer
  lcd.clear();
  delay(2000);
  lcd.print("Return the stick to");
  lcd.setCursor(0, 1);
  lcd.print("the start!");

  // Wait until the start button is pressed to clear the display
  while (digitalRead(startButtonPin) == HIGH) {
    delay(100);
    
  }
  lcd.clear();
}
