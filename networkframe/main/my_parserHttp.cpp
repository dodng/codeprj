#include "my_parserHttp.h"
#include "strUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MyParserHttp::MyParserHttp()
{
}

//ret: 1: 包未收全
//      2:包错误
//		0:正确
int MyParserHttp::parseHeader(const char* buf, size_t buf_len, size_t& headerlen, size_t& bodylen, size_t& packetlen)
{
	if(buf_len <= 0){
	    return 1;
	}

	if(buf==NULL || strlen(buf)<=0){
		return 1;
	}

	const char* pos=strstr(buf,CRLF);
	if(pos==NULL || (pos - buf) > buf_len){
		return 1;
	}

	const char* posend=strstr(pos,CRLFCRLF);
	if(posend==NULL || (posend - buf) > buf_len){
		return 1;
	}

	if(posend - pos <= 2){
		return 1;
	}

	const char* posstart=pos+strlen(CRLF);
	int msgHeaderLen = posend - posstart;
	if(msgHeaderLen <= 0){
		return 2;
	}

	//char* posContentLenth=strstr(pos,CONTENTLEN);
	//m_headerlen = msgHeaderLen;
	headerlen = posend - buf + strlen(CRLFCRLF);
	
	char* pTmpMsgHeader=new char[msgHeaderLen+1];
	memset(pTmpMsgHeader, 0, msgHeaderLen+1);
	memcpy(pTmpMsgHeader, posstart, msgHeaderLen);
	string strMsgHeader = pTmpMsgHeader;

	vector<string> headeritems;
	strUtil::ConvertStringToVector(strMsgHeader.c_str(), headeritems, CRLF);

	HEADERKEYVALUES headerKeyValues;
	if(headeritems.size() > 0){
		for(int i=0; i< headeritems.size(); i++){
			vector<string> keyvalues;
            strUtil::ConvertStringToVector(headeritems[i].c_str(),keyvalues,":");
			if(keyvalues.size() == 2){
				string key=keyvalues[0];
				string values= keyvalues[1];
				headerKeyValues.insert(HEADERKEYVALUES::value_type(key,values));
			}
		}
	}

	delete []pTmpMsgHeader;
	pTmpMsgHeader = NULL;

	bodylen = 0;
	HEADERKEYVALUES::iterator itHeaderItem = headerKeyValues.find(CONTENTLEN);
	if(itHeaderItem != headerKeyValues.end()){
		bodylen=atoi(itHeaderItem->second.c_str());
	}

	if(bodylen < 0 || bodylen > 65536){
	   //包错误
	        printf("MyParserHttp::parseHeader:: bodylen %d invalid, buf:%s",
	                bodylen, buf);
	       return 2;
	}
	packetlen = headerlen+bodylen;
	if(packetlen>buf_len){
		return 1;
	}
	return 0;
}
