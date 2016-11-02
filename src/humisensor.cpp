#include "DHT.h"       // DHT.h ���̺귯���� �����Ѵ�
#define DHTPIN 2      // DHT���� 2������ �����Ѵ�(DATA��)
#define DHTTYPE DHT11  // DHTŸ���� DHT11�� �����Ѵ�
DHT dht(DHTPIN, DHTTYPE);  // DHT���� - dht (������2, dht11)

void setup() {
	Serial.begin(9600);    // 9600 �ӵ��� �ø��� ����� �����Ѵ�
}

void loop() {
	delay(2000);
	int h = dht.readHumidity();  // ���� h�� ���� ���� ���� 
	int t = dht.readTemperature();  // ���� t�� �µ� ���� ����
	Serial.print("Humidity: ");  // ���ڿ� Humidity: �� ����Ѵ�.
	Serial.print(h);  // ���� h(����)�� ����Ѵ�.
	Serial.print("%\t");  // %�� ����Ѵ�
	Serial.print("Temperature: ");  // ���ϻ���
	Serial.print(t);
	Serial.println(" C");
}