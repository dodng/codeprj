#include "httpRequest.h"
#include "strUtil.h"

using namespace std;

int httpRequest::parserUri()
{
    if(m_requestStr==NULL || strlen(m_requestStr)==0){
        return -1;
    }

    char* pos=strstr(m_requestStr,CRLF);
    if(pos==NULL){
        return -2;
    }

    int reqLineLen=pos-m_requestStr;
    char tmpReqLine[MAX_REQLINELEN+1]={0};
    strncpy(tmpReqLine, m_requestStr, sizeof(tmpReqLine)-1);

    m_requestLine=tmpReqLine;

    if(strncmp(m_requestLine.c_str(), "HTTP", strlen("HTTP")) == 0){
        m_type = 1;
    }
    else{
        m_type = 0;
    }

    vector<string> elements;
    strUtil::ConvertStringToVector(m_requestLine.c_str(), elements, " ");

    if(elements.size() < 3){
        return -3;
    }

    if(m_type == 0){
        m_method=elements[0];
        m_uri=elements[1];
        m_version=elements[2]; 

        vector<string> uris;
	strUtil::ConvertStringToVector(m_uri.c_str(), uris, "?", 2);
        if(uris.size() > 0){
            m_uriname=uris[0]; 
            if(uris.size()>1){
                m_paramsstr=uris[1];
                vector<string> paramitems;
		strUtil::ConvertStringToVector(m_paramsstr.c_str(), paramitems, "&");
                if(paramitems.size() > 0){
                    for(int i=0; i< paramitems.size(); i++){
                        vector<string> keyvalues;
			strUtil::ConvertStringToVector(paramitems[i].c_str(), keyvalues, "=", 2);
                        if(keyvalues.size() == 2){
                            string key=keyvalues[0];
                            string values= keyvalues[1];
                            m_paramsKeyValues.insert(PARAMSKEYVALUES::value_type(key,values));
                        }
                    }
                }
            }   
        }
        return m_type;
    }
    else{
        m_version=elements[0];
        m_strStatusCode=elements[1];
        m_nStatusCode = atoi(m_strStatusCode.c_str());
        m_statusDesc=elements[2];

        return m_type;
    }
}

int httpRequest::parserHeader()
{
    if(m_requestStr==NULL || strlen(m_requestStr)==0){
        return -1;
    }

    char* pos=strstr(m_requestStr,CRLF);
    if(pos==NULL){
        return -2;
    }


    char* posend=strstr(pos,CRLFCRLF);
    if(pos==NULL){
        return -3;
    }
    
    if(posend - pos <= 2){
        return -4;
    }

    char* posstart=pos+strlen(CRLF);
    int msgHeaderLen = posend - posstart;
    if(msgHeaderLen == 0){
        return 0;
    }

    char* pTmpMsgHeader=(char*)malloc(msgHeaderLen+1);
    memset(pTmpMsgHeader, 0, msgHeaderLen+1);
    memcpy(pTmpMsgHeader, posstart, msgHeaderLen);
    string strMsgHeader = pTmpMsgHeader;

    vector<string> headeritems;
    strUtil::ConvertStringToVector(strMsgHeader.c_str(), headeritems, CRLF);
    if(headeritems.size() > 0){
        for(int i=0; i< headeritems.size(); i++){
            vector<string> keyvalues;
	    strUtil::ConvertStringToVector(headeritems[i].c_str(), keyvalues, ":", 2);
            if(keyvalues.size() == 2){
                string key=keyvalues[0];
                string values= keyvalues[1];
                m_headerKeyValues.insert(HEADERKEYVALUES::value_type(key,values));
            }
        }

    }
 
    free(pTmpMsgHeader);

    return m_headerKeyValues.size();
}

int httpRequest::parserBody()
{    
    if(m_requestStr==NULL || strlen(m_requestStr)==0){
        return -1;
    }

    char* pos=strstr(m_requestStr,CRLFCRLF);
    if(pos==NULL){
        return -2;
    }
    
    unsigned int headerlen=pos-m_requestStr+strlen(CRLFCRLF);
    unsigned int bodylen=m_reqLen-headerlen;
    m_contentLength = bodylen;
    m_content = pos+strlen(CRLFCRLF);

    return 0;
}

