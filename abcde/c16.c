// Вариант С16 переписать это дело надо
#include <iostream>
using namespace std;
int main () {
	int n;
	cin>>n;
	string res = "";
	while (n)
	{ 
		n%2? res+='1' : res+='0';
		n/=2;
	}
    cout<<res;
}