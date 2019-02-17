#ifndef IMGOP_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define IMGOP_H

#include <iostream>
#include <vector>
using namespace std;

typedef vector<float> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Feature;

// Convolution with padding
Matrix conv_pad(Matrix mat, Matrix ker, int n, int m, int p, int s = 1);
// Convolution without padding
Matrix conv(Matrix mat, Matrix ker, int n, int m, int s = 1);
// Convolution with padding using matrix multiplication
Matrix conv_mult_pad(Matrix mat, Matrix ker, int n, int m, int p, int s = 1, int mult = 0);
// Convolution without padding using matrix multiplication
Matrix conv_mult(Matrix mat, Matrix ker, int n, int m, int s = 1, int mult = 0);

Matrix maxPooling(Matrix mat, int n, int f, int s = 1);

Matrix avgPooling(Matrix mat, int n, int f, int s = 1);
// ReLu (matrix)
Matrix relu(Matrix mat, int n);
// tanh (matrix)
Matrix tan_h(Matrix mat, int n);
// Sigmoid (vector)
Array sigmoid(Array mat, int n);
// softmax (vector)
Array softmax(Array mat, int n);
// Display Matrix
void display(Matrix twoD);
// Display Vector
void display_vec(Array twoD);

Matrix conv3d(Feature input, Feature ker, int pad, int stride, int mult=1);

Matrix bias(Matrix input, float b);

Feature relu3d(Feature input);

Feature maxpool3d(Feature input, int kernel, int stride);

#endif
