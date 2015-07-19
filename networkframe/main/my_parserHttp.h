#ifndef __my_parserHttp__h__
#define __my_parserHttp__h__

#include "macroDef.h"
#include <string>
#include <map>
#include <vector>
#include "packetparser.h"
using namespace std;

#define    CRLF					"\r\n"
#define    CRLFCRLF			"\r\n\r\n"
#define    CONTENTLEN		"Content-Length"

typedef map<string, string, less<string> > HEADERKEYVALUES;

class MyParserHttp : public PacketParser
{
public:
    MyParserHttp();
    virtual ~MyParserHttp(){};
public:
    virtual int parseHeader(const char* buf, size_t buf_len, size_t& headerlen, size_t& bodylen, size_t& packetlen);

};

#endif
