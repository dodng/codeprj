////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_reflectV2.h
// @author: by Mr.Chen
// @date: 2015/7/15	15:15
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_TEST_TEST_REFLECTV2_H
#define APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_TEST_TEST_REFLECTV2_H

#include "simpleReflect.h"
#include <iostream>
#include <string>

class CTest
{
private:
    int _id;
    char _name[100];

public:
    REFLECT_DECLARE();
    PROPERTY_DECLARE(CTest, int, _id);
    PROPERTY_DECLARE(CTest, char*, _name);
};

// -----------------------------------------------------------------------------------------------------
#include "PropertySet.h"
class GameObject : public PropertySet
{
    int _itest;
    float _ftest;
    std::string _strtest;
    bool _btest;

public:
    GameObject()
    {
        registerProperty("_itest", &_itest);
        registerProperty("_ftest", &_ftest);
        registerProperty("_strtest", &_strtest);
        registerProperty("_btest", &_btest);
    }
};

void test_reflectV2();

#endif // _TEST_REFLECT_V2_H
