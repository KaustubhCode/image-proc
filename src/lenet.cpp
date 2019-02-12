// #include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>		// for floating point precision
// #include <unistd.h>
#include <string.h>
#include <vector>
// #include <boost/program_options.hpp>
// #include <boost/filesystem.hpp>
// #include <chrono> 
// #include "imgOp.h"

using namespace std;
typedef vector<float> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Feature;

class lenet{
	public:

	Feature conv_1_ker;
	vector<float> conv_1_bias;
	Feature conv_2_ker;
	vector<float> conv_2_bias;
	Feature fc_1_ker;
	vector<float> fc_1_bias;
	Feature fc_2_ker;
	vector<float> fc_2_bias;

	// public:

	void load_conv_1(string filename){
		int NO_OF_FILTERS=20;
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=1;
		int BIAS=20;

		conv_1_ker.reserve(NO_OF_FILTERS*INPUT_CHANNELS);
		conv_1_bias.reserve(BIAS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					Matrix mat(KERNEL_SIZE, Array(KERNEL_SIZE));
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> mat[j][k];
						}
					}
					conv_1_ker[i*INPUT_CHANNELS+l] = mat;
				}
			}
			for (int p=0; p<BIAS; p++){
				streamFile >> conv_1_bias[p];
			}
		}
	}

	void load_conv_2(string filename){
		int NO_OF_FILTERS=50;
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=20;
		int BIAS=50;

		conv_2_ker.reserve(NO_OF_FILTERS*INPUT_CHANNELS);
		conv_2_bias.reserve(BIAS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					Matrix mat(KERNEL_SIZE, Array(KERNEL_SIZE));
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> mat[j][k];
						}
					}
					conv_2_ker[i*INPUT_CHANNELS+l] = mat;
				}
			}
			for (int p=0; p<BIAS; p++){
				streamFile >> conv_2_bias[p];
			}
		}
	}

	void load_fc_1(string filename){
		int NO_OF_FILTERS=500;
		int KERNEL_SIZE=4;
		int INPUT_CHANNELS=50;
		int BIAS=500;

		fc_1_ker.reserve(NO_OF_FILTERS*INPUT_CHANNELS);
		fc_1_bias.reserve(BIAS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					Matrix mat(KERNEL_SIZE, Array(KERNEL_SIZE));
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> mat[j][k];
						}
					}
					fc_1_ker[i*INPUT_CHANNELS+l] = mat;
				}
			}
			for (int p=0; p<BIAS; p++){
				streamFile >> fc_1_bias[p];
			}
		}
	}

	void load_fc_2(string filename){
		int NO_OF_FILTERS=10;
		int KERNEL_SIZE=1;
		int INPUT_CHANNELS=500;
		int BIAS=10;

		fc_2_ker.reserve(NO_OF_FILTERS*INPUT_CHANNELS);
		fc_2_bias.reserve(BIAS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					Matrix mat(KERNEL_SIZE, Array(KERNEL_SIZE));
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> mat[j][k];
						}
					}
					fc_2_ker[i*INPUT_CHANNELS+l] = mat;
				}
			}
			for (int p=0; p<BIAS; p++){
				streamFile >> fc_2_bias[p];
			}
		}
	}
	
	Feature run_conv_1(Feature input){
		int INPUT_DIM = 12;
		int NO_OF_FILTERS=50;		// Same as OUTPUT_CHANNELS
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=20;
		int BIAS=50;
		// Stride 1, Padding 0
		int OUTPUT_DIM = INPUT_DIM - KERNEL_SIZE + 1;
		Feature out(NO_OF_FILTERS*INPUT_CHANNELS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			for (int j=0; j<INPUT_CHANNELS; j++){
				Matrix mat = conv(input[i][j], conv_2_ker[i*INPUT_CHANNELS+j]);
				out[i*INPUT_CHANNELS+j]=mat;
			}
		}
	}
	
	Feature run_conv_2(Feature input){
		int INPUT_DIM = 12;
		int NO_OF_FILTERS=50;		// Same as OUTPUT_CHANNELS
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=20;
		int BIAS=50;
		// Stride 1, Padding 0
		int OUTPUT_DIM = INPUT_DIM - KERNEL_SIZE + 1;
		Feature out(NO_OF_FILTERS*INPUT_CHANNELS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			for (int j=0; j<INPUT_CHANNELS; j++){
				Matrix mat = conv(input[i][j], conv_2_ker[i*INPUT_CHANNELS+j]);
				out[i*INPUT_CHANNELS+j]=mat;
			}
		}
	}


};

int main(){
	lenet net;
	net.load_conv_1("lenet_weights/conv1.txt");
	net.load_conv_2("lenet_weights/conv2.txt");
	net.load_fc_1("lenet_weights/fc1.txt");
	net.load_fc_2("lenet_weights/fc2.txt");

}

/*	Run and then check files with 'diff' command
	ofstream streamFile;
	streamFile.open("conv_check.txt");
	// streamFile << std::fixed << std::setprecision(6);
	if (streamFile.good()){
		// Params of layer you are checking
		int NO_OF_FILTERS=10;
		int KERNEL_SIZE=1;
		int INPUT_CHANNELS=500;
		int BIAS=10;
		for (int i=0; i<NO_OF_FILTERS; i++){
			for (int l=0; l<INPUT_CHANNELS; l++){
				for (int j=0; j<KERNEL_SIZE; j++){
					for (int k=0; k<KERNEL_SIZE; k++){
						streamFile << std::fixed << std::setprecision(6) << net.fc_2_ker[i*INPUT_CHANNELS+l][j][k] << endl;
					}
				}
			}
		}
		for (int p=0; p<BIAS; p++){
			streamFile << std::fixed << std::setprecision(6) << net.fc_2_bias[p] << endl;
		}
	}
*/