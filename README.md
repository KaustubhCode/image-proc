# Image Processing Library

Image processing library with functions (in src/imgOp) like convolution with a kernel, applying average pooling or max pooling to an image and applying relu, tanh, softmax and sigmoid functions on each pixel value of the image. Convolution has been implemented by matrix multiplication also using three methods - Intel MKL, OpenBLAS and using pthreads. Then it uses the image processing library to construct a Convolutional Neural Network called **LeNet** [1]. 
*Note: This library assumes square matrix input.*

## Description

Description of each file in `/src`:
- `main.cpp` - Takes in user input and calls `imgOp.cpp` for applying operations.
- `imgOp.cpp` - Contains all the required functions for the image processing.
- `matgen.cpp` - Creates 10 matrices of sizes from range 10 to `mat_size` with random integer elements from range `-range` to `+range` with `step`. All these matrices are stored in the `./data/matrix` folder.
- `evaluator.cpp` - Evaluated all the matrices in the `./data/matrix` for convolution using matrix multiplication using kernels from `./data/ker`. It stores the time in the evaluation inside `./data/mean`. First column is time and second column is matrix size n.
- `mean.cpp` -  Calculates mean and standard deviations of times (for each size) and stores them in `./data/mean_sd` respectively. First column has matrix size n, second is mean & third is standard deviation.
- `preprocess.py` - Processes an image to required format so that it can be taken as input from lenet.cpp. (Change the argument of input from within this file) and stores it in `./data/lenet_data`
- `lenet.cpp` -  Takes preprocessed image from `./data/lenet_data` and applies the convolutional neural network as in this paper [1] to find out top 5 softmax predictions and to predict the digit on the image. 

## Getting Started

### Prerequisites

- Standard C++ libraries (g++)
- Boost library (*Note*: It is only required for better user interface/program options, not for processing)
  ```
  $ sudo apt-get install libboost-all-dev
  $ sudo apt-get update 
  ```
- Intel MKL library (for matrix multiplication)
  - Download library from Intel mkl site. [2]
  - Link it using (Change  `/opt/intel` if intel library not downloaded in `/opt`)
    > . /opt/intel/mkl/bin/mklvars.sh intel64 #for setting env variable for intel mkl
- OpenBLAS library [3]
  > $ sudo apt-get install libopenblas-dev #for installing openBLAS
- Install opencv for preprocessing
  > $ sudo pip install opencv-python
  
### How to run

- After cloning/downloading the repo, in the main folder,use `make` to compile all the files.
- Use `./bin/main -h` for help on different instructions. Example:-
  > $ ./bin/main conv_mult_with_pad matrix1.txt 6 matrix2.txt 3 1 pthread
- Use `./bin/main [operator name] -h` to get help on the particular operation.
- For checking how many thread should be use, type `lscpu` in `bash`. Multiply number of threads/core * cores/socket * socket to get required number of threads. Change `MAX_THREAD` at the top of `imgOp.cpp` to the same.
- For generating matrices, use 
  ``` $ ./bin/matgen ```
- For evaluating generated matrices in 'data' folder use:
  ``` $ ./bin/evaluate ```
- For calculating time & standard deviation from './data/time/' use:
  ``` $ ./bin/mean ```
- For processing input use:
  ``` $ python3 ./src/preprocess.py```
- For applying LeNet use:
  ``` $ ./bin/lenet [file1.txt] [mult] ```

### Notes

- The input files have matrices given in column order with each value in different line. See example files for a sample matrix.
- It is assumed that input size given will be correct. If not it will either add extra zeros to matrix (if input size is more) or exclude some part of the matrix (if input size is less, it will take into account the matrix formed by the first input_size * input_size lines) 
- {} represents optional arguments for that operation. Default stride set to 1.
- Kernel matrices have not been inverted during convolution.
- All data needs to be stored in the respective subfolders of the `/data` folder. 

### Running operations - with examples
1. Convolution with padding 

Command Syntax

```./bin/main conv_with_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] [pad_size] {stride}```

Example Command

> ./bin/main conv_with_pad 1 ./data/matrix/matrix1.txt 6 ./data/matrix/matrix2.txt 3

2. Convolution without padding

Command Syntax

```./bin/main conv_without_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] {stride}```

Example Command

> ./bin/main conv_without_pad ./data/matrix/matrix1.txt 6 ./data/matrix/matrix2.txt 3

3. Convolution with padding using matrix multiplication

Command Syntax

```./bin/main conv_mult_with_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] [pad_size] {stride}```

Example Command

> ./bin/main conv_mult_with_pad 1 ./data/matrix/matrix1.txt 6 ./data/matrix/matrix2.txt 3

4. Convolution without padding using matrix multiplication

Command Syntax

```./bin/main conv_mult_without_pad [input_filename_1] [inputsize_1] [filename_2] [inputsize_2] {stride}```

Example Command

> ./bin/main conv_mult_without_pad ./data/matrix/matrix1.txt 6 ./data/matrix/matrix2.txt 3

5. Max Pooling

Command Syntax

```./bin/main max_pool [input_filename_1] [inputsize_1] [box_size] {stride}```

Example Command

> ./bin/main max_pool ./data/matrix/matrix1.txt 6 2

6. Average Pooling

Command Syntax

```./bin/main avg_pool [input_filename_1] [inputsize_1] [box_size] {stride}```

Example Command

> ./bin/main avg_pool ./data/matrix/matrix1.txt 6 2

7. Relu activation

Command Syntax

```./bin/main relu [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main relu ./data/matrix/matrix2.txt 3

8. tanh activation

Command Syntax

```./bin/main tanh [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main tanh ./data/matrix/matrix2.txt 3

9. Sigmoid on vector

Command Syntax

```./bin/main sigmoid [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main sigmoid ./data/matrix/vector1.txt 3

10. Softmax on vector

Command Syntax

```./bin/main softmax [input_filename_1] [inputsize_1] ```

Example Command

> ./bin/main softmax ./data/matrix/vector1.txt 3

## Examples

- Some examples of matrices, vectors are present in `./data/matrix`
- We have also run convolution with matrix multiplication using 4 types of kernels (in `./data/ker`) and 4 techniques and saved the times (in microsecond) in `./data/time` folder. Their mean and standard values are also in a file named `./data/mean_sd`. The files are appropriately named.
- Time vs size plots of the above matrices are stored in `/data/plot`.  

## Authors

* **Kaustubh Prakash** - 2016MT10647
* **Abhay Saxena** - 2016MT60648

## References

[1] Y. LeCun, L. Bottou, Y. Bengio, and P. Haffner, “Gradient-based learning applied to
document recognition,” in Intelligent Signal Processing, pp. 306–351, IEEE Press, 2001.

[2] Intel, “Intel math kernel library.” Available at https://software.intel.com/en-us/mkl.

[3] OpenBLAS, “Openblas library.” Available at https://www.openblas.net/.
