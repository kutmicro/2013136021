int relay = 10; //�����̿� 5V ��ȣ�� ���� �ɼ���
int x = 5; // �����̸� �״� ���� �� Ƚ�� 5�� ���� x�� ����


void setup ()
{
	pinMode (relay, OUTPUT); // relay�� output���� �����Ѵ�.
}


void loop (){
	for ( int i=0; i==x; i++){ relayOnoff(); } //relay�� x�� On Off
}


void relayOnoff(){ // relay�� On Off�ϴ� �Լ�
	digitalWrite (relay, HIGH); // ������ ON
	delay (500);              //0.5�� delay
	digitalWrite (relay, LOW); // ������ OFF
	delay (500);               //0.5�� delay
}