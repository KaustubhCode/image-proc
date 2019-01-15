#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "imgOp.h"
using namespace std;

bool debug = true;
char* fn_inp_1;
char* fn_inp_2;
char* op_name;
int op_no;
int inp_1_size;
int inp_2_size;
int padsize;
int box_size;
ifstream streamFile;

char *operators[8] = {"conv_with_pad",
					"conv_without_pad",
					"max_pool",
					"avg_pool",
					"relu",
					"tanh",
					"sigmoid",
					"softmax"};

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
            	cout << "USAGE: ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
            	exit(0);
            	break;
            case '?':  
                // cout << "unknown option: " << opt << endl; 
                break;  
        }  
    } 
	op_name = argv[1];
	op_no = -1;
	for (int i=0; i<8; i++){
		if (!strcmp(op_name, operators[i])){
			op_no = i;
		}
	}

	if (op_no == 0){
		if (argc != 7){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
		}
		padsize = atoi(argv[2]);
	  	fn_inp_1 = argv[3];
  		inp_1_size = atoi(argv[4]);
	  	fn_inp_2 = argv[5];
  		inp_2_size = atoi(argv[6]);
	}else if (op_no == 1){
		if (argc != 6){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
	  	fn_inp_2 = argv[4];
  		inp_2_size = atoi(argv[5]);
	}else if (op_no >= 2 && op_no <= 3){
		if (argc != 5){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
  		box_size = atoi(argv[4]);
	}else if (op_no >= 4 && op_no <= 7){
		if (argc != 4){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
	}else{

	}
	/*
  	if (argc > 5){
  		
  		if (op_no >= 1 && op_no <= 7){
  			if (argc != 6){
	  			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
				exit(0);
  			}
  		}else if(op_no == 0){
  			if (argc == 7){
  				padsize = atoi(argv[2]);
  			}else{
	  			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
				exit(0);
  			}
  		}else{
  			cout << "Undefined Operation Name: " << argv[1] << endl;
  			cout << "USAGE: " << endl << " ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl;
			exit(0);
  		}
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
		cout << "USAGE:" << " ' ./main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size '" << endl;
		exit(0);
	}
	*/

	// Take Input
	// Input 1

	float mat1[inp_1_size][inp_1_size];
	float mat2[inp_2_size][inp_2_size];
	float vec1[inp_1_size];
	float vec2[inp_2_size];
	if (op_no >= 0 && op_no <= 5 ){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i=0; i<inp_1_size; i++){
				for (int j=0; j<inp_1_size; j++){
					streamFile >> mat1[j][i];
				}
			}
			streamFile.close();
		}else {
		    cout << "Error!";
		    exit(0);
		}
		if (op_no <= 1){
			// Input 2
			streamFile.open(fn_inp_2);
			if (streamFile.good()) {
				for (int i=0; i<inp_2_size; i++){
					for (int j=0; j<inp_2_size; j++){
						streamFile >> mat2[j][i];
					}
				}
				streamFile.close();
				// Display the numbers read:
			}else {
			    cout << "Error!";
			    exit(0);
			}
		}
	}else if (op_no >= 6 && op_no <= 7){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i=0; i<inp_1_size; i++){
				streamFile >> vec1[i];
			}
			streamFile.close();
		}else {
		    cout << "Error! Here";
		    exit(0);
		}
	}

	// Convolution Operation
	float image[6][6]={{3,0,1,2,7,4},
					{1,5,8,9,3,1},
					{2,7,2,5,1,3},
				 	{0,1,3,1,7,8},
					{4,2,1,6,2,8},
					{2,4,5,2,3,9}};

	float ker[3][3] ={{-1,0,1},
					{-1,0,1},
					{-1,0,1}};

	float vec3[3] = {2, 0, -2};

	if (op_no == 0){
		float **convImage = conv_pad(image,ker,6,3,1);
		display(convImage, inp_1_size - inp_2_size + 1 + 2*padsize);
	}else if (op_no == 1){
		float **convImage2 = conv(mat1,mat2,inp_1_size,inp_2_size,1);
		display(convImage2, inp_1_size - inp_2_size + 1);
	}else if (op_no == 2){
		float **convImage2 = maxPooling(mat1, inp_1_size,box_size,1);
		display(convImage2, inp_1_size - box_size + 1);
	}else if (op_no == 3){
		float **convImage2 = avgPooling(mat1, inp_1_size,box_size,1);
		display(convImage2, inp_1_size - box_size + 1);
	}else if (op_no == 4){
		float **tanhimg = relu(mat1,inp_1_size);
		display(tanhimg, inp_1_size);
	}else if (op_no == 5){
		float **tanhimg = tan_h(mat1,inp_1_size);
		display(tanhimg, inp_1_size);
	}else if (op_no == 6){
		float *sigvec = sigmoid(vec1, inp_1_size);
		display_vec(sigvec,inp_1_size);
	}else if (op_no == 7){
		float *softmax_vec = softmax(vec1, inp_1_size);
		display_vec(softmax_vec,inp_1_size);	
	}
	return 0;
}

/*
0	./bin/main operation pad_size inp1 inp1_size inp2 inp2_size
1	./bin/main operation inp1 inp1_size inp2 inp2_size
2	./bin/main operation inp1 inp1_size box_size
3	./bin/main operation inp1 inp1_size box_size
4	./bin/main operation inp1 inp1_size
5	./bin/main operation inp1 inp1_size
6	./bin/main operation inp1 inp1_size
7	./bin/main operation inp1 inp1_size


./bin/main conv_with_pad 1 matrix1.txt 6 matrix2.txt 3
./bin/main conv_without_pad matrix1.txt 6 matrix2.txt 3
./bin/main max_pool matrix1.txt 6 2
./bin/main avg_pool matrix1.txt 6 2
./bin/main relu matrix2.txt 3
./bin/main tanh matrix2.txt 3
./bin/main sigmoid vector1.txt 3
./bin/main softmax vector1.txt 3
*/