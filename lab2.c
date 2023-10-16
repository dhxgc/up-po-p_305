// Lab2
#include <stdio.h>
#include <wiringPi.h>
int btn_up=19, btn_left=18;
int led1=31, led2=33, led3=35, led4=32;

int Press_up_detected=0;
int Press_left_detected=0;


void Press_up_Interrupt(void) {
	Press_up_detected=1;
	delay(20);
}

void Press_left_Interrupt(void) {
	Press_left_detected=1;
	delay(20);
}


int main() {
	wiringPiSetupPhys();
	pinMode(btn_up, INPUT);
	pinMode(btn_left, INPUT);
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);
	pinMode(led4, OUTPUT);
	
	wiringPiISR(btn_up,INT_EDGE_FALLING, &Press_up_Interrupt);
	wiringPiISR(btn_left,INT_EDGE_FALLING, &Press_left_Interrupt);
	
	while(1) {
		if(Press_left_detected==1 && Press_up_detected==1) {
			//printf("Button UP and LEFT pressed. Detected by Interrupt\n");
			Press_left_detected=0;
			Press_up_detected=0;
			
			digitalWrite(led1,1);
			delay(100);
			digitalWrite(led1,0);
			delay(100);
			
			digitalWrite(led2,1);
			delay(100);
			digitalWrite(led2,0);
			delay(100);

			digitalWrite(led3,1);
			delay(100);
			digitalWrite(led3,0);
			delay(100);
			
			digitalWrite(led4,1);
			delay(100);
			digitalWrite(led4,0);
			delay(1000);
			continue;
			}	
		if(Press_up_detected==1 && Press_left_detected==0) {
			//printf("Button UP pressed. Detected by Interrupt\n");
			Press_up_detected=0;
			
			digitalWrite(led3,1);
			digitalWrite(led4,1);
			delay(500);
			digitalWrite(led3,0);
			digitalWrite(led4,0);
			delay(500);
			continue;
			}
			
			
		if(Press_left_detected==1 && Press_up_detected==0) {
			//printf("Button LEFT pressed. Detected by Interrupt\n");
			Press_left_detected=0;
			
			digitalWrite(led1,1);
			digitalWrite(led2,1);
			delay(500);
			digitalWrite(led1,0);
			digitalWrite(led2,0);
			delay(500);
			continue;
			}		
		delay(200);
	}
}