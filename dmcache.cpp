#include <iostream>
#include <string>
#include <bitset>
#include <fstream>



int main(int argc, char *argv[]){
	int i, j;
	/*********initialize memory block/cache line********/
	unsigned int cacheDirty[32];
	unsigned int cacheTag[32]; //Each cacheline has one tag
	std::string cacheData[32][8];//stores our Data, 32 rows, 8 columns because 8 bytes for each cacheline
	      //cache[32][0]; stores the tag and cache[32][1] stores if dirty bit or not
	std::string memory[0xFFFF][8]; 	//stores data, it is indexed by the offset we recieve from cacheline



	for (i = 0; i < 32; i++){

			cacheDirty[i] = 0;
			cacheTag[i] = 0;
	}


	for (i = 0; i < 32; i++){
		for (j = 0; j < 8; j++){
			cacheData[i][j] = "";
			memory[i][j] = "";
		}

	}//initialize all data

	std::ifstream fs;
	fs.open(argv[1]);
	unsigned int offset, index, tag, outputDirty = 0, outputHit = 0, dirty = 0;
	std::string data;
	int cacheLine, ops;
	std::string outputData = "";

	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	


	while (fs >> std::hex >> cacheLine >> ops >> data){

		offset = cacheLine & 7;
		index = (cacheLine & 248) >> 3;
		tag = (cacheLine & 65280) >> 8;

		/*************cache-write-in**************/
		dirty = cacheDirty[index]; //check to see if dirty
		if(ops == 255)
		{
			if(cacheTag[index] == tag)
				cacheData[index][offset] = data;
			//if hits, put the data where it belongs in cacheline
			else{
					if(dirty)
					{
						for(int i = 0; i < 8; i++)
						{
							 memory[index][i] =  cacheData[index][i] ;

						}//if miss and dirty, transfer cache to memory. Remember to transfer all 8 bytes

					}

					cacheTag[index] = tag;
					for(int i = 0; i < 8; i++)
					{
							   
						cacheData[index][i] = memory[index][i]; 

					}//if miss and clean, transfer memory to cache, bc cache doesn't contain recent updates

					cacheData[index][offset] = data;

				}

			cacheDirty[index] = 1; //set to dirty
			
				
		}
		else if(ops == 0)
		{
			/*************cache-read-in**************/

			if(cacheTag[index] == tag)
			{
				outputHit = 1;
				outputDirty = cacheDirty[index];
				outputData = cacheData[index][offset];


			}//if hits
			


		std::cout << " banana " << std::endl;
		}

	}

	fs.close();

	/************return cache file***************/
	return 0;
}