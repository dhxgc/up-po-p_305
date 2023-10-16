// Вариант С10
#include <stdio.h>
#include <stdlib.h>
int main() {
	float x,p,y;
	int i=0;
	scanf("%f%f%f",&x,&p,&y);
	for(i=0; x<y; i++) {
		x=((x*(p/100))+x);
		}
	printf("%i",i);
    return 0;
}