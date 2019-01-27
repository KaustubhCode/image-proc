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
	int mat_size=1000;
	int step = 10;

	string mult[4] = {"simple", "openblas", "mkl", "pthread"};
	ofstream timefile;

	for (int size_ker=3; size_ker<11; size_ker+=2){
		for (int i=0; i<4; i++){
			string outfile = "data/time/time_"+mult[i]+"_"+to_string(size_ker)+".txt";
			timefile.open(outfile);	// Open File to write
			string fn_input_2 = "data/ker/"+to_string(size_ker)+".txt";
			for (int size_image=10; size_image<=mat_size; size_image = size_image + step){
				for (int l=1; l<=10; l++){
					cout << "Type: " << mult[i] << " Kernel: " << size_ker << " Mat: " << size_image << " Case: " << l << endl;
					string fn_input_1 = "data/matrix/"+to_string(size_image)+"x"+to_string(size_image)+"/"+to_string(l)+".txt";
					// Executing Main File
					string cmd = "./bin/main conv_mult_with_pad " +fn_input_1+ " " +to_string(size_image)+ " " +fn_input_2+ " " +to_string(size_ker)+ " 1 " +mult[i];
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
						  timefile << final << " " << size_image << endl;
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