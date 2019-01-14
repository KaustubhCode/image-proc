#include <iostream>
#include <vector>
#include <bits/stdc++.h> // for tanh, max and exp functions

using namespace std;

// Convolution with padding (matrix)
float** conv_pad(void *mat, void *ker, int n, int m, int p, int s = 1){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;
	float (*p_ker)[m][m] = (float (*)[m][m]) ker;

	
	float** ans;

	float padimage[(n+2*p)/s][(n+2*p)/s];

	ans = new float*[(n-m+2*p)/s + 1];
	for (int i=0; i < n; i++){
		ans[i] = new float[(n-m+2*p)/s + 1];
	}

	for (int i = 0; i < n+2*p; i++){
		for (int j = 0; j < n + 2*p; j ++){
			padimage[i][j] = 0;
		}
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			padimage[p+i][p+j] = (*p_mat)[i][j];
		}
	}

	for(int i = 0; i < (n-m+2*p)/s + 1; i++){
		for(int j = 0; j < (n-m+2*p)/s + 1; j++){
			int sum = 0;
			for(int k = 0; k < m; k++){
				for(int l = 0; l < m; l++){
					sum = sum + padimage[i+k][j+l] * (*p_ker)[m-k-1][m-l-1];
				}
			}
			ans[i][j] = sum;
		}
	}

	return ans;
}

// Convolution without padding (matrix)
float** conv(void *mat, void *ker, int n, int m, int s = 1){
	return conv_pad(mat,ker,n,m,0);
}

float** maxPooling(void *mat, int n, int f, int s = 1){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;
	float** ans;

	ans = new float*[n-f+ 1];
	for (int i=0; i < n; i++){
		ans[i] = new float[n-f + 1];
	}

	for (int i = 0; i < n-f + 1; i++){
		for (int j = 0; j < n-f + 1; j++){
			float max = (*p_mat)[i][j];
			for (int k = 0; k < f; k++){
				for (int l = 0; l < f; l++){
					max = ((*p_mat)[i+k][j+l] > max) ? (*p_mat)[i+k][j+l] : max;
				}
			}
			ans[i][j] = max;
		}
	}

	return ans;
}

float** avgPooling(void *mat, int n, int f, int s = 1){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;
	float** ans;

	ans = new float*[n-f+ 1];
	for (int i=0; i < n; i++){
		ans[i] = new float[n-f + 1];
	}

	for (int i = 0; i < n-f + 1; i++){
		for (int j = 0; j < n-f + 1; j++){
			float sum = 0;
			for (int k = 0; k < f; k++){
				for (int l = 0; l < f; l++){
					sum = sum + (*p_mat)[i+k][j+l];
				}
			}
			ans[i][j] = sum/(f*f);
		}
	}

	return ans;
}

// ReLu (matrix)
float** relu(void *mat, int n){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;

	float** ans;
	
	ans = new float*[n];
	for (int i=0; i < n; i++){
		ans[i] = new float[n];
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if ((*p_mat)[i][j] > 0){
				ans[i][j] = (*p_mat)[i][j];
			}else{
				ans[i][j] = 0;
			}
		}
	}

	return ans;
}

// tanh (matrix)
float** tan_h(void *mat, int n){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;

	float** ans;
	
	ans = new float*[n];
	for (int i=0; i < n; i++){
		ans[i] = new float[n];
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			ans[i][j] = tanh((*p_mat)[i][j]);
		}
	}

	return ans;
}

// Sigmoid (vector)
float* sigmoid(void *mat, int n){
	float (*p_mat)[n] = (float (*)[n]) mat;

	float* ans;
	
	ans = new float[n];

	for (int i = 0; i < n; i++){
		ans[i] = 1/(1+exp(-((*p_mat)[i])));
	}

	return ans;
}

// softmax (vector)
float* softmax(void *mat, int n){
	float (*p_mat)[n] = (float (*)[n]) mat;

	float* ans;
	
	float exp_sum = 0;
	for (int i = 0; i < n; i++){
		exp_sum = exp_sum + exp((*p_mat)[i]);
	}

	ans = new float[n];

	for (int i = 0; i < n; i++){
		ans[i] = exp((*p_mat)[i])/exp_sum;
	}

	return ans;
}

// Display Matrix
void display(float** twoD,int n){
	for (int i = 0; i < n; i++ ){
		for (int j = 0; j < n; j++){
			cout << twoD[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

// Display Vector
void display_vec(float* twoD,int n){
	for (int i = 0; i < n; i++ ){
		cout << twoD[i];
		cout << " ";
	}
	cout << endl;
}

/*
int main(){
	float image[6][6]={{3,0,1,2,7,4},
					{1,5,8,9,3,1},
					{2,7,2,5,1,3},
				 	{0,1,3,1,7,8},
					{4,2,1,6,2,8},
					{2,4,5,2,3,9}};

	float ker[3][3] ={{-1,0,1},
					{-1,0,1},
					{-1,0,1}};

	float **convImage = conv_pad(image,ker,6,3,1);
	display(convImage, 6);

	float **tanhimg = tan_h(ker,3);
	display(tanhimg, 3);

	float *sigvec = sigmoid(vec1, 3);
	display_vec(sigvec,3);

	float *softmax_vec = softmax(vec1, 3);
	display_vec(softmax_vec,3);
}
*/