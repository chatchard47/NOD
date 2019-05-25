

#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h> // นำเข้าไรบลารี่ ESP8266WiFi.h
#include <time.h>

//loadcell
#include "HX711.h"
#define calibration_factor 45000 // เอาค่าที่ได้จากการปรับ มาใส่ตรงนี้
#define DOUT  3
#define CLK  2
HX711 scale(DOUT, CLK);
double wei;


#include  <FirebaseArduino.h> // นำเข้าไรบลารี่ FirebaseArduino.h

#define FIREBASE_HOST "https://loadcell-iot.firebaseio.com/"  // Host ของฐานข้อมมูล Firebase
#define FIREBASE_AUTH "D4K6OejWgQzUHE50Ig5Waz5Mn3tpbDDiYOquetFe"  // รหัสข้อมูลลับของ Project บน Firebase

#define WIFI_SSID "KMITL-WIFI"  // ชื่อ WiFi
#define WIFI_PASSWORD ""  // รหัส WiFi

//Photo_sensor
int in2 = 8;
int a = 0;
int b;
int sensorPin = A0;    // select the input pin for sensor

//Relay
int m1 = 10;
int m2 = 11;

void setup() {
  
  Serial.begin(115200);

  // เชื่อมต่อ WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    
  }
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
  
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void loop() 
{
     wei = scale.get_units();
  lcd.print("Weight:");
  lcd.print(wei, 2); //แสดงผลน้ำหนัก 2 ตำแหน่ง
  lcd.print(" kg");
    //delay(1000);
  if(wei >= 1.00)
    {
        digitalWrite(m1,0);
    }
    delay(600);
          Serial.print ("a ");
  a = analogRead(sensorPin);   
          Serial.println (a);
  if(a > 150)
      {
        digitalWrite(m2,0);
        delay(2000);
        digitalWrite(m1,1);
      delay(4500);
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
  
 
  // อัพเดทข้อมูล weigth ไปยังฐานข้อมมูล
  Firebase.setFloat("wei");
  
  // แจ้งหากเกิด error
  if (Firebase.failed()) {
      Serial.print("wefailed:");
      Serial.println(Firebase.error());
      return;
  }
  delay(1000);

    // อัพเดทข้อมูล ความชื้น ไปยังฐานข้อมมูล
  Firebase.setFloat("DHT_Value/Tempurater", wei);
  
  // แจ้งหากเกิด error
  if (Firebase.failed()) {
      Serial.print("Tempurater/Tempurater failed:");
      Serial.println(Firebase.error());
      return;
  }
  delay(1000);

}
