// Вариант С12
 #include <stdio.h>
 int main() {
	int a,max,i=0;
	do {
		if(a>max && i!=0) {
			max=a;
			}
		scanf("%i",&a);
		if(i==0) {
			max=a;
			}
		i++;
	} while (a!=0);
	printf("%i",max);

    return 0;
}