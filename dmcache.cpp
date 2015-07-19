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
	string cache[32][2];      //cache[32][0]; stores the tag and cache[32][1] stores if dirty bit or not
	string memory[8]; 	//stores data, it is indexed by the offset we recieve from cacheline

	for (i = 0; i < 32; i++){
		for (j = 0; j < 4; j++){
			cache[i][j] = "0";
		}
	}

	/**********get data from test file/ convert the cache line************/
	ifstream fs;	
	fs.open(argv[1]);
	string line;
	
	string tempString;



	
	
	while(getline(fs, line)){

		tempString = line.substr(0, line.find(' ')); //cacheline 16 bits
		stringstream ss;
		ss << hex << tempString;
		unsigned n;
		ss >> n;
		bitset<16> b(n);
		cout << b.to_string() << endl;

		bitset<16>  offset;
		bitset<16> index;
		const bitset<16> mask = 0x7;
		offset = b & mask;
		// const bitset<16> mask2 = 0xF8;
		// b = b & mask2;
		// b >>= 3;
		// index = b;
		//cout << "myIndex is " << index.to_string() << endl;
		cout << " my offset is " << offset.to_string() << endl;





		tempString = (line.substr(line.find(' ') + 1, 2)); //read or write 8 bits


		tempString = line.substr(line.rfind(' ') + 1, 2) ;// data 8 bits



	}
		

	// }
    fs.close();



	/*************cache-write-in**************/


	/************return cache file***************/

	return 0;
}