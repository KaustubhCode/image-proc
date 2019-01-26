# Image Processing Library

Image processing library with functions (in src/imgOp) like convolution with a kernel, applying average pooling or max pooling to an image and applying relu, tanh, softmax and sigmoid functions on each pixel value of the image. Convolution has been implemented by matrix multiplication also using three methods - Intel MKL, OpenBLAS and using Pthreads. 
*Note: This library assumes square matrix input.*

## Description

Description of each file in '/src':
- `main.cpp` - Takes in user input and calls `imgOp.cpp` for applying operations.
- `imgOp.cpp` - Contains all the required functions for the image processing.
- `matgen.cpp` - Creates 10 matrices of sizes from range 1 to `mat_size` with random integer elements from range -`range` to +`range`. All these matrices are stored in the `./data/matrix` folder
- `evaluator.cpp` - Evaluated all the matrices in the `./data/matrix` for convolution using matrix multiplication using kernels from `./data/ker`. It stores the time in the evaluation inside `./data/mean`. First column is time and second column is matrix size n.
- `main.cpp` -  Calculates mean and standard deviations of times (for each size) and stores them in `./data/mean` and `./data/sd` respectively. First column has mean/standard deviation and second column is matrix size n.

## Getting Started

### Prerequisites

- Standard C++ libraries (g++)
- Boost library (*Note*: It is only required for better user interface/program options, not for processing)
  > sudo apt-get install libboost-all-dev
  > sudo apt-get update
- Intel MKL library (for matrix multiplication)
  - Download library from Intel mkl site.
  - Link it using 
    > . /opt/intel/mkl/bin/mklvars.sh intel64 #for setting env variable for intel mkl
- OpenBLAS library
  > $ sudo apt-get install libopenblas-dev #for installing openBLAS
  
### How to run

- After cloning/downloading the repo, in the main folder,use `make` to compile all the files.
- Use `./bin/main -h` for help on different instructions. Example:-
  > ./bin/main conv_mult_with_pad matrix1.txt 6 matrix2.txt 3 1 pthread
- Use './bin/main [operator name] -h' to get help on the particular operation.
- For generating matrices, use 
  > ./bin/matgen
- For evaluating generated matrices in 'data' folder use:
  > ./bin/evaluate
- For calculating time & standard deviation from './data/time/' use:
  > ./bin/mean

### Notes

- The input files have matrices given in column order with each value in different line. See example files for a sample matrix.
- It is assumed that input size given will be correct. If not it will either add extra zeros to matrix (if input size is more) or exclude some part of the matrix (if input size is less, it will take into account the matrix formed by the first input_size * input_size lines) 
- {} represents optional arguments for that operation. Default stride set to 1.

### Running operations - with examples
1. Convolution with padding 

Command Syntax

```./bin/main conv_with_pad [pad_size] [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] ```

Example Command

> ./bin/main conv_with_pad 1 matrix1.txt 6 matrix2.txt 3

2. Convolution without padding

Command Syntax

```./bin/main conv_without_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] ```

Example Command

> ./bin/main conv_without_pad matrix1.txt 6 matrix2.txt 3

3. Convolution with padding using matrix multiplication

Command Syntax

```./bin/main conv_mult_with_pad [pad_size] [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] ```

Example Command

> ./bin/main conv_mult_with_pad 1 matrix1.txt 6 matrix2.txt 3

4. Convolution without padding using matrix multiplication

Command Syntax

```./bin/main conv_mult_without_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] ```

Example Command

> ./bin/main conv_mult_without_pad matrix1.txt 6 matrix2.txt 3

5. Max Pooling

Command Syntax

```./bin/main max_pool [input_filename_1] [inputsize_1] [box_size] ```

Example Command

> ./bin/main max_pool matrix1.txt 6 2

6. Average Pooling

Command Syntax

```./bin/main avg_pool [input_filename_1] [inputsize_1] [box_size] ```

Example Command

> ./bin/main avg_pool matrix1.txt 6 2

7. Relu activation

Command Syntax

```./bin/main relu [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main relu matrix2.txt 3

8. tanh activation

Command Syntax

```./bin/main tanh [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main tanh matrix2.txt 3

9. Sigmoid on vector

Command Syntax

```./bin/main sigmoid [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main sigmoid vector1.txt 3

10. Softmax on vector

Command Syntax

```./bin/main softmax [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main softmax vector1.txt 3

## Authors

* **Kaustubh Prakash** - 2016MT10647
* **Abhay Saxena** - 2016MT60648

