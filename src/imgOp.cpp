#include <iostream>
#include <vector>
#include <bits/stdc++.h> // for tanh, max and exp functions
#include "mkl.h"

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

Matrix simple_multiplication(Matrix proc_image, Array proc_ker, int n, int m){
    Matrix ans(n, Array(n));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            ans[i][j]=0;
            for (int k=0; k<m*m; k++){
                ans[i][j] += proc_image[i*n+j][k] * proc_ker[k];
            }
        }
    }
    return ans;
}

Matrix mult_openblas(Matrix proc_image, Array proc_ker){
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");

    m = proc_image.size(), k = proc_ker.size, n = 1;
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    alpha = 1.0; beta = 0.0;

    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl_free(A);
      mkl_free(B);
      mkl_free(C);
      return 1;
    }

    printf (" Intializing matrix data \n\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < k; j++){
            A[i*m+k] = (double)proc_image[i][j];
        }
    }

    for (i = 0; i < (k*n); i++) {
        B[i] = (double)proc_ker[i];
    }

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }

    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                m, n, k, alpha, A, k, B, n, beta, C, n);
    printf ("\n Computations completed.\n\n");

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(k,6); j++) {
        printf ("%12.0f", A[j+i*k]);
      }
      printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.0f", B[j+i*n]);
      }
      printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
      for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
      }
      printf ("\n");
    }

    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
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

    for(int i = 0; i < (n-m+2*p)/s + 1; i++){
        for(int j = 0; j < (n-m+2*p)/s + 1; j++){
            int sum = 0;
            for(int k = 0; k < m; k++){
                for(int l = 0; l < m; l++){
                    sum = sum + padimage[i*s+k][j*s+l] * ker[m-k-1][m-l-1];
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
    int newsz = (n-m+2*p)/s+1;
    int n_pad = n+2*p;
    Matrix padimage(n_pad, Array(n_pad));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            padimage[p+i][p+j] = mat[i][j];
        }
    }

    Matrix proc_image(newsz*newsz, Array(m*m));

    for(int i = 0; i < newsz; i++){
        for(int j = 0; j < newsz; j++){
            for(int k = 0; k < m; k++){
                for(int l = 0; l < m; l++){
                    proc_image[i*newsz+j][k*m+l] = padimage[i*s+k][j*s+l];
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

    return simple_multiplication(proc_image,proc_ker,newsz,m);
}

Matrix conv_mult(Matrix mat, Matrix ker, int n, int m, int s = 1){
    return conv_mult_pad(mat,ker,n,m,0,s);
}

Matrix maxPooling(Matrix mat, int n, int f, int s = 1){

    int newsz = (n-f)/s+1;
    Matrix ans(newsz,Array(newsz));

    for (int i = 0; i < (n-f)/s + 1; i++){
        for (int j = 0; j < (n-f)/s + 1; j++){
            float max = mat[i][j];
            for (int k = 0; k < f; k++){
                for (int l = 0; l < f; l++){
                    max = (mat[i*s+k][j*s+l] > max) ? mat[i*s+k][j*s+l] : max;
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

    for (int i = 0; i < (n-f)/s + 1; i++){
        for (int j = 0; j < (n-f)/s + 1; j++){
            float sum = 0;
            for (int k = 0; k < f; k++){
                for (int l = 0; l < f; l++){
                    sum = sum + mat[i*s+k][j*s+l];
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