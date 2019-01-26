#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <cmath>

using namespace std;

int main() {
	int total=3000;
	int trials = 10;
	string mult[4] = {"simple", "openblas", "mkl", "pthread"};
	ifstream timefile;
	ifstream meanfile;
	ofstream plotfile;
	for (int type=0; type<4; type++){
		for (int k_size = 3; k_size < 11; k_size+=2){
			// int type = 0, k_size = 3;
			timefile.open("Performance/time/time_"+mult[type]+"_"+to_string(k_size)+".txt");
			plotfile.open("Performance/mean/mean_"+mult[type]+"_"+to_string(k_size)+".txt");
			int mean=0;
			for (int m = 0; m<total-(k_size-1)*10; m++){
				int tmp, size;
				timefile >> tmp >> size;
				mean += tmp;
				// cout << m+1 << " entry: " << mean << " " << size << endl;
				if ((m+1)%trials == 0){
					mean /= trials;
					plotfile << mean << " " << size << endl;
					mean = 0;
				}
			}
			timefile.close();
			plotfile.close();
		}
	}
	for (int type=0; type<4; type++){
		for (int k_size = 3; k_size < 11; k_size+=2){
			timefile.open("Performance/time/time_"+mult[type]+"_"+to_string(k_size)+".txt");
			meanfile.open("Performance/mean/mean_"+mult[type]+"_"+to_string(k_size)+".txt");
			plotfile.open("Performance/sd/sd_"+mult[type]+"_"+to_string(k_size)+".txt");
			float sdev=0;
			for (int m = 0; m<total/trials-(k_size-1); m++){
				int tmp, size, mean;
				meanfile >> mean >> size;
				cout << m+1 << " entry: " << mean << " " << size << endl;
				for (int t=0; t<trials; t++){
					timefile >> tmp >> size;
					sdev += (tmp-mean)*(tmp-mean);
				}
				sdev = sqrt(sdev);
				plotfile << sdev << " " << size << endl;
				sdev = 0;
			}
			timefile.close();
			meanfile.close();
			plotfile.close();
		}
	}
}