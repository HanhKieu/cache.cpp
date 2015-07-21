#include <iostream>
#include <string>
#include <bitset>
#include <fstream>



int main(int argc, char *argv[])
{
		std::cout << "banana" << std::endl;

	int i, j, z;
	/*********initialize memory block/cache line********/
	unsigned int cacheDirty[10][6];
	unsigned int cacheTag[10][6]; //Each cacheline has one tag
	unsigned int LRUcounter[10][6];
	std::string cacheData[10][6][4];//stores our Data, 32 rows, 8 columns because 8 bytes for each cacheline
	std::string memory[0xFFFF][6][4];
	unsigned int memoryTag[0xFFFF][6];//stores data, it is seted by the offset we recieve from cacheline

	for (i = 0; i < 10; i++){
		for (j = 0; j < 6; j++){
			cacheTag[i][j] = 0;
			cacheDirty[i][j] = 0;
			
			LRUcounter[i][j] = j;
		}

	}//initialize all data


	for (i = 0; i < 0xFFFF; i++){
		for (j = 0; j < 6; j++){
			memoryTag[i][j] = 0;
			for(z = 0; z < 4; z++)
			{
				memory[i][j][z] = "";
			}
		}

	}//initialize all data

	for (i = 0; i < 10; i++){
		for (j = 0; j < 6; j++){
			for(z = 0; z < 4 ; z++)
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
	//unsigned int outputDirty;
	//ckunsigned int tempHit = 0, tempDirty = 0,outputHit = 0;
	std::string data;
	int cacheLine, ops;
	std::string outputData = "";
	

	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	


	while (fs >> std::hex >> cacheLine >> ops >> data)
	{


		offset = cacheLine & 3;
		set = (cacheLine & 60) >> 2;
		tag = (cacheLine & 65472) >> 6;

		/*************cache-write-in**************/
		
		if(ops == 255)
		{

			for(int i = 0; i < 6; i++)
			{

				dirty = cacheDirty[set][i]; //check to see if dirty

				if(cacheTag[set][i] == tag)
				{
					cacheData[set][i][offset] = data; //place the data in the cache no matter what.

					for( int j = 0; j < 6; j++)
					{
						if(LRUcounter[set][j] < LRUcounter[set][i] )
							LRUcounter[set][j]++;

					}//find all less than original counter , increment by 1
					LRUcounter[set][i] = 0;// then set original counter to 0
					cacheDirty[set][i] = 1;

				}//if hit

				else
				{
					if(dirty)
					{
						for(int j = 0; j < 4; j++)
						{
							 memory[cacheTag[set][i] << 4 | set ][i][j] =  cacheData[set][i][j] ;

						}//if miss
						memoryTag[cacheTag[set][i] << 4 | set][i] = cacheTag[set][i];


					}// if miss and dirty transfer all from cacheData to memory, then transfer cacheTag to memorytag
					else
					{
				
						for(z = 0; z < 6; z++)
						{
							if(LRUcounter[set][z] == 5)
							{	
								for(int y = 0; y < 6; y++)
									LRUcounter[set][y]++;
								//increment everything by one, then set original counter to 0;
								LRUcounter[set][z] = 0;
								for(int j = 0; j < 4; j++)
								{
									cacheData[set][z][j] = memory[cacheTag[set][i] << 4 | set][z][j];
								}//for each byte in this cacheline, replace it with the one from memory
							}
						}//find the LRU cacheline and replace that data for that line


				    }//else if it misses and clean , find the least recently used cacheline, and transfer from memory to that LRU cacheline, 



			}//if hits, put the data where it belongs in cacheline



			cacheDirty[set][i] = 1; //set to dirty	
				
		}//for each line in set


	}//if write
}//while
	// 	else if(ops == 0)
	// 	{
	// 		/*************cache-read-in**************/
	// 		dirty = cacheDirty[set];

	// 		if(cacheTag[set] == tag)
	// 		{
	// 			outputHit = 1;
	// 			outputDirty = cacheDirty[set];
	// 			outputData = cacheData[set][offset];


	// 		}//if hits
	// 		else
	// 		{
	// 			outputHit = 0;
	// 			outputDirty = cacheDirty[set];

	// 			if(dirty)
	// 			{
	// 				for(int i = 0; i < 8; i++)
	// 				{
	// 					 memory[(cacheTag[set] << 3 | set )][i] =  cacheData[set][i] ;

	// 				}//if miss and dirty, transfer cache to memory. Remember to transfer all 8 bytes

	// 			}// if dirty

	// 			cacheTag[set] = tag;
	// 			for(int i = 0; i < 8; i++)
	// 			{
						   
	// 				cacheData[set][i] = memory[(cacheTag[set] << 3 | set )][i]; 

	// 			}//if miss and clean, transfer memory to cache, bc cache doesn't contain recent updates

	// 			cacheDirty[set] = 0;
	// 			outputData = cacheData[set][offset];


	// 		}//if it misses
			


	// 	of << outputData << " " <<  outputHit << " " << outputDirty << std::endl;
	// 	}

	// }
	// of.close();
	fs.close();

	/************return cache file***************/
	return 0;
}