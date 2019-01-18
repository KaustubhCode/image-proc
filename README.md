# Image Processing Library - Task 1
### Notes

- Kernel is flipped while doing convolution

- It is assumed that input size given will be correct. If not it will either add extra zeros to matrix (if input size is more) or exclude some part of the matrix (if input size is less) 

### How to run - with examples
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
