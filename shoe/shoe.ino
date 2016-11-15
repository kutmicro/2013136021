#include <MsTimer2.h>
#include <LiquidCrystal.h>
#include "DHT.h"       // DHT.h 라이브러리를 포함한다

#define DHTPIN 2      // DHT핀을 2번으로 정의한다(DATA핀)
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다
#define RELAY1 3      // 릴레이1에 5V 신호를 보낼 핀설정
#define RELAY2 4      // 릴레이2에 5V 신호를 보낼 핀설정

BOOL lampOn = false;

DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)
LiquidCrystal lcd(5, 6, 7, 8, 9, 10); // CLCD 핀 설정

void setup() {
  Serial.begin(9600);    // 9600 속도로 시리얼 통신을 시작한다
  
  lcd.begin(16, 2);
  
  pinMode (RELAY1, OUTPUT); // relay1를 output으로 설정한다.
  pinMode (RELAY2, OUTPUT); // relay2를 output으로 설정한다.

  digitalWrite (RELAY1, HIGH);  // relay1 OFF
  digitalWrite (RELAY2, HIGH);  // relay2 OFF
}

void loop() {
  int h = dht.readHumidity();  // 변수 h에 습도 값을 저장
  int t = dht.readTemperature();  // 변수 t에 온도 값을 저장
  
  lcdPrintHumi(h, t); // lcd에 온도, 습도 값 풀력

  if(opticalHumidity(h, t)) {
    digitalWrite (RELAY2, LOW);  // relay2 ON
    lampOn = true;
  }
  else {
    digitalWrite (RELAY2, HIGH);  // relay2 OFF
    if(lampOn) {
      digitalWrite (RELAY1, LOW);  // relay1 OFF
      //timer
      lampOn = false;
    }
  }
}

void lcdPrintHumi(int h, int t) {   // 온습도센서에서 습도를 받아 lcd에 출력하는 함수
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
}

bool opticalHumidity(int h, int t) {
  if (t <= 10)
    return h < 70;
  else if (t <= 18 && t > 10)
    return h < 60;
  else if (t <= 24 && t > 18)
    return h < 50;
  else if (t > 24)
    return h < 40;
}
