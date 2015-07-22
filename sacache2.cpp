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
	int line ; //line is where you found where it hit
	std::string outputData = "";
	int temp;

	

// 	/**********get data from test file/ convert the cachewwwwwwwwwwwwwwwwwwwww line************/
	


	while (fs >> std::hex >> cacheLine >> ops >> data)
	{

		tempHit = 0; 

		offset = cacheLine & 0x3; //keep first two values
		set = (cacheLine & 0x3C) >> 2;
		tag = (cacheLine & 0xFFC0) >> 6;
		//std::cout << offset << " " << set << " "<< tag << std::endl;

	// 	/*************cache-write-in**************/
		line = -1;


		
		if(ops == 255)
		{
			
				
	
		}//if write


	   

		else if(ops == 0)
		{



			for(int i= 0; i < 6; i++)
			{
				if(cacheTag[set][i] == tag)
				{
					line = i;
					tempHit = 1;
				}
				cacheDirty[set][i] = 0; //if it misses we set all dirty bits to 0
			}



			if(tempHit == 0)
			{

				for(int i= 0; i < 6; i++)
				{
					cacheDirty[set][i] = 0; //if it misses we set all dirty bits to 0
				}

				temp = tag;




			}

			std::cout << outputData << " " << outputHit << " " << outputDirty << " fsaf" << std::endl;

			
	  
		}//if read

















}//while

	
 	fs.close();

	/************return cache file***************/
	return 0;
}