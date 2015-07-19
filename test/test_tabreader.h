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

#pragma pack(push) //保存对齐状态
#pragma pack(8) //设置8字节对齐

//定义表SceneDefine.tab行结构
struct tDBC_FILE_SCENE_DEFINE
{
	int			m_nSceneID;									//场景ID
	char*	m_szSceneName;							//场景名字
	int			m_bIsActive;									//是否激活
	int			m_nServerID;									//运行此场景的LOGIC程序SERVERID
	int			m_nSceneType; 								//场景类型(0表示普通场景，1表示副本)
	int			m_nSceneLevel;								//场景最低级别限制
	int			m_bIsCanRide;								//是否可骑乘
	int			m_bIsHaveShadow;						//是否有阴影
	int			m_nPlayer;										//玩家
	int			m_nMonster;									//怪
	int			m_nDropbox;									//掉落包
	int			m_nBus;										//交通工具
	int			m_nArea;										//事件区域
	int			m_nGrowPoint;								//生长点	
	int			m_nTimer;										//定时器
	int			m_nEffect;										//特效
	int			m_nSpellEffectObject;						//技能特效对象
	int			m_nXSize;										//场景X最大值
	int			m_nZSize;										//场景Z最大值
	char*	m_szSceneFileName;						//场景文件名字
	char*	m_szMapName;								//场景地图名
	int			m_nBrightenParam;						//场景Actor亮度参数
	int			m_nCursorSelectBrightenParam;		//光标选择亮度参数
	char*	m_szBackSoundName;						//场景背景音乐文件名
	char*	m_szBack2DSoundName;					//场景背景音效文件名
	int			m_nScriptID;									//脚本id
	int			m_nFYDistance;								//反隐距离参数
	char*	m_szImageNameOnSceneTrans;		//场景Load背景图		
	int			m_PkModeParent;							//进入此区域将强制切换为对应的PK模式大类：自由模式，和平模式
	char*	m_PKModeChildArray;						//进入此区域将强制切换为对应的PK模式子类 eg：组队模式，帮会模式等
	int			m_scenePKAttribute;						//场景默认pk属性
	char*	m_szImageOnEnterScene;				//进入场景时显示的图片
};

#pragma pack(pop)  //恢复对齐状态

void test_tabreader();
