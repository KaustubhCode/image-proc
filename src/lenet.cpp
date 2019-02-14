#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>	// for sort
#include <string.h>
#include <vector>
#include "imgOp.h"

using namespace std;
typedef vector<float> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Feature;
typedef vector<Feature> FList;
 
Matrix conv3d(Feature input, Feature ker, int pad, int stride){
	int n = input[0].size();
	int newsz = ((n - ker[0].size() + 2*pad)/stride) + 1;
	Matrix output(newsz,Array(newsz));
	for (int i = 0; i < input.size();i++){
		Matrix out = conv_mult_pad(input[i],ker[i],n,ker[0].size(),pad,stride);
		for (int j = 0; j < newsz; j++){
			for (int k = 0; k < newsz; k++){
				output[j][k] = output[j][k] + out[j][k];
			}
		}
	}
	return output;
}

Matrix bias(Matrix input, float b){
	int n = input.size();
	Matrix output(n,Array(n));
	for (int j = 0; j < n; j++){
		for (int k = 0; k < n; k++){
			output[j][k] = input[j][k] + b;
		}
	}
	return output;
}

Feature relu3d(Feature input){
	int n = input[0].size();
	Feature output(input.size(),Matrix(n,Array(n)));
	for (int i = 0; i < input.size();i++){
		for (int j = 0; j < n; j++){
			for (int k = 0; k < n; k++){
				if (input[i][j][k] <= 0){
					output[i][j][k] = 0;
				}
				else{
					output[i][j][k] = input[i][j][k];
				}
			}
		}
	}
	return output;
}

Feature maxpool3d(Feature input, int kernel, int stride){
	int n = input[0].size();
	int newsz = (n-kernel)/stride + 1;
	Feature output(input.size(),Matrix(newsz,Array(newsz)));
	for (int i = 0; i < input.size();i++){
		output[i] = maxPooling(input[i],n,kernel,stride);
	}
	return output;
}

class lenet{
	public:

	FList conv_1_ker;
	vector<float> conv_1_bias;
	FList conv_2_ker;
	vector<float> conv_2_bias;
	FList fc_1_ker;
	vector<float> fc_1_bias;
	FList fc_2_ker;
	vector<float> fc_2_bias;

	void load_conv_1(string filename){
		int NO_OF_FILTERS=20;
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=1;
		int BIAS=20;

		FList featlist(NO_OF_FILTERS, Feature(INPUT_CHANNELS, Matrix(KERNEL_SIZE, Array(KERNEL_SIZE))));
		conv_1_bias.reserve(NO_OF_FILTERS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> featlist[i][l][j][k];
						}
					}
				}
			}
			conv_1_ker = featlist;
			for (int p=0; p<NO_OF_FILTERS; p++){
				streamFile >> conv_1_bias[p];
			}
		}
	}

	void load_conv_2(string filename){
		int NO_OF_FILTERS=50;
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=20;
		int BIAS=50;

		FList featlist(NO_OF_FILTERS, Feature(INPUT_CHANNELS, Matrix(KERNEL_SIZE, Array(KERNEL_SIZE))));
		conv_2_bias.reserve(NO_OF_FILTERS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> featlist[i][l][j][k];
						}
					}
				}
			}
			conv_2_ker = featlist;
			for (int p=0; p<NO_OF_FILTERS; p++){
				streamFile >> conv_2_bias[p];
			}
		}
	}

	void load_fc_1(string filename){
		int NO_OF_FILTERS=500;
		int KERNEL_SIZE=4;
		int INPUT_CHANNELS=50;
		int BIAS=500;

		FList featlist(NO_OF_FILTERS, Feature(INPUT_CHANNELS, Matrix(KERNEL_SIZE, Array(KERNEL_SIZE))));
		fc_1_bias.reserve(NO_OF_FILTERS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> featlist[i][l][j][k];
						}
					}
				}
			}
			fc_1_ker = featlist;
			for (int p=0; p<NO_OF_FILTERS; p++){
				streamFile >> fc_1_bias[p];
			}
		}
	}

	void load_fc_2(string filename){
		int NO_OF_FILTERS=10;
		int KERNEL_SIZE=1;
		int INPUT_CHANNELS=500;
		int BIAS=10;

		FList featlist(NO_OF_FILTERS, Feature(INPUT_CHANNELS, Matrix(KERNEL_SIZE, Array(KERNEL_SIZE))));
		fc_2_bias.reserve(NO_OF_FILTERS);
		ifstream streamFile;
		streamFile.open(filename);
		if (streamFile.good()){
			for (int i=0; i<NO_OF_FILTERS; i++){
				for (int l=0; l<INPUT_CHANNELS; l++){
					for (int j=0; j<KERNEL_SIZE; j++){
						for (int k=0; k<KERNEL_SIZE; k++){
							streamFile >> featlist[i][l][j][k];
						}
					}
				}
			}
			fc_2_ker = featlist;
			for (int p=0; p<NO_OF_FILTERS; p++){
				streamFile >> fc_2_bias[p];
			}
		}
	}

	Feature run_conv_1(Feature input){
		int INPUT_DIM = 28;
		int NO_OF_FILTERS=20;		// Same as OUTPUT_CHANNELS, BIAS
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=1;
		int STRIDE = 1;
		int PADDING = 0;
		int OUTPUT_DIM = (INPUT_DIM-KERNEL_SIZE+2*PADDING)/STRIDE + 1;
		Feature out(NO_OF_FILTERS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			Matrix mat = conv3d(input, conv_1_ker[i], PADDING, STRIDE);
			mat = bias(mat, conv_1_bias[i]);
			out[i]=mat;
		}
		return out;
	}

	Feature run_conv_2(Feature input){
		int INPUT_DIM = 12;
		int NO_OF_FILTERS=50;		// Same as OUTPUT_CHANNELS, BIAS
		int KERNEL_SIZE=5;
		int INPUT_CHANNELS=20;
		int STRIDE = 1;
		int PADDING = 0;
		int OUTPUT_DIM = (INPUT_DIM-KERNEL_SIZE+2*PADDING)/STRIDE + 1;
		Feature out(NO_OF_FILTERS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			Matrix mat = conv3d(input, conv_2_ker[i], PADDING, STRIDE);
			mat = bias(mat, conv_2_bias[i]);
			out[i]=mat;
		}
		return out;
	}

	Feature run_pool_1(Feature input){
		int INPUT_DIM = 24;
		int KERNEL_SIZE=2;
		int INPUT_CHANNELS=20;
		int STRIDE = 2;
		int OUTPUT_DIM = (INPUT_DIM - KERNEL_SIZE)/STRIDE + 1;
		// Feature out(INPUT_CHANNELS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		Feature out = maxpool3d(input, KERNEL_SIZE, STRIDE);
		return out;
	}

	Feature run_pool_2(Feature input){
		int INPUT_DIM = 8;
		int KERNEL_SIZE=2;
		int INPUT_CHANNELS=50;
		int STRIDE = 2;
		int OUTPUT_DIM = (INPUT_DIM - KERNEL_SIZE)/STRIDE + 1;
		// Feature out(NO_OF_FILTERS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		Feature out = maxpool3d(input, KERNEL_SIZE, STRIDE);
		return out;
	}

	Feature run_fc_1(Feature input){
		int INPUT_DIM = 4;
		int NO_OF_FILTERS=500;		// Same as OUTPUT_CHANNELS
		int KERNEL_SIZE=4;
		int INPUT_CHANNELS=50;
		int STRIDE = 1;
		int PADDING = 0;
		int OUTPUT_DIM = (INPUT_DIM-KERNEL_SIZE+2*PADDING)/STRIDE + 1;
		Feature out(NO_OF_FILTERS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			Matrix mat = bias(conv3d(input, fc_1_ker[i], PADDING, STRIDE), fc_1_bias[i]);
			out[i]=mat;
		}
		Feature relu_out = relu3d(out);
		return relu_out;
	}

	Feature run_fc_2(Feature input){
		int INPUT_DIM = 1;
		int NO_OF_FILTERS=10;		// Same as OUTPUT_CHANNELS
		int KERNEL_SIZE=1;
		int INPUT_CHANNELS=500;
		int BIAS=50;
		int STRIDE = 1;
		int PADDING = 0;
		int OUTPUT_DIM = (INPUT_DIM-KERNEL_SIZE+2*PADDING)/STRIDE + 1;
		Feature out(NO_OF_FILTERS, Matrix(OUTPUT_DIM, Array(OUTPUT_DIM)));
		for (int i=0; i<NO_OF_FILTERS; i++){
			Matrix mat = bias(conv3d(input, fc_2_ker[i], PADDING, STRIDE), fc_2_bias[i]);
			out[i]=mat;
		}
		return out;
	}

	vector<float> run(string filename, int CHANNELS, int HEIGHT, int WIDTH){
		Feature input(CHANNELS,Matrix(HEIGHT, Array(WIDTH)));
		ifstream streamFile;
		streamFile.open(filename);
		ofstream tt("test_inp.txt");
		if (streamFile.good()){
			for (int i=0; i<CHANNELS; i++){
				for (int j=0; j<HEIGHT; j++){
					for (int k=0; k<WIDTH; k++){
						streamFile >> input[i][j][k];
						input[i][j][k] = 1 - input[i][j][k]/255; // Normalize and revert image
						tt << input[i][j][k] << endl;
					}
				}
			}
		}
		Feature conv1, conv2, pool1, pool2, fc1, fc2;
		conv1 = this->run_conv_1(input);
		// cout << "Conv1 Dim: " << conv1.size() << " " << conv1[0].size() << " " << conv1[0][0].size() << endl;
		pool1 = this->run_pool_1(conv1);
		// cout << "pool1 Dim: " << pool1.size() << " " << pool1[0].size() << " " << pool1[0][0].size() << endl;
		conv2 = this->run_conv_2(pool1);
		// cout << "conv2 Dim: " << conv2.size() << " " << conv2[0].size() << " " << conv2[0][0].size() << endl;
		pool2 = this->run_pool_2(conv2);
		// cout << "pool2 Dim: " << pool2.size() << " " << pool2[0].size() << " " << pool2[0][0].size() << endl;
		fc1 = this->run_fc_1(pool2);
		// cout << "fc1 Dim: " << fc1.size() << " " << fc1[0].size() << " " << fc1[0][0].size() << endl;
		fc2 = this->run_fc_2(fc1);
		// cout << "fc2 Dim: " << fc2.size() << " " << fc2[0].size() << " " << fc2[0][0].size() << endl;
		vector<float> fc_vec(fc2.size());
		for (int i=0; i<fc2.size(); i++){
			fc_vec[i]=fc2[i][0][0];
		}
		vector<float> sfmax = softmax(fc_vec, fc_vec.size());
		return sfmax;
	}
};

int main(){
	lenet net;
	net.load_conv_1("lenet_weights/conv1.txt");
	net.load_conv_2("lenet_weights/conv2.txt");
	net.load_fc_1("lenet_weights/fc1.txt");
	net.load_fc_2("lenet_weights/fc2.txt");
	vector<float> out = net.run("sample/1/1_new.txt", 1, 28, 28);
	vector<pair<float, int> > probab(out.size());
	for (int i=0; i<out.size(); i++){
		probab[i] = make_pair(out[i], i);
	}
	sort(probab.begin(), probab.end());
	reverse(probab.begin(), probab.end());
	cout << "Top 5 softmax probabilities are: " << endl;
	for (int i=0; i<5; i++){
		cout << "Class " << probab[i].second << ": " << probab[i].first*100 << endl;
	}
}
