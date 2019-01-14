#include <iostream>
#include <vector>

using namespace std;

float** conv_pad(void *mat, void *ker, int n, int m, int p){
	float (*p_mat)[n][n] = (float (*)[n][n]) mat;
	float (*p_ker)[m][m] = (float (*)[m][m]) ker;

	
	float** ans;
	float padimage[n+2*p][n+2*p];
	float kerflip[m][m];
	
	ans = new float*[n-m+1+2*p];
	for (int i=0; i < n; i++){
		ans[i] = new float[n-m+1+2*p];
	}

	for (int i =0; i < m; i++){
		for (int j = 0; j < m; j++){
			kerflip[i][j] = (*p_ker)[m-i-1][m-j-1];
		}
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			padimage[p+i][p+j] = (*p_mat)[i][j];
		}
	}

	for(int i = 0; i < n-m+1; i++){
		for(int j = 0; j < n-m+1; j++){
			int sum = 0;
			for(int k = 0; k < m; k++){
				for(int l = 0; l < m; l++){
					sum = sum + padimage[i+k][j+l] * kerflip[k][l];
				}
			}
			ans[i][j] = sum;
		}

	}

	return ans;


}

float** conv(void *mat, void *ker, int n, int m){

	return conv_pad(mat,ker,n,m,0);

}


void display(float** twoD,int n){
	for (int i = 0; i < n; i++ ){
		for (int j = 0; j < n; j++){
			cout << twoD[i][j];
			cout << " ";
		}
		cout << endl;
	}
}

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

	float **convImage = conv(image,ker,6,3);

	display(convImage, 4);

	return 0;
}