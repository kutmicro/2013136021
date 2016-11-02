int relay = 10; //릴레이에 5V 신호를 보낼 핀설정
int x = 5; // 릴레이를 켰다 껏다 할 횟수 5를 변수 x에 저장


void setup ()
{
	pinMode (relay, OUTPUT); // relay를 output으로 설정한다.
}


void loop (){
	for ( int i=0; i==x; i++){ relayOnoff(); } //relay를 x번 On Off
}


void relayOnoff(){ // relay를 On Off하는 함수
	digitalWrite (relay, HIGH); // 릴레이 ON
	delay (500);              //0.5초 delay
	digitalWrite (relay, LOW); // 릴레이 OFF
	delay (500);               //0.5초 delay
}