#include <iostream>
#include "packet.cpp"
using namespace std;



std::ostream& writeString(std::ostream& out, std::string const& s)
{
   for ( auto ch : s )
   {
      switch (ch)
      {
         case '\'':
            out << "\\'";
            break;

         case '\"':
            out << "\\\"";
            break;

         case '\0':
            out << "\\0";
            break;

         case '\\':
            out << "\\\\";
            break;

         case '\a':
            out << "\\a";
            break;

         case '\b':
            out << "\\b";
            break;

         case '\f':
            out << "\\f";
            break;

         case '\n':
            out << "\\n";
            break;

         case '\r':
            out << "\\r";
            break;

         case '\t':
            out << "\\t";
            break;

         case '\v':
            out << "\\v";
            break;

         default:
            out << ch;
      }
   }

   return out;
}

int main(int argc, char *argv[]){
	char buff[512] = "Hello this is my message";
	char test[512];
	packet testing(1, 5, 512, buff);
	testing.serialize(test);
	writeString(cout, test);
	testing.deserialize(test);
	testing.printContents();
	cout << endl;
	return 0;
}
