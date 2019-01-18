#include <iostream>
#include <vector>
#include <bits/stdc++.h> // for tanh, max and exp functions

using namespace std;

typedef vector<float> Array;
typedef vector<Array> Matrix;

// Display Matrix
void display(Matrix twoD){
    for (int i = 0; i < twoD.size(); i++ ){
        for (int j = 0; j < twoD[0].size(); j++){
            cout << twoD[i][j];
            cout << " ";
        }
        cout << endl;
    }
}

// Display Vector
void display_vec(Array oneD){
    for (int i = 0; i < oneD.size(); i++ ){
        cout << oneD[i];
        cout << " ";
    }
    cout << endl;
}

// Convolution with padding (matrix)
Matrix conv_pad(Matrix mat, Matrix ker, int n, int m, int p, int s = 1){

    int newsz = (n-m+2*p)/s +1;
    
    Matrix ans(newsz, Array(newsz));
    Matrix padimage(n+2*p, Array(n+2*p));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            padimage[p+i][p+j] = mat[i][j];
        }
    }

    for(int i = 0; i < (n-m+2*p)/s + 1; i=i+s){
        for(int j = 0; j < (n-m+2*p)/s + 1; j=j+s){
            int sum = 0;
            for(int k = 0; k < m; k++){
                for(int l = 0; l < m; l++){
                    sum = sum + padimage[i+k][j+l] * ker[m-k-1][m-l-1];
                }
            }
            ans[i][j] = sum;
        }
    }

    return ans;
}

// Convolution without padding (matrix)
Matrix conv(Matrix mat, Matrix ker, int n, int m, int s = 1){
    return conv_pad(mat,ker,n,m,0,s);
}

Matrix conv_mult_pad(Matrix mat, Matrix ker, int n, int m, int p, int s = 1){
    int newsz = n-m+2*p+1;
    int n_pad = n+2*p;
    Matrix ans(newsz, Array(newsz));
    Matrix padimage(n_pad, Array(n_pad));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            padimage[p+i][p+j] = mat[i][j];
        }
    }

    Matrix proc_image((n_pad-m+1)*(n_pad-m+1), Array(m*m));

    for(int i = 0; i < n_pad-m+1; i++){
        for(int j = 0; j < n_pad-m+1; j++){
            for(int k = 0; k < m; k++){
                for(int l = 0; l < m; l++){
                    proc_image[i*(n_pad-m+1)+j][k*m+l] = padimage[i+k][j+l];
                }
            }
        }
    }
    Array proc_ker(m*m);
    for(int k = 0; k < m; k++){
        for(int l = 0; l < m; l++){
            proc_ker[k*m+l] = ker[m-k-1][m-l-1];
        }
    }

    for (int i=0; i<newsz; i++){
    	for (int j=0; j<newsz; j++){
    		ans[i][j]=0;
    		for (int k=0; k<m*m; k++){
    			ans[i][j] += proc_image[i*newsz+j][k] * proc_ker[k];
    		}
    	}
    }
    return ans;
}

Matrix conv_mult(Matrix mat, Matrix ker, int n, int m, int s = 1){
    return conv_mult_pad(mat,ker,n,m,0);
}

Matrix maxPooling(Matrix mat, int n, int f, int s = 1){

    int newsz = (n-f)/s+1;
    Matrix ans(newsz,Array(newsz));

    for (int i = 0; i < (n-f)/s + 1; i=i+s){
        for (int j = 0; j < (n-f)/s + 1; j=j+s){
            float max = mat[i][j];
            for (int k = 0; k < f; k++){
                for (int l = 0; l < f; l++){
                    max = (mat[i+k][j+l] > max) ? mat[i+k][j+l] : max;
                }
            }
            ans[i][j] = max;
        }
    }

    return ans;
}

Matrix avgPooling(Matrix mat, int n, int f, int s = 1){

    int newsz = (n-f)/s+1;
    Matrix ans(newsz,Array(newsz));

    for (int i = 0; i < (n-f)/s + 1; i=i+s){
        for (int j = 0; j < (n-f)/s + 1; j=j+s){
            float sum = 0;
            for (int k = 0; k < f; k++){
                for (int l = 0; l < f; l++){
                    sum = sum + mat[i+k][j+l];
                }
            }
            ans[i][j] = sum/(f*f);
        }
    }

    return ans;
}

// ReLu (matrix)
Matrix relu(Matrix mat, int n){

    Matrix ans(n,Array(n));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (mat[i][j] > 0){
                ans[i][j] = mat[i][j];
            }else{
                ans[i][j] = 0;
            }
        }
    }

    return ans;
}

// tanh (matrix)
Matrix tan_h(Matrix mat, int n){

    Matrix ans(n,Array(n));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            ans[i][j] = tanh(mat[i][j]);
        }
    }

    return ans;
}

// Sigmoid (vector)
Array sigmoid(Array mat, int n){

    Array ans(n);

    for (int i = 0; i < n; i++){
        ans[i] = 1/(1+exp(-(mat[i])));
    }

    return ans;
}

// softmax (vector)
Array softmax(Array mat, int n){
    
    Array ans(n);
    
    float exp_sum = 0;
    for (int i = 0; i < n; i++){
        exp_sum = exp_sum + exp(mat[i]);
    }

    for (int i = 0; i < n; i++){
        ans[i] = exp(mat[i])/exp_sum;
    }

    return ans;
}