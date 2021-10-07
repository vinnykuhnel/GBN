#include <iostream>
#include "packet.cpp"
using namespace std;

int main(int argc, char *argv[]){
	char buff[512] = "";
	char test[512];
	packet testing(1, 5, 512, buff);
	testing.serialize(test);
	cout << strlen(test) << endl;
}
