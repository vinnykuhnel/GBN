#include <iostream>
#include "packet.cpp"
using namespace std;


int main(int argc, char *argv[]){
	packet eotPack(3, 0, 0, NULL);
   char buff[512];
   eotPack.serialize(buff);
   cout << strlen(buff) << endl;
}
