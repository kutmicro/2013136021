#include "DHT.h"       // DHT.h 라이브러리를 포함한다
#define DHTPIN 2      // DHT핀을 2번으로 정의한다(DATA핀)
#define DHTTYPE DHT11  // DHT타입을 DHT11로 정의한다
#define RELAY 10      // 릴레이에 5V 신호를 보낼 핀설정
DHT dht(DHTPIN, DHTTYPE);  // DHT설정 - dht (디지털2, dht11)

bool opticalHumidity(int humidity, int temperature);

void setup() {
	Serial.begin(9600);    // 9600 속도로 시리얼 통신을 시작한다
	pinMode (RELAY, OUTPUT); // relay를 output으로 설정한다.
}

void loop() {
	delay(1000);
	int h = dht.readHumidity();  // 변수 h에 습도 값을 저장 
	int t = dht.readTemperature();  // 변수 t에 온도 값을 저장

	if(opticalHumidity(h, t)) {
		digitalWrite (RELAY, HIGH); // 릴레이 ON
	}
	else {
		digitalWrite (RELAY, LOW);  // 릴레이 OFF
	}
}

bool opticalHumidity(int humidity, int temperature) {
	if(temperature <= 10) {
		if(humidity < 70) {
			return true;
		}
		else
			return false;
	}
	else if(temperature <= 18 && temperature > 10) {
		if(humidity < 60) {
			return true;
		}
		else
			return false;
	}
	else if(temperature <= 24 && temperature > 18) {
		if(humidity < 50) {
			return true;
		}
		else
			return false;
	}
	else if(temperature > 24) {
		if(humidity < 40) {
			return true;
		}
		else
			return false;
	}
}