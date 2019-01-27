#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

string exec(const char* cmd) {
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

int main() {
	int mat_size=300;
	string mult[4] = {"simple", "openblas", "mkl", "pthread"};
	ofstream timefile;
	for (int size_2=3; size_2<11; size_2+=2){
		for (int i=0; i<4; i++){
			string outfile = "Performance/time/time_"+mult[i]+"_"+to_string(size_2)+".txt";
			timefile.open(outfile);	// Open File to write
			string fn_input_2 = "Performance/ker/"+to_string(size_2)+".txt";
			for (int size_1=1; size_1<=mat_size; size_1++){
				for (int l=1; l<=10; l++){
					cout << "Type: " << mult[i] << " Kernel: " << size_2 << " Mat: " << size_1 << " Case: " << l << endl;
					string fn_input_1 = "Performance/matrix/"+to_string(size_1)+"x"+to_string(size_1)+"/"+to_string(l)+".txt";
					// Executing Main File
					string cmd = "./bin/main conv_mult_with_pad " +fn_input_1+ " " +to_string(size_1)+ " " +fn_input_2+ " " +to_string(size_2)+ " 1 " +mult[i];
				  cout << cmd << endl;
				  cout << outfile << endl;
				  const char* c_cmd = cmd.c_str();
				  string res = exec(c_cmd);
					  
				  // Extracting Time and Size
				  int first = res.find("Time: ");
				  int last = res.find("microseconds");
				  first = first + 6;
				  cout << first << endl;
				  try{
				  	if (first > 5){
						  string final = res.substr(first, last-first);
						  timefile << final << " " << size_1 << endl;
						}
					}catch(exception e){
						cout << "Invalid" << endl;
					}
				}
			}
  		timefile.close();
		}
	}
  return 0;
}