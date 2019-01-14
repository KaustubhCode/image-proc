#ifndef IMGOP_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define IMGOP_H

#include <iostream>

// Convolution with padding (matrix)
float** conv_pad(void *mat, void *ker, int n, int m, int p, int s = 1);
// Convolution without padding (matrix)
float** conv(void *mat, void *ker, int n, int m, int s = 1);

float** maxPooling(void *mat, int n, int f, int s = 1);

float** avgPooling(void *mat, int n, int f, int s = 1);
// ReLu (matrix)
float** relu(void *mat, int n);
// tanh (matrix)
float** tan_h(void *mat, int n);
// Sigmoid (vector)
float* sigmoid(void *mat, int n);
// softmax (vector)
float* softmax(void *mat, int n);
// Display Matrix
void display(float** twoD,int n);
// Display Vector
void display_vec(float* twoD,int n);

#endif
