#include <iostream>
#include <bits/stdc++.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <fstream>
#include <string>
#include <unistd.h>
#include <cstdlib>

using namespace std;

int main(){
  int range = 1000; //from -range to +range
  int mat_size = 300;
  if (mkdir("Performance/matrix/", 0777) == -1){
      cerr << "Error :  " << strerror(errno) << endl;
    }
  for (int i = 1; i <= mat_size;i++){
      if (mkdir(("Performance/matrix/"+to_string(i)+"x"+to_string(i)).c_str(), 0777) == -1){
          cerr << "Error :  " << strerror(errno) << endl;
        } 
      else{
          cout << "Directory created" << i << "x" << i << endl; 
      }
  
      string dir = "./Performance/matrix/";
      dir += to_string(i)+"x"+to_string(i) + "/";

      cout << "Entered Directory" << dir << endl;
      for (int j = 0; j < 10; j++){
        ofstream file;
        string fname = dir + to_string(j+1) + ".txt";
        file.open (fname, ios::out);
        for (int k = 0; k < i*i; k++) {
          file << (rand()%(2*range)) - range << endl;
        }
        file.close();
      }
      cout << "Directory  " << i << " completed" << endl; 
  }
  return 0;

}