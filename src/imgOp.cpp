#include <iostream>
#include <vector>
#include <bits/stdc++.h> // for tanh, max and exp functions
#include <pthread.h>
#include "imgOp.h"
namespace openblas{
    #include <cblas.h>
}
namespace mkl{
    #include <mkl.h>
}
using namespace std;

#define MAX_THREAD 8

typedef vector<float> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Feature;

struct thread_mat{
    int tid = 0;
    Matrix A;
    Array B;
    Array C;
};

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

void* thread_multiplication(void* arg) 
{ 
    struct thread_mat* tmat = (struct thread_mat*) arg;
    int k = tmat->tid++;

    int n = (tmat->A).size();
    int m = (tmat->B).size();

    // Each thread computes 1/4th of matrix multiplication 
    for (int i = k * n / MAX_THREAD; i < (k + 1) * n / MAX_THREAD; i++){
        for (int j = 0; j < m; j++){ 
            (tmat->C)[i] += (tmat->A)[i][j] * (tmat->B)[j];
        }
    }

    pthread_exit(NULL);
} 

Matrix mult_openblas(Matrix proc_image, Array proc_ker, int n, int m){
    int A_rows = proc_image.size();
    int A_cols = proc_image[0].size();

    float *A;
    float *B;
    B = new float[A_cols];
    A = new float[A_rows*A_cols];
    float *AB;
    AB = new float[A_rows];
    for (int i=0; i<A_rows; i++){
        for (int j=0; j<A_cols; j++){
            A[i*A_cols+j] = proc_image[i][j];
        }
    }
    for (int i=0; i<A_cols; i++){
        B[i]=proc_ker[i];
    }
    openblas::cblas_sgemv(openblas::CblasRowMajor, openblas::CblasNoTrans, A_rows, A_cols, 1.0, A, A_cols, B, 1, 0, AB, 1);
    
    Matrix ans(n, Array(n));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            ans[i][j]=AB[i*n+j];
        }
    }

    free(A);
    free(B);
    free(AB);
    return ans;
}

Matrix mult_mkl(Matrix proc_image, Array proc_ker, int n, int m){
    int A_rows = proc_image.size();
    int A_cols = proc_image[0].size();

    float *A;
    float *B;
    float *AB;
    A = (float *)mkl::mkl_malloc( A_rows*A_cols*sizeof( float ), 32);
    B = (float *)mkl::mkl_malloc( A_cols*sizeof( float ), 32);
    AB = (float *)mkl::mkl_malloc( A_rows*sizeof( float ), 32);
    if (A == NULL || B == NULL || AB == NULL) {
      printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
      mkl::mkl_free(A);
      mkl::mkl_free(B);
      mkl::mkl_free(AB);
      exit(0);
      //return 1;
    }
    for (int i=0; i<A_rows; i++){
        for (int j=0; j<A_cols; j++){
            A[i*A_cols+j] = proc_image[i][j];
        }
    }
    for (int i=0; i<A_cols; i++){
        B[i]=proc_ker[i];
    }

    mkl::cblas_sgemv(mkl::CblasRowMajor, mkl::CblasNoTrans, A_rows, A_cols, 1.0, A, A_cols, B, 1, 0, AB, 1);
    
    Matrix ans(n, Array(n));
    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            ans[i][j]=AB[i*n+j];
        }
    }

    mkl::mkl_free(A);
    mkl::mkl_free(B);
    mkl::mkl_free(AB);

    return ans;
}

Matrix mult_pthread(Matrix proc_image, Array proc_ker, int n, int m){

    Array result(n*n);

    struct thread_mat p;

    p.A = proc_image;
    p.B = proc_ker;
    p.C = result;

    pthread_t threads[MAX_THREAD];

    // Creating four threads, each evaluating its own part 
    for (int i = 0; i < MAX_THREAD; i++) { 
        pthread_create(&threads[i], NULL, thread_multiplication, (void*)(&p)); 
    } 
  
    // joining and waiting for all threads to complete 
    for (int i = 0; i < MAX_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    Matrix ans(n, Array(n));

    for (int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            ans[i][j] = (p.C)[i*n+j];
        }
    }

    return ans;
}

// Convolution with padding (matrix)
Matrix conv_pad(Matrix mat, Matrix ker, int n, int m, int p, int s /*= 1*/){

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
                    sum = sum + padimage[i*s+k][j*s+l] * ker[k][l];
                }
            }
            ans[i][j] = sum;
        }
    }

    return ans;
}

// Convolution without padding (matrix)
Matrix conv(Matrix mat, Matrix ker, int n, int m, int s /*= 1*/){
    return conv_pad(mat,ker,n,m,0,s);
}

Matrix conv_mult_pad(Matrix mat, Matrix ker, int n, int m, int p, int s /*= 1*/, int mult /*= 0*/){
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
            proc_ker[k*m+l] = ker[k][l];
        }
    }

    switch(mult){
        case 1:
            return mult_openblas(proc_image,proc_ker, newsz, m);
            break;
        case 2:
            return mult_mkl(proc_image,proc_ker, newsz, m);
            break;
        case 3:
            return mult_pthread(proc_image,proc_ker,newsz,m);
            break;
        default:
            return simple_multiplication(proc_image,proc_ker,newsz,m);
    }

    //return mult_openblas(proc_image,proc_ker, newsz, m);
    //return mult_mkl(proc_image,proc_ker, newsz, m);
    //return mult_pthread(proc_image,proc_ker,newsz,m);
    //return simple_multiplication(proc_image,proc_ker,newsz,m);
}   

Matrix conv_mult(Matrix mat, Matrix ker, int n, int m, int s /*= 1*/, int mult /*= 0*/){
    return conv_mult_pad(mat,ker,n,m,0,s);
}

Matrix maxPooling(Matrix mat, int n, int f, int s /*= 1*/){

    int newsz = (n-f)/s+1;
    Matrix ans(newsz,Array(newsz));

    for (int i = 0; i < (n-f)/s + 1; i++){
        for (int j = 0; j < (n-f)/s + 1; j++){
            float max = mat[i*s][j*s];
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

Matrix avgPooling(Matrix mat, int n, int f, int s /*= 1*/){

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

//Extra functions for 3d convolution

Matrix conv3d(Feature input, Feature ker, int pad, int stride, int mult){
    int n = input[0].size();
    int newsz = ((n - ker[0].size() + 2*pad)/stride) + 1;
    Matrix output(newsz,Array(newsz));
    for (int i = 0; i < input.size();i++){
        Matrix out = conv_mult_pad(input[i],ker[i],n,ker[0].size(),pad,stride,mult);
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