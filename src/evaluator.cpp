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
	string mult[4] = {"simple", "openblas", "mkl", "pthread"};
	ofstream timefile;
	for (int i=0; i<4; i++){
		for (int size_2=3; size_2<11; size_2+=2){
			string outfile = "time_"+mult[i]+"_"+to_string(size_2)+".txt";
			timefile.open(outfile);	// Open File to write
			string fn_input_2 = "ker_"+to_string(size_2)+".txt";
			for (int size_1=0; size_1<1000; size_1++){
				for (int l=0; l<10; l++){
					string fn_input_1 = "mat_"+to_string(size_1)+"_"+to_string(l)+".txt";
					// Executing Main File
					string cmd = "./bin/main conv_mult_with_pad matrix/" +to_string(size_1)+ "/" +fn_input_1+ " " +to_string(size_1)+ " " +fn_input_2+ " " +to_string(size_2)+ " 1 " +mult[i];
				  const char* c_cmd = cmd.c_str();
				  string res = exec(c_cmd);
				  
				  // Extracting Time and Size
				  int first = res.find("Time: ");
				  int last = res.find("microseconds");
				  string final = res.substr(first, last-first);
				  timefile << final << endl;
				}
			}
  		timefile.close();
		}
	}

  // cout << "First: " << first << " Last: " << last << endl;
  // cout << final << endl;
  // cout << " TEST " << endl << res << endl;
  return 0;
}