#include <iostream>
using namespace std;

void mult(int *mat, int *ker, int n, int m){
	int ans[n-m+1][n-m+1];

	int kerflip[m][m];

	for (int i =0; i < m; i++){
		for (int j = 0; j < m; j++){
			kerflip[i][j] = *((ker + (m-i-1)*m) + m-j-1);
		}
	}

	for (int i =0; i < m; i++){
		for (int j = 0; j < m; j++){
			cout << kerflip[i][j];
			cout << " ";
		}
		cout << endl;
	} 

	cout << "Flipping Done" << endl;

	for(int i = 0; i < n-m+1; i++){
		for(int j = 0; j < n-m+1; j++){
			int sum = 0;
			for(int k = 0; k < m; k++){
				for(int l = 0; l < m; l++){
					sum = sum + *(mat + (i+k)*n + j+l) * kerflip[k][l];
				}
			}
			ans[i][j] = sum;
		}

	}

	for (int i =0; i < n-m+1; i++){
		for (int j = 0; j < n-m+1; j++){
			cout << ans[i][j];
			cout << " ";
		}
		cout << endl;
	} 

	return;
}

int main(){
	int image[6][6]={{3,0,1,2,7,4},
					{1,5,8,9,3,1},
					{2,7,2,5,1,3},
					{0,1,3,1,7,8},
					{4,2,1,6,2,8},
					{2,4,5,2,3,9}};

	int ker[3][3] ={{-1,0,1},
					{-1,0,1},
					{-1,0,1}};

	int ker2[1][1] = {{2}};


	mult((int *)image,(int *)ker,6,3);
	return 0;
}