#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <chrono> 
#include "imgOp.h"

namespace po = boost::program_options;
using namespace std;
using namespace std::chrono; 

typedef vector<float> Array;
typedef vector<Array> Matrix;

bool debug = true;
string fn_inp_1;
string fn_inp_2;
string op_name;
string mult_type = "simple";
int op_no = -2;
int inp_1_size, inp_2_size;
int padsize;
int box_size;
int stride = 1;
int mult_method = 0; // 0 = simple, 1 = openblas, 2 = mkl
ifstream streamFile;

string operators[10] = {"conv_with_pad",
					"conv_without_pad",
					"conv_mult_with_pad",
					"conv_mult_without_pad",
					"max_pool",
					"avg_pool",
					"relu",
					"tanh",
					"sigmoid",
					"softmax"};

// ARGUMENT PARSER
int parse(int argc, char** argv){
	try { 
    namespace po = boost::program_options; 
    po::options_description desc("Options"); 
    desc.add_options() 
      ("help,h", "Print help messages") 
      ("version,v", "image-proc Library v1.1") 

      ("operator,o", po::value<string>(&op_name), "operation-name") 
      ("input-1,A", po::value<string>(), "File Name of Input 1")
      ("input-size-1,m", po::value<int>(&inp_1_size), "Dimension of Input 1")
      ("rest", po::value<vector<string> >(), "File Name of Input 1");
 
    po::positional_options_description positionalOptions; 
    positionalOptions.add("operator", 1); 
    positionalOptions.add("input-1", 1); 
    positionalOptions.add("input-size-1", 1); 
    positionalOptions.add("rest", -1); 
 
    po::variables_map vm; 

    try 
    { 
      po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(),vm); // throws on error 
      
      if (vm.count("operator")){
        op_name = vm["operator"].as<string>();
        op_no = -1;
        for (int i=0; i<10; i++){
          if (operators[i].compare(op_name) == 0){
            op_no = i;
          }
        }
      }

      if (vm.count("help")){
        switch (op_no){
          case 0:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1 input-file-2 size-2 pad_size" << endl;
            break;
          case 1:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1 input-file-2 size-2" << endl;
            break;
          case 2:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1 input-file-2 size-2 pad_size [mult_type]" << endl;
            break;
          case 3:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1 input-file-2 size-2 [mult_type]" << endl;
            break;
          case 4:
          case 5:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1 filter-size" << endl;
            break;
          case 6:
          case 7:
          case 8:
          case 9:
            cout << "Improper Usage." << endl << endl << "Correct Usage." << endl << "\t./bin/main " << operators[op_no] << " input-file-1 size-1" << endl;
            break;
          default:
            cout << "Displaying Help Message for image-proc library." << endl << endl << "Following operations are valid." << endl;
          for (int i=0; i<10; i++){
            cout << "\t" << operators[i] << endl;
          }
          cout << endl << "For more details on an operation use ./bin/main [operator] -h" << endl;
        }
        exit(0);
      }
      if (vm.count("version")){ 
        cout << "image-proc Library v1.1" << endl;
        exit(0);
      }
 
      po::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    } 
    catch(boost::program_options::required_option& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
    } 
    catch(boost::program_options::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
    } 

    if (vm.count("operator")){
      vector<string> args;
      if (vm.count("rest")){
        args = vm["rest"].as<vector<string> >();
      }
      switch (op_no){
        case -2:
          exit(0);
        case 0:
          if (args.size() == 3){
            fn_inp_2 = args[0];
            try{ inp_2_size = stoi(args[1]); }
            catch(invalid_argument& e){ cout << "Invalid Size 2" << endl; exit(0); }
            try{padsize = stoi(args[2]);}
            catch(invalid_argument& e){cout << "Invalid Padding value" << endl; exit(0); }
          }else{
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        case 1:
          if (args.size() == 2){
            fn_inp_2 = args[0];
            try{ inp_2_size = stoi(args[1]); }
            catch(invalid_argument& e){ cout << "Invalid Size 2" << endl; exit(0); }
          }else{
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        case 2:
          if (args.size() == 3 || args.size() == 4){
            fn_inp_2 = args[0];
            try{ inp_2_size = stoi(args[1]); }
            catch(invalid_argument& e){ cout << "Invalid Size 2" << endl; exit(0); }
            try{ padsize = stoi(args[2]); }
            catch(invalid_argument& e){ cout << "Invalid Padding value" << endl; exit(0); }
            if (args.size() == 4){
              mult_type = args[3];
            }
          }else{
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        case 3:
          if (args.size() == 2 || args.size() == 3){
            fn_inp_2 = args[0];
            try{ inp_2_size = stoi(args[1]); }
            catch(invalid_argument& e){ cout << "Invalid Size 2" << endl; exit(0); }
            if (args.size() == 3){
              mult_type = args[2];
            }
          }else{
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        case 4:
        case 5:
          if (args.size() == 1){
            try{ box_size = stoi(args[0]); }
            catch(invalid_argument& e){ cout << "Invalid Filter Size" << endl; exit(0); }
          }else{
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        case 6:
        case 7:
        case 8:
        case 9:
          if (args.size()>0){
            cout << "Incorrect number of Arguments." << endl;
            exit(0);
          }
          break;
        default:
          cout << "Invalid operation name. Use -h to list valid operators." << endl;
          exit(0);
      }
    }
    if (vm.count("input-1")){
      fn_inp_1 = vm["input-1"].as<string>();
    }
    if (vm.count("input-size-1")) { 
      inp_1_size = vm["input-size-1"].as<int>();
    }
  } 
  catch(std::exception& e) 
  { 
    std::cerr << "Invalid Arguments " << e.what() << ", application will now exit" << std::endl; 
    exit(0);
  }
}

// main() is where program execution begins.
int main(int argc, char** argv)
{
	parse(argc,argv);
	if (mult_type.compare("openblas") == 0){
		mult_method = 1;
	}else if (mult_type.compare("mkl") == 0){
		mult_method = 2;
	}else if (mult_type.compare("simple") == 0){
		mult_method = 0;
	}else{
		cout << "Invalid multiplication type. Using default simple multiplication. Valid types are: simple, mkl, openblas" << endl << endl;
		mult_method = 0;
	}
	if (inp_1_size <= 0){
    	cout << "Invalid usage: Input 1 size should be positive" << endl;
    	exit(0);
	}
	if (op_no <= 3 && op_no >= 0){
		if (inp_2_size <= 0){
    		cout << "Invalid usage: Input 2 size should be positive" << endl;
    		exit(0);
   		}
   		if (inp_2_size > inp_1_size){
    		cout << "Invalid usage: Kernel/Filter size cannot be greater than image size" << endl;
    		exit(0);
   		}
	}

	// Take Input
	// Input 1
	Matrix mat1(inp_1_size, Array(inp_1_size));
	Matrix mat2(inp_2_size, Array(inp_2_size));
	Array vec1(inp_1_size);
	if (op_no >= 0 && op_no <= 7 ){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i = 0; i < inp_1_size; i++){
				for (int j = 0; j < inp_1_size; j++){
					streamFile >> mat1[j][i];
				}
			}
			streamFile.close();
		}else {
		    cout << "Error: Input File 1 Not Found." << endl;
		    exit(0);
		}
		if (op_no <= 3){
			// Input 2
			streamFile.open(fn_inp_2);
			if (streamFile.good()) {
				for (int i = 0; i < inp_2_size; i++){
					for (int j = 0; j < inp_2_size; j++){
						streamFile >> mat2[j][i];
					}
				}
				streamFile.close();
			}else {
			    cout << "Error: Input File 2 Not Found." << endl;
			    exit(0);
			}
		}
	}else if (op_no >= 8 && op_no <= 9){
		streamFile.open(fn_inp_1);
		if (streamFile.good()) {
			for (int i = 0; i < inp_1_size; i++){
				streamFile >> vec1[i];
			}
			streamFile.close();
		}else {
		    cout << "Error: Input File 1 Not Found." << endl;
		    exit(0);
		}
	}

	if (op_no == 0){
		Matrix convImage = conv_pad(mat1,mat2,inp_1_size,inp_2_size,padsize,stride);
		display(convImage);
	}else if (op_no == 1){
		Matrix convImage = conv(mat1,mat2,inp_1_size,inp_2_size,stride);
		display(convImage);
	}else if (op_no == 2){
		auto start = high_resolution_clock::now();
		Matrix convImage = conv_mult_pad(mat1,mat2,inp_1_size,inp_2_size,padsize,stride,mult_method);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Time: "
         << duration.count() << " microseconds" << endl;
		display(convImage);
	}else if (op_no == 3){
		auto start = high_resolution_clock::now();
		Matrix convImage = conv_mult(mat1,mat2,inp_1_size,inp_2_size,stride,mult_method);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Time: "
         << duration.count() << " microseconds" << endl;
		display(convImage);
	}else if (op_no == 4){
		Matrix convImage = maxPooling(mat1, inp_1_size,box_size,stride);
		display(convImage);
	}else if (op_no == 5){
		Matrix convImage = avgPooling(mat1, inp_1_size,box_size,stride);
		display(convImage);
	}else if (op_no == 6){
		Matrix reluimg = relu(mat1,inp_1_size);
		display(reluimg);
	}else if (op_no == 7){
		Matrix tanhimg = tan_h(mat1,inp_1_size);
		display(tanhimg);
	}else if (op_no == 8){
		Array sigvec = sigmoid(vec1, inp_1_size);
		display_vec(sigvec);
	}else if (op_no == 9){
		Array softmax_vec = softmax(vec1, inp_1_size);
		display_vec(softmax_vec);	
	}
	return 0;
}


// EXAMPLE USAGE
/*op_no    op_name
0	./bin/main operation pad_size inp1 inp1_size inp2 inp2_size
1	./bin/main operation inp1 inp1_size inp2 inp2_size
2	./bin/main operation pad_size inp1 inp1_size inp2 inp2_size
3	./bin/main operation inp1 inp1_size inp2 inp2_size
4	./bin/main operation inp1 inp1_size box_size
5	./bin/main operation inp1 inp1_size box_size
6	./bin/main operation inp1 inp1_size
7	./bin/main operation inp1 inp1_size
8	./bin/main operation inp1 inp1_size
9	./bin/main operation inp1 inp1_size

./bin/main conv_with_pad matrix1.txt 6 matrix2.txt 3 1
./bin/main conv_without_pad matrix1.txt 6 matrix2.txt 3
./bin/main conv_mult_with_pad matrix1.txt 6 matrix2.txt 3 1
./bin/main conv_mult_without_pad matrix1.txt 6 matrix2.txt 3
./bin/main max_pool matrix1.txt 6 2
./bin/main avg_pool matrix1.txt 6 2
./bin/main relu matrix2.txt 3
./bin/main tanh matrix2.txt 3
./bin/main sigmoid vector1.txt 3
./bin/main softmax vector1.txt 3
*/