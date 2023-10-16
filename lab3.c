// Lab3
#include <stdio.h>
#include <wiringPi.h>
int j = 0;
char Light_detected=0;
int LightPin=22;
int VoicePin=21;
int k,c, p;
char Voice_detected=0;
void LightSensorInterrupt(void) {
	Light_detected=1;
}
char get_digital_light(void) {
	int LightFactor=0;
	if (Light_detected==1)
		LightFactor++;
	Light_detected=0;
	if(!digitalRead(LightPin))
		LightFactor++;
	if (LightFactor<1) 
		return 0;
	else
		return 1;
}
void VoiceSensorInterrupt(void) {
	Voice_detected=1;
}
char get_digital_voice(void) {
	int VoiceFactor=0;
	if (Voice_detected==1)
		VoiceFactor++;
	Voice_detected=0;
	if(!digitalRead(VoicePin))
		VoiceFactor++;
	if (VoiceFactor<1) 
		return 0;
	else
		return 1;
}
void Press_right (void){
	k=1;
}
void Press_btn (void){
	j=1; }
void Press_left (void){
	c=1;
}
void Press_dwn (void){
	p=1;
}
int main(void) {
	if (wiringPiSetupPhys()==-1) return 0;
	int btn_up=19;
	pinMode (18, INPUT);
	pinMode (16, INPUT);
	pinMode(btn_up, INPUT);
	wiringPiSetupPhys();
	pinMode(LightPin, INPUT);
	pinMode(VoicePin, INPUT);
	wiringPiISR(18, INT_EDGE_BOTH, &Press_left);
	wiringPiISR(15, INT_EDGE_BOTH, &Press_right);
	wiringPiISR(16, INT_EDGE_BOTH, &Press_dwn);
	wiringPiISR(19, INT_EDGE_BOTH, &Press_btn);
	wiringPiISR (LightPin,INT_EDGE_BOTH, &LightSensorInterrupt);
	wiringPiISR (VoicePin,INT_EDGE_BOTH, &VoiceSensorInterrupt);
	while (1) {
		k=0;
		if (k==1) {
			
			while(p!=1){
				k=0;
			if (get_digital_light()==1) {printf("on\n");}
			else {printf ("off\n");}
			delay (100); 
			}
			
			p=0;
	}
		if (c==1) {
			c=0;
			while(j!=1){
			if (get_digital_voice()==1) {printf("zvuk\n");}
			else {printf ("tiho\n");}
			delay (100);
			
		}
		j=0;
	}
	}
}