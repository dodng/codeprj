#include "math.h"
#include "algorithm.h"
#include "macroDef.h"

namespace mathUtil {

	uint Rand::randUint() 
	{
		uint	inta	= (rand() & 0x00000FFF);
		uint	intb	= ((rand() & 0x00000FFF)<<12);
		uint	intc	= ((rand() & 0x000000FF)<<24);
		uint	result	= inta + intb + intc;
		return result;
	}

	uint Rand::getRand(int nStart,int nEnd)
	{
		return (randUint())%(nEnd-nStart+1)+(nStart);
	}

	std::string Rand::getRandStr(int length, char* szPrefix, bool letters, bool numbers, bool symbols) 
	{
		// the shortest way to do this is to create a string, containing
		// all possible values. Then, simply add a random value from that string
		// to our return value
		std::string allPossible; // this will contain all necessary characters
		std::string str = szPrefix; // the random string

		if (letters == true) 
		{ // if you passed true for letters, we'll add letters to the possibilities
			for (int i = 65; i <= 90; i++) 
			{
				allPossible += static_cast<char>(i);
				allPossible += static_cast<char>(i+32); // add a lower case letter, too!
			}
		} 
		if (numbers == true) 
		{ // if you wanted numbers, we'll add numbers
			for (int i = 48; i <= 57; i++) 
			{
				allPossible += static_cast<char>(i);
			}
		} 
		if (symbols == true) 
		{ // if you want symbols, we'll add symbols (note, their ASCII values are scattered)
			for (int i = 33; i <= 47; i++)
			{
				allPossible += static_cast<char>(i);
			} 
			for (int i = 58; i <= 64; i++) 
			{
				allPossible += static_cast<char>(i);
			} 
			for (int i = 91; i <= 96; i++) 
			{
				allPossible += static_cast<char>(i);
			} 
			for (int i = 123; i <= 126; i++) 
			{
				allPossible += static_cast<char>(i);
			}
		}
		// get the number of characters to use (used for rand())
		int numberOfPossibilities = allPossible.length();

		for (int i = 0; i < length; i++) 
		{
			str += allPossible[rand() % numberOfPossibilities];
		}
		return str;
	}
	struct Weight
	{
		Weight():pos(0),value(0){}
		int pos;
		int value;
	};

	int Rand::weightedRandSelect(int* weight, int n)
	{
		int totalWeight = 0;
		Weight* temp = new Weight[n];
		for (int i=0; i<n; ++i)
		{
			temp[i].pos = i;
			temp[i].value = weight[i];
			totalWeight += weight[i];
		}
		
		//ÈÅÂÒË³Ðò
		for (int i=n-1;i>0;--i)
		{
			int pos = getRand(0,i);
			swap<Weight>(temp[pos],temp[i]);
		}

		int randValue = getRand(1,totalWeight);
		int sum = 0;
		for (int i=0;i<n;++i)
		{
			sum += temp[i].value;
			if (randValue <= sum)
			{
				return temp[i].pos;
			}
		}
		SAFE_DELETE_ARRAY(temp);
		return 0;
	}
	
} //namespace mathUtil


