// Lab5
#include <stdio.h>
#include <wiringPi.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
int servo=12;

int btnup=19;
int btndown=16;
int btnleft=18;
int btnright=15;

int Press_udetected=0;
int Press_ddetected=0;
int Press_ldetected=0;
int Press_rdetected=0;

void Press_Interruptl(void) {
	Press_ldetected=1;
	delay(500); }
void Press_Interruptr(void) {
	Press_rdetected=1;
	delay(500); }
void Press_Interruptu(void) {
	Press_udetected=1;
	delay(500); }
void Press_Interruptd(void) {
	Press_ddetected=1;
	delay(500); }
	
int main() {
	wiringPiSetupPhys();
	
	pinMode(btnleft, INPUT);
	pinMode(btnright, INPUT);
	pinMode(btnup, INPUT);
	pinMode(btndown, INPUT); 
	
	pinMode(servo, OUTPUT);
	
	wiringPiISR(btnleft, INT_EDGE_FALLING, &Press_Interruptl);
	wiringPiISR(btnright, INT_EDGE_FALLING, &Press_Interruptr);
	wiringPiISR(btnup, INT_EDGE_FALLING, &Press_Interruptu);
	wiringPiISR(btndown, INT_EDGE_FALLING, &Press_Interruptd);
	
	int t=20000;
	int l=1600;
	
	digitalWrite(servo, 0);
	while(1) 
	{
		if(Press_ldetected==1)
		{
			l=l+200;
			if (l>2400) {l=2400;}
			for (int i=0; i<30; i++) 
			{
				digitalWrite(servo,1); delayMicroseconds(l);
				digitalWrite(servo,0); delayMicroseconds(t-l); 
				}
				Press_ldetected=0; 
			}
		if(Press_rdetected==1)
		{
			l=l-200;
			if (l<600) l=600;
			for (int i=0; i<30; i++) 
			{
				digitalWrite(servo,1); delayMicroseconds(l);
				digitalWrite(servo,0); delayMicroseconds(t-l); 
				}
				Press_rdetected=0; 
			}
		if(Press_udetected==1) 
		{
			while(1) 
			{
				int sec;
				time_t now = time(0);
				struct tm*ltm = localtime(&now);
				sec=ltm->tm_sec;
				l=2500-sec*33;
				for (int i=0; i<30; i++) 
				{
					digitalWrite(servo,1); delayMicroseconds(l);
					digitalWrite(servo,0); delayMicroseconds(t-l); 
					}
				if (Press_ddetected==1) 
				break;
			}
			Press_udetected=0;
			Press_ddetected=0;
		}
	}
}