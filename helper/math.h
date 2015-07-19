////////////////////////////////////////////////////////////////////////////////////
// 
// @file: math.h
// @author: by Mr.Chen
// @date: 2014/5/9	17:54
// @brief: 数学类函数
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __MATH_H_
#define __MATH_H_

#include "type.h"
#include <iostream>
#include <time.h>
#include <string>
#include <stdlib.h>

using std::string;

namespace mathUtil {

const float PI				 = 3.1415926535897932384626433832795f;
const float PI2				 = 6.283185307179586476925286766559f;
const float PI_HALF		 = 1.5707963267948966192313216916398f;
const float PI_QUATER	 = 0.78539816339744830961566084581988f;

class Rand
{
public:
	Rand()		{ srand((uint)time(NULL)); }
	uint			randUint();
	uint			getRand(int nStart, int nEnd);
	std::string	getRandStr(int length, char* szPrefix, bool letters, bool numbers, bool symbols);
	int				weightedRandSelect(int* weight, int n);
};

} //namespace mathUtil
using namespace mathUtil;


#endif	// __MATH_H_

