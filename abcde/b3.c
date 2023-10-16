#include <stdio.h>

int main () {
    int k;
    scanf ("%d", &k);
    
    if (k%4 == 0 && k!=1) printf ("NO\n");
    else printf ("YES\n");

    return 0;
}