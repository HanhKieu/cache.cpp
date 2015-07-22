#include <iostream>
#include <string>
#include <bitset>
#include <fstream>










int main(int argc, char *argv[])
{
		//std::cout << "banana" << std::endl;

	
	//10 sets 6 lines each.
	/*********initialize memory block/cache line********/
	unsigned int cacheDirty[10][6];
	unsigned int cacheTag[16][6]; //Each cacheline has one tag
	unsigned int LRUcounter[10][6];
	std::string cacheData[16][6][4];//stores our Data, 32 rows, 8 columns because 8 bytes for each cacheline
	std::string memory[16384][6][4];
	unsigned int memoryTag[16384][6];//stores data, it is seted by the offset we recieve from cacheline

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 6; j++){
			cacheTag[i][j] = 0;
			cacheDirty[i][j] = 0;
			
			LRUcounter[i][j] = j;
		}

	}//initialize all data


	for (int i = 0; i < 16384; i++){
		for (int j = 0; j < 6; j++){
			memoryTag[i][j] = 0;
			for(int z = 0; z < 4; z++)
			{
				memory[i][j][z] = "";
			}
		}

	}//initialize all data

	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 6; j++){
			cacheTag[i][j] = 0;
			for(int z = 0; z < 4 ; z++)
			{
				cacheData[i][j][z] = "";
				
			}	
		}

	}//initialize all data

	std::ifstream fs;
	//std::ofstream of;
	fs.open(argv[1]);
	//of.open("dm-out.txt");
	unsigned int offset, set, tag, dirty = 0;
	unsigned int outputDirty;
	unsigned int tempHit = 0,outputHit = 0;
	std::string data;
	int cacheLine, ops;
	int line ; //line is where you found where it hit
	int hitPos;
	std::string outputData = "";
	int LRUindex;
	int lineSize = 0;


	

// 	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	


	while (fs >> std::hex >> cacheLine >> ops >> data)
	{

		tempHit = 0; 

		offset = cacheLine & 0x3; //keep first two values
		set = (cacheLine & 0x3C) >> 2;
		tag = (cacheLine & 0xFFC0) >> 6;

		for(int i = 0; i < 6; i++)
		{
			if(cacheTag[set][i] == tag)
			{
				hitPos = i;
				tempHit = 1;
			}
		}

		if(ops == 255){
			if(tempHit == 1 ){
				cacheData[set][hitPos][offset] = data;
				for(int i = 0; i < lineSize; i++)
					LRUcounter[set][i]++;
				LRUcounter[set][hitPos] = 0;
		    }//hit. If there is a hit , transfer data and tag to where it hit
		    if(tempHit == 0){
		    	for(int i = 0; i < lineSize; i++){
			    	dirty = cacheDirty[set][i];

			    	if(dirty){
			    		for(int j = 0; j < 4; j++)
							memory[cacheTag[set][i] << 4  | set ][i][j] = cacheData[set][i][j];
						memoryTag[set][i] = cacheTag[set][i];
						
			    	}//if its dirty transfer data from cache to memory. Then write data to that offset and set. Then Initialiate LRU counter 
			    	else{
			    		for(int j = 0; j < 4; j++)
							cacheData[set][i][j] = memory[cacheTag[set][i] << 4  | set ][i][j];
						cacheTag[set][i] = memoryTag[set][i];
			    	}//if clean write from memory to cache

		    	}//for every line in set



		    	for(int i = 0;i < lineSize; i++){
		    		if(LRUcounter[set][i] == 5){
		    			LRUindex = i;
		    		}

		    	}

		    	cacheTag[set][LRUindex] = tag;
				cacheData[set][LRUindex][offset] = data;
				for(int y = 0; y < 6; y++)
					LRUcounter[set][y]++;
				LRUcounter[set][LRUindex] = 0;
		    }//if it misses something is wrong, and all lines that are dirty neeeds to be updated
		}



}//while

	
 	fs.close();

	return 0;
}