#include <cstdlib>
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <cmath>
#include <sys/stat.h> 
#include <sys/types.h>
#include <bits/stdc++.h> 

using namespace std;

int main() {
    int START_SIZE=10;
    int END_SIZE=1000;
    int MAT_INC=10;
    int TEST_CASES=10;
    
    int KER_INC=2;
    int KER_MAX_SIZE=9;
    int KER_MIN_SIZE=3;
    
	int total=3000;
	int trials = 10;
	string mult[4] = {"simple", "openblas", "mkl", "pthread"};
	ifstream timefile;
	ofstream meanfile;
    
    if (mkdir("data/mean_sd/", 0777) == -1){
    cerr << "Error :  data/mean_sd " << strerror(errno) << endl;
    }
	for (int type=0; type<4; type++){
		for (int k_size = KER_MIN_SIZE; k_size <= KER_MAX_SIZE; k_size= k_size+KER_INC){
			timefile.open("data/time/time_"+mult[type]+"_"+to_string(k_size)+".txt");
			meanfile.open("data/mean_sd/mean_sd_"+mult[type]+"_"+to_string(k_size)+".txt");
            for (int mat_size=START_SIZE; mat_size<=END_SIZE; mat_size=mat_size+MAT_INC){
                float tmp_time[TEST_CASES];
                float mean = 0;
                int size;
                for (int i=0; i<TEST_CASES; i++){
                    timefile >> tmp_time[i] >> size;
                    mean += tmp_time[i];
                }
                mean = mean/TEST_CASES;
                float sd = 0;
                for (int i=0; i<TEST_CASES; i++){
                    sd += (mean - tmp_time[i])*(mean - tmp_time[i]);
                }
                sd = sqrt(sd/TEST_CASES);
                meanfile << size << " " << mean << " "<< sd << endl;;
            }
			timefile.close();
			meanfile.close();
		}
	}
    
}