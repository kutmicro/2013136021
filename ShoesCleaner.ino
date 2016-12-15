#include <MsTimer2.h>        // 인터럽트타이머 라이브러리
#include <LiquidCrystal.h>  // CLCD 라이브러리
#include "DHT.h"              // 온습도센서 라이브러리

#define DHT_PIN_IN 21     // 내부DHT의 DATA핀을 19번으로 정의
#define DHT_PIN_OUT 19    // 외부DHT의 DATA핀을 21번으로 정의
#define RELAY1 3      // 릴레이1에 5V 신호를 보낼 핀 정의
#define RELAY2 4      // 릴레이2에 5V 신호를 보낼 핀 정의
#define LBUTTON 22    // 왼쪽 버튼 핀 정의
#define CBUTTON 23    // 중앙 버튼 핀 정의
#define RBUTTON 24    // 오른쪽 버튼 핀 정의
#define LED 30        // LED 핀 정의
#define DHTTYPE DHT11 // DHT타입을 DHT11로 정의

DHT dht_in(DHT_PIN_IN, DHTTYPE);   // DHT설정 - dht (디지털19, dht11)
DHT dht_out(DHT_PIN_OUT, DHTTYPE);  // DHT설정 - dht (디지털21, dht11)
LiquidCrystal lcd(5, 6, 7, 8, 9, 10); // CLCD핀 설정

bool lBtState = false;  // 왼쪽 버튼 상태
bool cBtState = false;  // 중앙 버튼 상태
bool rBtState = false;  // 오른쪽 버튼 상태
int rly1State = LOW;    // 릴레이1(팬모터) 상태
int rly2State = LOW;    // 릴레이2(펠티어) 상태
int ledState = LOW;     // LED 상태
int timeCnt = 0;        // 시간(1초)을 저장할 변수
int startTime = 0;      // 타이머(제습, 소독) 시작 시간
int r2Time = 0;         // r2(펠티어) 시작 시간
bool r2Run = false;

void setup() {
  Serial.begin(9600);    // 9600 속도로 시리얼 통신을 시작
  
  lcd.begin(16, 2);
  
  pinMode (RELAY1, OUTPUT);         // RELAY1 핀을 output으로 설정
  pinMode (RELAY2, OUTPUT);         // RELAY2 핀을 output으로 설정
  pinMode(LED, OUTPUT);             // LED 핀을 output으로 설정
  pinMode(LBUTTON, INPUT_PULLUP);   // 왼쪽 버튼을 풀업스위치로 설정
  pinMode(CBUTTON, INPUT_PULLUP);   // 중앙 버튼을 풀업스위치로 설정
  pinMode(RBUTTON, INPUT_PULLUP);   // 오른쪽 버튼을 풀업스위치로 설정
 
  digitalWrite (RELAY1, LOW);  // relay1 OFF
  digitalWrite (RELAY2, LOW);  // relay2 OFF
  digitalWrite (LED, LOW);     // led OFF
  MsTimer2::set(1000, timerISR);  // 1초마다 timerISR() 함수 호출(인터럽트 타이머)
  MsTimer2::start();              // 인터럽트 타이머 시작
}

void loop() {
  int menu = selectMenu();
  
  if(menu == 0) {
    autoClean();
  }
  
  else if(menu == 1 || menu == -1) {
    manualClean();
  }
  
  else {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("ERROR!!");
    delay(1000);
  }
  
  lcd.clear();
}

// 초기 메뉴 선택 함수
int selectMenu() {
  int bCnt = 0;
  while(1) {
    int lBt = digitalRead(LBUTTON);
    int cBt = digitalRead(CBUTTON);
    int rBt = digitalRead(RBUTTON);
    
    if(lBt == 0) { lBtState = true; }   // 왼쪽 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
    if(cBt == 0) { cBtState = true; }   // 중앙 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
    if(rBt == 0) { rBtState = true; }   // 오른쪽 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
  
    if(lBtState && lBt == 1) {          // 왼쪽 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      bCnt--;                           // 버튼 카운트 변수 1 감소
      lBtState = false;                 // 왼쪽 버튼 상태를 false로 초기화
    }
    
    if(rBtState && rBt == 1) {          // 오른쪽 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      bCnt++;                           // 버튼 카운트 변수 1 증가
      rBtState = false;                 // 오른쪽 버튼 상태를 false로 초기화
    }
    
    bCnt = bCnt % 2;                    // 버튼 카운트 수 계산 (0 or 1)
    Serial.println(bCnt);

    if(cBtState && cBt == 1) {          // 중앙 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      lBtState = false;                 // 왼쪽 버튼 상태를 false로 초기화
      cBtState = false;                 // 중앙 버튼 상태를 false로 초기화
      rBtState = false;                 // 오른쪽 버튼 상태를 false로 초기화
      lcd.clear();
      return bCnt;                      // 버튼 카운트 값을 반환
    }
    
    // LCD에 선택한 메뉴 출력
    lcd.setCursor(2, 0);
    lcd.print("Select Menu");
    lcd.setCursor(0, 1);
    if(bCnt == 0)
      lcd.print("<  Auto Clean  >");
    else
      lcd.print("< Manual Clean >");
  }
}

void autoClean() {
  int hIn = dht_in.readHumidity();
  int hOut = dht_out.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Put the sensor");
  lcd.setCursor(0, 1);
  lcd.print("in the shoe!!");
  delay(500);
  for(int i = 0; i < 3; i++) {
    lcd.noDisplay();
    delay(300);
    lcd.display();
    delay(500);
  }
  
  delay(2000);
  
  while(1) {
    hIn = dht_in.readHumidity();
    hOut = dht_out.readHumidity();

    // 제습 시간 동안 신발장 내부와 외부의 습도 출력
    lcd.setCursor(0, 0);
    lcd.print("Humi In : ");
    lcd.print(hIn);
    lcd.print("%  ");
    lcd.setCursor(0, 1);
    lcd.print("Humi Out : ");
    lcd.print(hOut);
    lcd.print("%  ");
    
    // 내부의 습도가 외부의 습도보다 높으면 
    if(hIn > hOut) {
      digitalWrite (RELAY1, HIGH);
      digitalWrite (RELAY2, HIGH);
      rly1State = HIGH;
    }
    else {
      lcd.clear();
      if(rly1State == HIGH) {
      digitalWrite (RELAY2, LOW);
      rly1State == LOW;
      startTime = timeCnt;
      while (1) {
         if(60 < timeCnt - startTime) {    // 설정시간 + 30초가 지나면
           digitalWrite (RELAY1, LOW);   // 릴레이1 LOW
           rly1State = LOW;        // 릴레이1 상태 LOW로 저장
           r2Run = false;
           startTime = 0;          // 시작 시간 변수 0으로 저장
           break;                  // 반복문 종료
         }
         lcd.setCursor(0, 0);
         lcd.print("Dehumidification");
         lcd.setCursor(0, 1);
         lcd.print("Remaining : ");
         int remainTime = 60 - (timeCnt - startTime);
         if(remainTime > 99)
           lcd.print(remainTime);
         else if(remainTime > 9) {
          lcd.print(" ");
          lcd.print(remainTime);
        }
         else {
          lcd.print("  ");
          lcd.print(remainTime);
        }
      }
      break;
    }
    break;
    }
  }
  disinfec(10);
}

void manualClean() {
  int dehumiTime = 10;
  int disinfectionTime = 10;
  int clcd = 0;
  while(1) {
    int lBt = digitalRead(LBUTTON);
    int cBt = digitalRead(CBUTTON);
    int rBt = digitalRead(RBUTTON);
    
    if(lBt == 0) { lBtState = true; }   // 왼쪽 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
    if(cBt == 0) { cBtState = true; }   // 중앙 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
    if(rBt == 0) { rBtState = true; }   // 오른쪽 버튼이 눌리면 왼쪽 버튼 상태를 TRUE로 초기화
  
    if(lBtState && lBt == 1) {          // 왼쪽 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      lcd.clear();
      if(clcd == 0) { 
        if(dehumiTime > 0)
          dehumiTime--;       // 버튼 카운트 변수 1 감소
      }
      else{
        if(disinfectionTime > 0)
           disinfectionTime--;
      }
      lBtState = false;                 // 왼쪽 버튼 상태를 false로 초기화
    }
    
    if(rBtState && rBt == 1) {          // 오른쪽 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      lcd.clear();
      if(clcd == 0) dehumiTime++;       // 버튼 카운트 변수 1 증가
      else         disinfectionTime++;  // 버튼 카운트 변수 1 증가
      rBtState = false;                 // 오른쪽 버튼 상태를 false로 초기화
    }
    
    if(cBtState && cBt == 1) {          // 중앙 버튼이 눌렸다가 떨어지면 (버튼 1회 눌림)
      lcd.clear();
      cBtState = false;            // 오른쪽 버튼 상태를 false로 초기화
      if(clcd == 0) clcd = 1;             // 버튼 카운트 변수 1 감소
      else {
        clcd == 0;
        dehumi(dehumiTime);
        disinfec(disinfectionTime);
        break;
      }
    }
    
    // LCD에 선택한 메뉴 출력
    if(clcd == 0) {
      lcd.setCursor(0, 0);
      lcd.print("  Dehumi Time  ");
      lcd.setCursor(6, 1);
      lcd.print(dehumiTime);
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("DisinfectionTime");
      lcd.setCursor(6, 1);
      lcd.print(disinfectionTime);
    }
  }
}

void dehumi(int setTime) {      // 제습 함수 (setTime초 만큼 실행)
  while (1) {
    if(rly1State == LOW) {      // 릴레이1(팬)이 LOW이면
      startTime = timeCnt;      // 현재의 시간 카운트 값(timerISR()함수)을 startTime에 저장
      digitalWrite (RELAY1, HIGH);  // 릴레이1 HIGH
      rly1State = HIGH;         // 릴레이1 상태 변수를 HIGH로 저장
    }
    else {                      // 릴레이1 상태가 HIGH이고
      if(setTime + 60 < timeCnt - startTime) {    // 설정시간 + 30초가 지나면
        digitalWrite (RELAY1, LOW);   // 릴레이1 LOW
        rly1State = LOW;        // 릴레이1 상태 LOW로 저장
        r2Run = false;
        startTime = 0;          // 시작 시간 변수 0으로 저장
        break;                  // 반복문 종료
      }
    }
    if(rly2State == LOW && r2Run == false) {
      r2Time = timeCnt;      // 현재의 시간 카운트 값(timerISR()함수)을 startTime에 저장
      digitalWrite (RELAY2, HIGH);  // 릴레이1 HIGH
      rly2State = HIGH;         // 릴레이1 상태 변수를 HIGH로 저장
      r2Run = true;
    }
    else {
      if(setTime < timeCnt - r2Time) {    // 설정시간이 지나면
        digitalWrite (RELAY2, LOW);   // 릴레이1 LOW
        rly2State = LOW;        // 릴레이1 상태 LOW로 저장
        r2Time = 0;          // 시작 시간 변수 0으로 저장
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Dehumidification");
    lcd.setCursor(0, 1);
    lcd.print("Remaining : ");
    int remainTime = setTime + 60 - (timeCnt - startTime);
    if(remainTime > 99)
      lcd.print(remainTime);
    else if(remainTime > 9) {
      lcd.print(" ");
      lcd.print(remainTime);
    }
    else {
      lcd.print("  ");
      lcd.print(remainTime);
    }
  } 
}

void disinfec (int setTime) {
  lcd.clear();
  while (1) {
    if(ledState == LOW) {
      startTime = timeCnt;
      digitalWrite (LED, HIGH);
      ledState = HIGH;
    }
    else {
      if(setTime < timeCnt - startTime) {
        digitalWrite (LED, LOW);
        ledState = LOW;
        startTime = 0;
        break;
      }
    }
    int remainTime = setTime - (timeCnt - startTime);
    lcd.setCursor(0, 0);
    lcd.print("DisInfection");
    lcd.setCursor(0, 1);
    lcd.print("Remaining : ");
      if(remainTime > 99)
      lcd.print(remainTime);
    else if(remainTime > 9) {
      lcd.print(" ");
      lcd.print(remainTime);
    }
    else {
      lcd.print("  ");
      lcd.print(remainTime);
    }
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cleaning");
  lcd.setCursor(0, 1);
  lcd.print("Complete!!");
  delay(500);
  for(int i = 0; i < 3; i++) {
    lcd.noDisplay();
    delay(300);
    lcd.display();
    delay(500);
  }
}

void timerISR() {   // 인터럽트 타이머에 의해 1초마다 호출되는 함수
  timeCnt++;        // 1초마다 timeCnt 변수 1 증가 (timeCnt == 1초)
}
