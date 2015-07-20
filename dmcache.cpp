#include <iostream>
#include <string>
#include <bitset>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
	int i, j;
	/*********initialize memory block/cache line********/
	unsigned int cacheDirty[32];
	unsigned int cacheTag[32]; //Each cacheline has one tag
	string cacheData[32][8];//stores our Data, 32 rows, 8 columns because 8 bytes for each cacheline
	      //cache[32][0]; stores the tag and cache[32][1] stores if dirty bit or not
	string memory[0xFFFF]; 	//stores data, it is indexed by the offset we recieve from cacheline



	for (i = 0; i < 32; i++){

			cacheDirty[i] = 0;
			cacheTag[i] = 0;
	}


	for (i = 0; i < 32; i++){
		for (j = 0; j < 8; j++){
			cacheData[i] = "";
		}

	}//initialize all data

	ifstream fs;
	fs.open(argv[1]);
	unsigned int offset, index, tag, outputDirty = 0, outputHit = 0;
	string data;
	int cacheLine, ops;
	string outputData;



	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	
	
	
	
	


	while (fs >> hex >> cacheLine >> ops >> data){

		offset = cacheLine & 7;
		index = (cacheLine & 248) >> 3;
		tag = (cacheLine & 65280) >> 8;

		/*************cache-write-in**************/
		if(ops == 255)
		{
				cache[index][0] = tag;
				cache[index][1] = 1; //set it dirty
					
		}
		else if(ops == 0)
		{
			/*************cache-read-in**************/
			
			//checks if hit or miss. If hit then hit = 1, else it misses and hit = 0

			if(cache[index][0] == tag && cache[index][1] == 1){
					hit = 1;
					dirty = 1;
					
				

					



			}//if hit and dirty, then stay dirty
			else if((cache[index][0] == tag) && (cache[index][1] == 0)){
					hit = 1;
					dirty = 0;
			

			}//if hit and clean
			else if(cache[index][0] != tag && cache[index][1] == 1){
					hit = 0;
					dirty = 1;
					cache[index][1] = 0 ; //clean or dirty
					cache[index][0] = tag;

					


				
			}// if miss and dirty
			else if(cache[index][0] != tag && cache[index][1] == 0){
					hit = 0;
					dirty = 0;
					cache[index][1] = 1;
					cache[index][0] = tag;
				
			}//if miss and clean
	

			//if dirty and miss then clean it.
			cout << hex << data;
			cout << " " << hit << " " << dirty << endl;
			//checks if dirty bit or not


		}	
	}

	fs.close();

	/************return cache file***************/
	return 0;
}