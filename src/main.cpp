#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "imgOp.h"
using namespace std;

typedef vector<float> Array;
typedef vector<Array> Matrix;

bool debug = true;
string fn_inp_1;
string fn_inp_2;
string op_name;
int op_no;
int inp_1_size;
int inp_2_size;
int padsize;
int box_size;
int stride = 1;
int mult_method = 0; // 0 = simple, 1 = openblas, 2 = mkl
ifstream streamFile;

string operators[10] = {"conv_with_pad",
					"conv_without_pad",
					"conv_mult_with_pad",
					"conv_mult_without_pad",
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
	if (argc == 1){
    	cout << "Invalid usage." << endl << endl << "USAGE:" << endl << "    ./bin/main operation [operation_arg] inp1_filename inp1_size [inp2_filename inp2_size] [other_args]" << endl << endl;
    	cout << "Valid operations are " << endl;
    	for (int i=0; i<10; i++){
    		cout << "    " << operators[i] << endl;
    	}
    	exit(0);
	}
	char opt;
	while((opt = getopt(argc, argv, ":hv")) != -1)  
    {  
        switch(opt)  
        {  
            case 'h':
            	cout << "USAGE:" << endl << "    ./bin/main operation [operation_arg] inp1_filename inp1_size inp2_filename inp2_size" << endl << endl;
            	cout << "Valid operations are " << endl;
            	for (int i=0; i<10; i++){
            		cout << "    " << operators[i] << endl;
            	}
            	exit(0);
            	break;
            case 'v':
            	cout << "image-proc Library: Version 1.0" << endl;
      			exit(0);
      			break;
            case '?':  
                // cout << "unknown option: " << opt << endl; 
                break;  
        }  
    }
	op_name = argv[1];
	op_no = -1;
	for (int i=0; i<10; i++){
		if (!op_name.compare(operators[i])){	
			op_no = i;
		}
	}

	if (op_no == 0 || op_no == 2){
		if (argc < 7 || argc > 8){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << "    ./bin/main " << op_name << " pad_size inp1_filename inp1_size inp2_filename inp2_size {stride}" << endl;
			exit(0);
		}
		padsize = atoi(argv[2]);
	  	fn_inp_1 = argv[3];
  		inp_1_size = atoi(argv[4]);
	  	fn_inp_2 = argv[5];
  		inp_2_size = atoi(argv[6]);
  		if (argc == 8){
  			stride = atoi(argv[7]);
  		}
	}else if (op_no == 1 || op_no == 3){
		if (argc < 6 || argc > 7){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << "    ./bin/main " << op_name << " inp1_filename inp1_size inp2_filename inp2_size {stride}" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
	  	fn_inp_2 = argv[4];
  		inp_2_size = atoi(argv[5]);
  		if (argc == 7){
  			stride = atoi(argv[6]);
  		}
	}else if (op_no >= 4 && op_no <= 5){
		if (argc < 5 || argc > 6){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << "    ./bin/main " << op_name << " inp1_filename inp1_size box_size {stride}" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
  		box_size = atoi(argv[4]);
  		if (argc == 6){
  			stride = atoi(argv[5]);
  		}
	}else if (op_no >= 6 && op_no <= 9){
		if (argc != 4){
			cout << "Incorrect Number of Arguments." << endl << "USAGE: " << endl << "    ./bin/main " << op_name << " inp1_filename inp1_size" << endl;
			exit(0);
		}
	  	fn_inp_1 = argv[2];
  		inp_1_size = atoi(argv[3]);
	}else{
    	cout << "Invalid usage." << endl << endl << "USAGE:" << endl << "    ./bin/main operation [operation_arg / padsize] inp1_filename inp1_size inp2_filename inp2_size" << endl << endl;
    	cout << "Valid operations are " << endl;
    	for (int i=0; i<10; i++){
    		cout << "    " << operators[i] << endl;
    	}
    	exit(0);
	}

	if (inp_1_size <= 0){
    	cout << "Invalid usage: Input 1 size should be positive" << endl;
    	exit(0);
	}

	if (op_no <= 1 && op_no >= 0){
		if (inp_2_size <= 0){
    		cout << "Invalid usage: Input 2 size should be positive" << endl;
    		exit(0);
   		}
   		if (inp_2_size > inp_1_size){
    		cout << "Invalid usage: Kernel/Filter size cannot be greater than image size" << endl;
    		exit(0);
   		}
	}

	// Take Input
	// Input 1

	Matrix mat1(inp_1_size, Array(inp_1_size));
	Matrix mat2(inp_2_size, Array(inp_2_size));
	Array vec1(inp_1_size);
	if (op_no >= 0 && op_no <= 7 ){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i = 0; i < inp_1_size; i++){
				for (int j = 0; j < inp_1_size; j++){
					streamFile >> mat1[j][i];
				}
			}
			streamFile.close();
		}else {
		    cout << "Error: Input File 1 Not Found." << endl;
		    exit(0);
		}
		if (op_no <= 3){
			// Input 2
			streamFile.open(fn_inp_2);
			if (streamFile.good()) {
				for (int i = 0; i < inp_2_size; i++){
					for (int j = 0; j < inp_2_size; j++){
						streamFile >> mat2[j][i];
					}
				}
				streamFile.close();
			}else {
			    cout << "Error: Input File 2 Not Found." << endl;
			    exit(0);
			}
		}
	}else if (op_no >= 8 && op_no <= 9){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i = 0; i < inp_1_size; i++){
				streamFile >> vec1[i];
			}
			streamFile.close();
		}else {
		    cout << "Error: Input File 1 Not Found." << endl;
		    exit(0);
		}
	}

	if (op_no == 0){
		Matrix convImage = conv_pad(mat1,mat2,inp_1_size,inp_2_size,padsize,stride);
		display(convImage);
	}else if (op_no == 1){
		Matrix convImage = conv(mat1,mat2,inp_1_size,inp_2_size,stride);
		display(convImage);
	}else if (op_no == 2){
		Matrix convImage = conv_mult_pad(mat1,mat2,inp_1_size,inp_2_size,padsize,stride);
		display(convImage);
	}else if (op_no == 3){
		Matrix convImage = conv_mult(mat1,mat2,inp_1_size,inp_2_size,stride);
		display(convImage);
	}else if (op_no == 4){
		Matrix convImage = maxPooling(mat1, inp_1_size,box_size,stride);
		display(convImage);
	}else if (op_no == 5){
		Matrix convImage = avgPooling(mat1, inp_1_size,box_size,stride);
		display(convImage);
	}else if (op_no == 6){
		Matrix reluimg = relu(mat1,inp_1_size);
		display(reluimg);
	}else if (op_no == 7){
		Matrix tanhimg = tan_h(mat1,inp_1_size);
		display(tanhimg);
	}else if (op_no == 8){
		Array sigvec = sigmoid(vec1, inp_1_size);
		display_vec(sigvec);
	}else if (op_no == 9){
		Array softmax_vec = softmax(vec1, inp_1_size);
		display_vec(softmax_vec);	
	}
	return 0;
}

/*op_no    op_name
0	./bin/main operation pad_size inp1 inp1_size inp2 inp2_size
1	./bin/main operation inp1 inp1_size inp2 inp2_size
2	./bin/main operation pad_size inp1 inp1_size inp2 inp2_size
3	./bin/main operation inp1 inp1_size inp2 inp2_size
4	./bin/main operation inp1 inp1_size box_size
5	./bin/main operation inp1 inp1_size box_size
6	./bin/main operation inp1 inp1_size
7	./bin/main operation inp1 inp1_size
8	./bin/main operation inp1 inp1_size
9	./bin/main operation inp1 inp1_size

./bin/main conv_with_pad 1 matrix1.txt 6 matrix2.txt 3
./bin/main conv_without_pad matrix1.txt 6 matrix2.txt 3
./bin/main conv_mult_with_pad 1 matrix1.txt 6 matrix2.txt 3
./bin/main conv_mult_without_pad matrix1.txt 6 matrix2.txt 3
./bin/main max_pool matrix1.txt 6 2
./bin/main avg_pool matrix1.txt 6 2
./bin/main relu matrix2.txt 3
./bin/main tanh matrix2.txt 3
./bin/main sigmoid vector1.txt 3
./bin/main softmax vector1.txt 3
*/