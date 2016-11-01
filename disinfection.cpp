#include "DHT.h"       // DHT.h ���̺귯���� �����Ѵ�
#define DHTPIN 2      // DHT���� 2������ �����Ѵ�(DATA��)
#define DHTTYPE DHT11  // DHTŸ���� DHT11�� �����Ѵ�
#define RELAY 10      // �����̿� 5V ��ȣ�� ���� �ɼ���
DHT dht(DHTPIN, DHTTYPE);  // DHT���� - dht (������2, dht11)

bool opticalHumidity(int humidity, int temperature);

void setup() {
	Serial.begin(9600);    // 9600 �ӵ��� �ø��� ����� �����Ѵ�
	pinMode (RELAY, OUTPUT); // relay�� output���� �����Ѵ�.
}

void loop() {
	delay(1000);
	int h = dht.readHumidity();  // ���� h�� ���� ���� ���� 
	int t = dht.readTemperature();  // ���� t�� �µ� ���� ����

	if(opticalHumidity(h, t)) {
		digitalWrite (RELAY, HIGH); // ������ ON
	}
	else {
		digitalWrite (RELAY, LOW);  // ������ OFF
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