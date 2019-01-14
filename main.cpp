#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "conv.h"
using namespace std;

bool debug = true;
char* fn_inp_1;
char* fn_inp_2;
char* op_name;
int inp_1_size;
int inp_2_size;
int padsize;
ifstream streamFile;

float conv()
{

}

// main() is where program execution begins.
int main(int argc, char** argv)
{
	// Parsing Arguments
	char opt;
	while((opt = getopt(argc, argv, ":h")) != -1)  
    {  
        switch(opt)  
        {  
            case 'h':
            	cout << "USAGE: ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
            	exit(0);
            	break;
            case '?':  
                // cout << "unknown option: " << opt << endl; 
                break;  
        }  
    } 

  	if (argc > 5){
  		op_name = argv[1];
  		if (!strcmp(op_name, "conv_no_pad_matrix")){
  			if (argc != 6){
	  			cout << "Incorrect Number of Arguments." << endl;
	  			cout << "USAGE: " << endl << " ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
				exit(0);
  			}
  		}else if(!strcmp(op_name, "conv_no_pad_vector")){
  			if (argc != 6){
	  			cout << "Incorrect Number of Arguments." << endl;
	  			cout << "USAGE: " << endl << " ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
				exit(0);
  			}
  		}else if(!strcmp(op_name, "conv_with_padding")){
  			if (argc == 7){
  				padsize = atoi(argv[2]);
  			}else{
	  			cout << "Incorrect Number of Arguments." << endl;
	  			cout << "USAGE: " << endl << " ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
				exit(0);
  			}
  		}else{
  			cout << "Undefined Operation Name: " << argv[1] << endl;
  			cout << "USAGE: " << endl << " ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
  		}
	  	fn_inp_1 = argv[argc-4];
	  	fn_inp_2 = argv[argc-2];
  		inp_1_size = atoi(argv[argc-3]);
  		inp_2_size = atoi(argv[argc-1]);
  		streamFile.open(fn_inp_1);
  		if (!streamFile.good()){
  			cout << "Input File 1 does not exist." << endl;
  			exit(0);
  		}
  		streamFile.close();
  		streamFile.open(fn_inp_2);
  		if (!streamFile.good()){
  			cout << "Input File 2 does not exist." << endl;
  			exit(0);
  		}
  		streamFile.close();
  		if (inp_1_size <= 0){
  			cout << "Invalid Input Size of operand 1" << endl;
  			exit(0);
  		}
  		if (inp_2_size <= 0){
  			cout << "Invalid Input Size of operand 2" << endl;
  			exit(0);
  		}
	  	// if (debug){
		  // 	cout << "Filename of Input 1 = " << fn_inp_1 << endl;
	  	// 	cout << "Filename of Input 1 = " << fn_inp_2 << endl;
		  // 	cout << "Input Size of Input 1 = " << inp_1_size << endl;
	  	// 	cout << "Input Size of Input 2 = " << inp_2_size << endl;
	  	// }
	}else{
		cout << "Some arguments are missing." << endl;
		cout << "USAGE:" << " ' ./conv.out operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size '" << endl;
		exit(0);
	}

	// Take Input
	// Input 1
	float mat1[inp_1_size][inp_1_size];
	streamFile.open(fn_inp_1);
	if (streamFile.good()) {
		for (int i=0; i<inp_1_size; i++){
			for (int j=0; j<inp_1_size; j++){
				streamFile >> mat1[j][i];
			}
		}
		streamFile.close();
		// Display the numbers read:
		cout << "Mat 1 is: " << endl;
		for (int i = 0; i < inp_1_size; i++){
			for (int j = 0; j < inp_1_size; j++){
		    	cout << mat1[i][j] << " ";
			}
			cout << endl;
		}
	}else {
	    cout << "Error!";
	    exit(0);
	}
	// Input 2
	float mat2[inp_2_size][inp_2_size];
	streamFile.open(fn_inp_2);
	if (streamFile.good()) {
		for (int i=0; i<inp_2_size; i++){
			for (int j=0; j<inp_2_size; j++){
				streamFile >> mat2[j][i];
			}
		}
		streamFile.close();
		// Display the numbers read:
		cout << "Mat 2 is: " << endl;
		for (int i = 0; i < inp_1_size; i++){
			for (int j = 0; j < inp_1_size; j++){
		    	cout << mat2[i][j] << " ";
			}
			cout << endl;
		}
	}else {
	    cout << "Error!";
	    exit(0);
	}

	// Convolution Operation

	int image[6][6]={{3,0,1,2,7,4},
					{1,5,8,9,3,1},
					{2,7,2,5,1,3},
					{0,1,3,1,7,8},
					{4,2,1,6,2,8},
					{2,4,5,2,3,8}};

	int ker[3][3] ={{-1,0,1},
					{-1,0,1},
					{-1,0,1}};

	int ker2[1][1] = {{2}};


	conv((int *)image,(int *)ker,6,3);
	return 0;
}

/*
	./conv.out operation inp1 inp1_size inp2 inp2_size
*/