#include <stdio.h>

int main () {
    int n;
    scanf ("%d", &n);
    int uarr[n], iter = 1, temp;

    for (int i = 0; i < n; i++) scanf ("%d", &uarr[i]);

    temp = uarr[0];
    for (int i = 1; i < n; i++) {
        if (temp != uarr[i]) iter++;
        temp = uarr[i];
    }

    printf ("%3d", iter);

}