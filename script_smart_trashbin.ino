#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

// Global var
const int trigger_inner = 7;
const int echo_inner = 6;
const int trigger_outer = 13;
const int echo_outer = 12;
const int servo_pin = 11;
long duration, duration_hand, inches, cm;
int distance, distance_hand;
const int MAX_OUTER_DISTANCE = 10;
const int MAX_INNER_DISTANCE = 5;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;


// Main Function
void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(trigger_inner, OUTPUT);
  pinMode(echo_inner, INPUT);
  pinMode(trigger_outer, OUTPUT);
  pinMode(echo_outer, INPUT);
  servo.attach(servo_pin);
  
  lcd.setCursor(1,0);
  lcd.print("Welcome User!");
  delay(3000);
  Serial.begin(9600);
}


void loop() {
  resetLCD();
  lcd.print("Silahkan scan!");
  delay(500);
  
  clearTrigger(trigger_inner);
  setupTrigger(trigger_inner);
  duration = pulseIn(echo_inner, HIGH);
  distance = getDistance(duration);
  
  clearTrigger(trigger_outer);
  setupTrigger(trigger_outer);
  duration_hand = pulseIn(echo_outer, HIGH);
  distance_hand = getDistance(duration_hand);
  
  Serial.print("Distance Hand : ");
  Serial.println(distance_hand);
  Serial.print("Distance : ");
  Serial.println(distance);
  
  resetLCD();
  if (isObjectExist() && !isTrashFull()) {    
    servo.write(180);
    lcd.print("Masukan disini!");
    delay(3000);
    resetLCD();
    lcd.print("Terima Kasih :)");
    delay(2000);
  }
  else if (isObjectExist() && isTrashFull()) {
    servo.write(90);
    lcd.print("Sampah penuh!");
    delay(3000);
  }
  else if (!isObjectExist()) {
    servo.write(90);
    resetLCD();
  }
}


/* DIY Function */
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void resetLCD() {
  lcd.clear();
  lcd.setCursor(0,0);
}

void clearTrigger(int trigger) {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
}

void setupTrigger(int trigger) {
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
}

int getDistance(long duration) {
  return duration * 0.034 / 2;
}

bool isTrashFull() {
  if (distance < MAX_INNER_DISTANCE) return true;
  else return false;
}

bool isObjectExist() {
  if (distance_hand <= MAX_OUTER_DISTANCE) return true;
  else return false;
} 