////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_tabreader.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:24
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "type.h"
#include "DBCSystem.h"

#pragma pack(push) //�������״̬
#pragma pack(8) //����8�ֽڶ���

//�����SceneDefine.tab�нṹ
struct tDBC_FILE_SCENE_DEFINE
{
	int			m_nSceneID;									//����ID
	char*	m_szSceneName;							//��������
	int			m_bIsActive;									//�Ƿ񼤻�
	int			m_nServerID;									//���д˳�����LOGIC����SERVERID
	int			m_nSceneType; 								//��������(0��ʾ��ͨ������1��ʾ����)
	int			m_nSceneLevel;								//������ͼ�������
	int			m_bIsCanRide;								//�Ƿ�����
	int			m_bIsHaveShadow;						//�Ƿ�����Ӱ
	int			m_nPlayer;										//���
	int			m_nMonster;									//��
	int			m_nDropbox;									//�����
	int			m_nBus;										//��ͨ����
	int			m_nArea;										//�¼�����
	int			m_nGrowPoint;								//������	
	int			m_nTimer;										//��ʱ��
	int			m_nEffect;										//��Ч
	int			m_nSpellEffectObject;						//������Ч����
	int			m_nXSize;										//����X���ֵ
	int			m_nZSize;										//����Z���ֵ
	char*	m_szSceneFileName;						//�����ļ�����
	char*	m_szMapName;								//������ͼ��
	int			m_nBrightenParam;						//����Actor���Ȳ���
	int			m_nCursorSelectBrightenParam;		//���ѡ�����Ȳ���
	char*	m_szBackSoundName;						//�������������ļ���
	char*	m_szBack2DSoundName;					//����������Ч�ļ���
	int			m_nScriptID;									//�ű�id
	int			m_nFYDistance;								//�����������
	char*	m_szImageNameOnSceneTrans;		//����Load����ͼ		
	int			m_PkModeParent;							//���������ǿ���л�Ϊ��Ӧ��PKģʽ���ࣺ����ģʽ����ƽģʽ
	char*	m_PKModeChildArray;						//���������ǿ���л�Ϊ��Ӧ��PKģʽ���� eg�����ģʽ�����ģʽ��
	int			m_scenePKAttribute;						//����Ĭ��pk����
	char*	m_szImageOnEnterScene;				//���볡��ʱ��ʾ��ͼƬ
};

#pragma pack(pop)  //�ָ�����״̬

void test_tabreader();
