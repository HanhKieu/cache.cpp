#include <iostream>
#include <string>
#include <bitset>
#include <fstream>



int main(int argc, char *argv[])
{
		//std::cout << "banana" << std::endl;

	int i, j, z;
	//10 sets 6 lines each.
	/*********initialize memory block/cache line********/
	unsigned int cacheDirty[10][6];
	unsigned int cacheTag[16][6]; //Each cacheline has one tag
	unsigned int LRUcounter[10][6];
	std::string cacheData[16][6][4];//stores our Data, 32 rows, 8 columns because 8 bytes for each cacheline
	std::string memory[16384][6][4];
	unsigned int memoryTag[16384][6];//stores data, it is seted by the offset we recieve from cacheline

	for (i = 0; i < 10; i++){
		for (j = 0; j < 6; j++){
			cacheTag[i][j] = 0;
			cacheDirty[i][j] = 0;
			
			LRUcounter[i][j] = j;
		}

	}//initialize all data


	for (i = 0; i < 16384; i++){
		for (j = 0; j < 6; j++){
			memoryTag[i][j] = 0;
			for(z = 0; z < 4; z++)
			{
				memory[i][j][z] = "";
			}
		}

	}//initialize all data

	for (i = 0; i < 16; i++){
		for (j = 0; j < 6; j++){
			cacheTag[i][j] = 0;
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
	unsigned int outputDirty;
	unsigned int tempHit = 0,outputHit = 0;
	std::string data;
	int cacheLine, ops;
	std::string outputData = "";

	

// 	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	


	while (fs >> std::hex >> cacheLine >> ops >> data)
	{

		tempHit = 0;

		offset = cacheLine & 0x3; //keep first two values
		set = (cacheLine & 0x3C) >> 2;
		tag = (cacheLine & 0xFFC0) >> 6;
		//std::cout << offset << " " << set << " "<< tag << std::endl;

	// 	/*************cache-write-in**************/
		
		if(ops == 255)
		{
			for(int line = 0; line < 6; line++)
			{
				dirty = cacheDirty[set][line]; //check to see if dirty

				if(cacheTag[set][line] == tag)
				{
					cacheData[set][line][offset] = data; //place the data in same tag in the cache no matter what.
					cacheTag[set][line] = tag;

					for( int j = 0; j < 6; j++)
					{
						if(LRUcounter[set][j] < LRUcounter[set][line] )
							LRUcounter[set][j]++;
					}//find all less than original counter , increment by 1
					LRUcounter[set][line] = 0;// then set original counter to 0
					tempHit = 1;
				}//if hit
				
		
				if(tempHit == 0 )
				{
					if(dirty)
					{

							for(int j = 0; j < 4; j++)
							{
								memory[cacheTag[set][line] << 4  | set ][line][j] = cacheData[set][line][j];
								//std::cout << cacheData[set][i][j] << std::endl;
							}
							memoryTag[cacheTag[set][line] << 4  | set][line] = cacheTag[set][line];

					// cacheData[set][i][offset] = data; //put data into LRU cacheData
					// 			cacheTag[set][i] = tag; //put testFile tag into cacheTag

					}// if miss and dirty transfer all from cacheData to memory, then transfer cacheTag to memorytag
					else
					{
						for(int i = 0; i < 6; i++)
						{
							if(LRUcounter[set][i] == 5)
							{	
								

								for(int j = 0; j < 4; j++)
								{
									cacheData[set][i][j] = memory[cacheTag[set][i] << 4  | set << 2][i][j];

								}//bring all memory to cache on that line

								cacheData[set][i][offset] = data; //put data into LRU cacheData
								cacheTag[set][i] = tag; //put testFile tag into cacheTag

								for(int y = 0; y < 6; y++)
									LRUcounter[set][y]++;
								//increment everything by one, then set original counter to 0;
								LRUcounter[set][i] = 0;
								break;

							}
						}//find the LRU cacheline and replace that data for that line


				    }//else if it misses and clean , find the least recently used cacheline, and transfer from memory to that LRU cacheline, 


				}//if misses 

				cacheDirty[set][line] = 1; //set to dirty	
		}//for each line in set
	}//if write


	   

	else if(ops == 0)
	{

		outputHit = 0;
		outputDirty = 0;
		outputData = "";

		for(int line= 0; line < 6; line++)
		{
			dirty = cacheDirty[set][line]; //check to see if dirty

			if(cacheTag[set][line] == tag)
			{	 //place the data in same tag in the cache no matter what.
				
				outputHit = 1;
				outputDirty = cacheDirty[set][line];
				outputData = cacheData[set][line][offset];

				//std::cout << outputData << " " << outputHit << " " << outputDirty << std::endl;
				tempHit = 1;
				
			}//if hit


			if(tempHit == 0)
			{	

				outputHit = 0;//the outputHit will be 0, if its dirty or not
				if(dirty)
				{	

						for(int j = 0; j < 4; j++)
						{
							memory[cacheTag[set][line] << 4  | set ][i][j] = cacheData[set][line][j];
							//std::cout << cacheData[set][i][j] << std::endl;
						}
						memoryTag[cacheTag[set][line] << 4  | set][line] = cacheTag[set][line];
						// cacheTag[set][i] = tag;
						// cacheData[set][i][offset] = data;
						
					//if miss and dirty transfer from cache to memory
					//std::cout << outputData << " " << outputHit << " " << outputDirty << " H" << std::endl;
				}
				else
				{

					for(int i = 0; i < 6; i++)
					{
						if(LRUcounter[set][i] == 5)
						{	
							
							cacheTag[set][i] = tag; //put testFile tag into cacheTag

							for(int j = 0; j < 4; j++)
							{
								cacheData[set][i][j] = memory[cacheTag[set][i] << 4  | set ][i][j];

							}//bring all memory to cache on that line

							outputData = cacheData[set][i][offset];

							for(int y = 0; y < 6; y++)
								LRUcounter[set][y]++;
							//increment everything by one, then set original counter to 0;
							LRUcounter[set][i] = 0;

							outputDirty = cacheDirty[set][i];
							cacheDirty[set][i] = 0;
							break;

						}
						
					}//find the LRU cacheline and replace that data for that line


					//std::cout << outputData << " " << outputHit << " " << outputDirty << " M" << std::endl;

			}//if miss and clean trasnfer from memory to cache keeping in mind LRU

			}// if miss

	    }//for each line in set

			///std::cout << outputData << " " << outputHit << " " << outputDirty << " fsaf" << std::endl;

			
	  
	}//if read

















}//while

	
 	fs.close();

	/************return cache file***************/
	return 0;
}