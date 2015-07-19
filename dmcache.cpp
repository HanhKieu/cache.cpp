#include <iostream>
#include <string.h>
#include <bitset>
#include <fstream>
#include <stdlib.h>
#include <sstream>



using namespace std;

int main(int argc, char *argv[]){
	int i, j;
	/*********initialize memory block/cache line********/
	string cache[32][4];            //cache[i][0] = Tag; cache[i][1]= Index; [i][2] = offset; [i][3] = data

	for (i = 0; i < 32; i++){
		for (j = 0; j < 4; j++){
			cache[i][j] = "0";
		}
	}

	/**********get data from test file************/
	ifstream fs;	
	fs.open(argv[1]);
	string line;
	int numberOfLines = 0;



	string tempString;
	
	
	while(getline(fs, line)){

		tempString = line.substr(0, line.find(' ')); //cacheline 16 bits
		stringstream ss;
		ss << hex << tempString;
		unsigned n;
		ss >> n;
		bitset<16> b(n);
		cout << b.to_string() << endl;
		// cout << tempString << endl;

		tempString = (line.substr(line.find(' ') + 1, 2)); //read or write 8 bits
		// cout << tempString << endl;

		tempString = line.substr(line.rfind(' ') + 1, 2) ;// data 8 bits

		// cout << tempString << endl;

	}
		

	// }
    fs.close();

	/*************cache-write-in**************/

	/************return cache file***************/

	return 0;
}