/*
|==========================================
|	DBC���ݿ��ļ���
|		(������/�ͻ���ͨ��)
|==========================================
|
|		--------------------
|		|  ���ݿ��ļ���ʽ  |
|		--------------------
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  UINT		DBC File Identity�� always 0XDDBBCC00
|		0X004	  UINT      Number of records in the file 
|		0X008     UINT      Number of 4-BYTE fields per record
|		0X010     UINT      String block size 
|   FieldType
|		0X014     UINT[FieldNum]   
|							  The type fo fields(0-INT, 1-FLOAT, 2-string)
|   FieldBlock
|				  UINT[FieldNum*RecordNum]
|							  DataBlock
|	StringBlock
|				  CHAR[StringSize]
|							  StringBlock
|
*/
////////////////////////////////////////////////////////////////////////////////////
// 
// @file: DBCFile.h
// @author: by Mr.Chen
// @date: 2014/8/15	09:55
// @brief: ���ݿ��ļ� Ref from CYOU.
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef __DBC_H_
#define __DBC_H_

#include <vector>
#ifdef _WIN32
#include <hash_map>
#else
#include <ext/hash_map>
#endif
#include "type.h"

using std::vector;

namespace tabReader 
{
	class DBCFile	
	{
	public:
		//�ļ�ͷ
		struct FILE_HEAD
		{
			uint		m_Identify;				//��ʾ	0XDDBBCC00
			int			m_nFieldsNum;			//����
			int			m_nRecordsNum;		//����
			int			m_nStringBlockSize;	//�ַ�������С
		};

		//�ֶ���������
		enum FIELD_TYPE
		{
			T_INT		= 0,	//����
			T_FLOAT	= 1,	//������
			T_STRING	= 2,	//�ַ���
		};

		//���ݿ��ʽ����
		typedef vector< FIELD_TYPE >	FILEDS_TYPE;

		//���ݶ�
		union FIELD
		{
			float		fValue;
			int			iValue;
			uint16	wValue;
			char		cValue;
			const char*	pString;	// Just for runtime!

			//Construct
			FIELD() {}
			FIELD(int value) { iValue = value; }
			FIELD(float value) { fValue = value; }
			FIELD(const char* value) { pString = value; }
		};
		//������
		typedef vector< FIELD >		DATA_BUF;

	public:
		//���ı��ļ�������һ�����ݿ�
		bool			OpenFromTXT(const char* szFileName);
		//�����ڴ��е��ļ���
		bool			OpenFromMemory(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
	protected:
		//��ȡ�ı���ʽ����
		bool			OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);
		//��ȡ�����Ƹ�ʽ����
		bool			OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd, const char* szFileName=0);

	public:
		//����������(��һ��Ϊ����)
		const FIELD*	Search_Index_EQU(int nValue) const;
		//����λ�ò���
		const FIELD*	Search_Posistion(int nRecordLine, int nColumNum) const;
		//����ĳ�е���ָ��ֵ�ĵ�һ��
		const FIELD*	Search_First_Column_Equ(int nColumnNum, const FIELD& value) const;

	public:
		//ȡ��ID
		uint	GetID(void) const				{ return m_ID; }
		//ȡ������
		int		GetFieldsNum(void) const	    { return m_nFieldsNum; }
		//ȡ�ü�¼������
		int		GetRecordsNum(void) const		{ return m_nRecordsNum; }
		//����������
		void	CreateIndex(int nColum = 0, const char* szFileName=0);

	protected:
#ifdef __SGI_STL_PORT
		typedef std::hash_map< int, FIELD*>	FIELD_HASHMAP;
#else
	#ifdef _WIN32
		typedef stdext::hash_map< int, FIELD*>	FIELD_HASHMAP;
	#else
		typedef __gnu_cxx::hash_map< int,FIELD*>    FIELD_HASHMAP;
							  // hash_compare <INT, less<INT> > ,
							   //allocator< pair<const INT, FIELD*> > >
	#endif
		
#endif
		//���ݿ��ʽ�ļ���
		uint						m_ID;
		//���ݿ��ʽ����
		FILEDS_TYPE			m_theType;
		//����
		int							m_nRecordsNum;
		//����
		int							m_nFieldsNum;
		//������
		DATA_BUF				m_vDataBuf;		//size = m_nRecordsNum*m_nFieldsNum
		//�ַ�����
		char*						m_pStringBuf;
		//�ַ�������С
		int							m_nStringBufSize;
		//������
		FIELD_HASHMAP	m_hashIndex;
		//������
		int							m_nIndexColum;

	public:
		//�Ƚ�����ֵ�Ƿ����
		template < FIELD_TYPE T>	
		static bool			isFieldEqual(const FIELD& a, const FIELD& b);

	public:
		DBCFile(uint id);
		virtual ~DBCFile();
	};

} // namespace DBC

#endif // __DBC_H_
