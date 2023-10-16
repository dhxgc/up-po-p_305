// Lab1
#include <stdio.h>
#include <wiringPi.h>
int main() {
	if (wiringPiSetupPhys()==-1) return 0;
	int btn_left=18;
	int led1=31;
	int led2=33;
	int led3=35;
	int led4=32; 
	pinMode(btn_left, INPUT);
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);
	pinMode(led4, OUTPUT); 
	while(1) {
		digitalWrite(led1, 1);
		delay (100);
		digitalWrite(led1, 0);
		delay (10); 
		digitalWrite(led2, 1);
		delay (100);
		digitalWrite(led2, 0);
		delay (10); 
		digitalWrite(led3, 1);
		delay (100);
		digitalWrite(led3, 0);
		delay (10); 
		digitalWrite(led4, 1);
		delay (100);
		digitalWrite(led4, 0);
		delay (10); 
		if (digitalRead(btn_left)==0) return 1;
	}
}