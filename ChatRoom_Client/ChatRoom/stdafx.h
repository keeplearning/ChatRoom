
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <map>
#include <vector>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
using namespace std;

#define BUFMSG 2048
#define MAIN_DLG1 10001
#define MAIN_DLG2 10002
#define MAIN_DLG3 10003

#define ID_SUBMENU						2000
#define ID_SUBMENU1						2001
#define ID_SUBMENU2						2002
#define WM_MYSOCKET						WM_USER+1
#define WM_MYCLOSE						WM_USER+2

enum CHATPURPOSE
{
	ANONYMOUS = 1,
	CHAT,
	ONE2ONE,
	REGISTER,
	LOGIN,
	ADDFRIEND,
	SEARCHUSER,
	FILETRANS,
	MSGRECORD,
	UPDATEUSER,
	LEAVEROOM
};

//匿名聊天结构体
typedef struct _CHATANONYMOUS
{
	DWORD dwLen;
	DWORD roomid;
	char buf[50];
}CHATANONYMOUS;

//离开聊天室结构体
typedef struct _CHATLEAVE
{
	DWORD dwLen;
	DWORD roomid;
	char buf[50];
}CHATLEAVE;

//聊天结构体
typedef struct _CHATCHAT
{
	DWORD dwLen;
	DWORD roomid;
	char buf[1024];
}CHATCHAT;

//1V1结构体
typedef struct _CHATONE2ONE
{
	DWORD roomid;
	CHAR szName[50];
	CHAR szToName[50];
	CHAR szContent[1024];
}CHATONE2ONE;

//注册结构体
typedef struct _CHATREGISTER
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATREGISTER;

//登录结构体
typedef struct _CHATLOGIN
{
	CHAR szName[50];
	CHAR szPwd[50];
}CHATLOGIN;

//添加好友结构体
typedef struct _CHATADDFRIEND
{
	CHAR szName[50];
	CHAR szFrd[50];
}CHATADDFRIEND;

//搜索用户结构体
typedef struct _CHATSEARCHUSER
{
	CHAR szName[50];
}CHATSEARCHUSER;

//文件传输结构体
typedef struct _CHATFILETRANS
{
}CHATFILETRANS;

//查看聊天记录结构体
typedef struct _CHATMSGRECORD
{
	CHAR szFrom[50];
	CHAR szTo[50];
	CHAR szContent[1024];
}CHATMSGRECORD;

//更新在线用户结构体
typedef struct _CHATUPDATEUSER
{
	DWORD bAdd;
	DWORD dwLen;
	DWORD roomid;
	CHAR buf[50];
}CHATUPDATEUSER;

//发送内容
typedef struct _CHATSEND
{
	CHATPURPOSE m_type;
	union 
	{
		char buf[BUFMSG];
		CHATANONYMOUS any;
		CHATCHAT chat;
		CHATONE2ONE o2o;
		CHATREGISTER reg;
		CHATLOGIN log;
		CHATADDFRIEND adf;
		CHATSEARCHUSER seu;
		CHATFILETRANS trs;
		CHATMSGRECORD rec;
		CHATUPDATEUSER upd;
		CHATLEAVE cl;
	}m_content;
}CHATSEND;