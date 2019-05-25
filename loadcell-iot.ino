//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//loadcell
#include "HX711.h"
#define calibration_factor 29000 // เอาค่าที่ได้จากการปรับ มาใส่ตรงนี้
#define DOUT  A3
#define CLK  A2
HX711 scale(DOUT, CLK);
double wei;

//Photo_sensor
int in2 = 8;
int a = 0;
int b;
int sensorPin = A0;    // select the input pin for sensor



//Relay
int m1 = 10;
int m2 = 11;

void setup() 
{
  // initialize the LCD
  lcd.begin();
  lcd.print("Welcome to");
        lcd.setCursor(0, 1);
        lcd.print("weight cheaking.");
  delay(3000);
  lcd.clear();
  
  lcd.print("Calibrating");
  scale.set_scale(calibration_factor); // ปรับค่า calibration factor
  scale.tare(); //รีเซตน้ำหนักเป็น 0
  lcd.clear();
  lcd.print("OK Start :");
  delay(1500);
  lcd.clear();

  pinMode(in2, INPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  
  digitalWrite(m1,1);
  digitalWrite(m2,1);
  digitalWrite(13,1);
  
  Serial.begin(9600);
}

void loop() {
  wei = scale.get_units();
  lcd.print("Weight:");
  lcd.print(wei, 2); //แสดงผลน้ำหนัก 2 ตำแหน่ง
  lcd.print(" kg");
//  delay(1000);
  if(wei >= 1.00)
    {
        digitalWrite(m1,0);
    }
    delay(600);
          Serial.print ("a ");
  a = analogRead(sensorPin);   
          Serial.println (a);
  if(a > 500)
      {
        digitalWrite(m2,0);
        delay(2000);             //ตัวแรกรับได้150มอเตอร์ตัวที่2ทำงาน
        digitalWrite(m1,1);
      delay(4500);              //ถึงจุดปลาย
        digitalWrite(m2,1);
      }
          Serial.print ("b ");
    b = digitalRead(in2);
          Serial.println (b);
/*  if(b == 1)
      {
        delay(2000);
        digitalWrite(m2,1);
      }*/
    delay(500);
  lcd.clear();
}
