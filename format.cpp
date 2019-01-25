#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

int main(){
	ifstream input;
	input.open("time_simple_3.txt");
	ofstream output;
	output.open("time_simple_3.dat");
	if (!input) {
    	cerr << "Unable to read file";
    	exit(1);   // call system to stop
	}
	if (!output) {
    	cerr << "Unable to write file";
    	exit(1);   // call system to stop
	}

	int a,b;
	string m,n;

	output << "#Time Size" << endl;

	while (input >> m >> a >> n >> b){
		output << a << " " << b << endl;
	}

	input.close();
	output.close();

	return 0;
}
