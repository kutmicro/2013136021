#include <LiquidCrystal.h>
#include "DHT.h"       // DHT.h 라이브러리를 포함한다
#define DHTPIN 2      // DHT핀을 2번으로 정의한다(DATA핀)
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다
DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)

LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

void setup() {
  Serial.begin(9600);    // 9600 속도로 시리얼 통신을 시작한다
   // set up the LCD's number of columns and rows:
   lcd.begin(16, 2);
   // Print a message to the LCD.
   
}

void loop() {
   int h = dht.readHumidity();  // 변수 h에 습도 값을 저장 
   int t = dht.readTemperature();  // 변수 t에 온도 값을 저장
   // set the cursor to column 0, line 1
   // (note: line 1 is the second row, since counting begins with 0):
   lcd.setCursor(0, 0);
   lcd.print("Temp : ");
   lcd.print(t);
   lcd.print("'C");
   lcd.setCursor(0, 1);
   lcd.print("Humi : ");
   lcd.print(h);
   lcd.print("%");
   // print the number of seconds since reset:
   //lcd.print(millis() / 1000);
}
