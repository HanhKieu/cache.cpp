#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <stdlib.h>


using namespace std;

int main(int argc, char *argv[]){
	int i, j;



	/*********initialize memory block/cache line********/
	string data[32][4];            //data[i][0] = Tag; data[i][1]= Index; [i][2] = offset; [i][3] = data

	for (i = 0; i < 32; i++){
		for (j = 0; j < 4; j++){
			data[i][j] = "0";
		}
	}

	/**********get data from test file************/

	ifstream fs;
	
	fs.open(argv[1]);
	string line;
	int numberOfLines = 0;

	while(getline(fs, line)){
		cout << "the line is " << line << endl;
		++numberOfLines;

	}
	
	string info[numberOfLines][3];   //[i][0] = Address; [i][1] = Read/Write; [i][2] = data
	

	for (i = 0; i < 300;i++){
		for (j = 0; j < 3; j++){
			fs >> info[i][j];
		}
	}

    fs.close();

	/*************cache-write-in**************/

	/************return cache file***************/

	return 0;
}