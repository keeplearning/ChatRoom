// CSelectDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "CSelectDlg.h"
#include "afxdialogex.h"

// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialogEx)

CSelectDlg::CSelectDlg(CClientSocket *pClient, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_DIALOG, pParent),
	m_pClient(pClient),
	m_pChatRoom1(nullptr),
	m_pChatRoom2(nullptr),
	m_pChatRoom3(nullptr)
{

}

CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(L"选择聊天室");
	//异步IO
	WSAAsyncSelect(m_pClient->m_socket, m_hWnd, WM_MYSOCKET, FD_READ /*| FD_CLOSE*/);
	return TRUE;
}

afx_msg LRESULT CSelectDlg::OnMysocket(WPARAM wParam, LPARAM lParam)
{
	SOCKET s = wParam;
	WORD  wEvent = WSAGETSELECTEVENT(lParam);
	WORD wErrorCode = WSAGETSELECTERROR(lParam);
	if (wErrorCode)
	{
		exit(0);
		MessageBox(L"网络错误");
		CDialogEx::OnClose();
	}
	switch (wEvent)
	{
	case FD_READ:
	{
		TCHAR *szRecv = m_pClient->Recv();
		if (szRecv == nullptr)
		{
			if (m_pClient->m_pObjUpdate)
			{
				CMainDlg *pDlg = GetMainDlg(m_pClient->m_pObjChatRecv->m_content.upd.roomid);
				if (pDlg != nullptr)
				{
					pDlg->InsertOrDeleteUser(*m_pClient->m_pObjUpdate);
				}
				delete m_pClient->m_pObjUpdate;
				m_pClient->m_pObjUpdate = nullptr;
			}
			else if (m_pClient->m_pObjOne2One)
			{
				CMainDlg *pDlg = GetMainDlg(m_pClient->m_pObjChatRecv->m_content.o2o.roomid);
				//1对1聊天返回recv函数处理
				if (pDlg != nullptr)
				{
					pDlg->ChatForOne2One(*m_pClient->m_pObjOne2One);
				}
				delete m_pClient->m_pObjOne2One;
				m_pClient->m_pObjOne2One = nullptr;
			}
			return 0;
		}
		CMainDlg *pDlg = GetMainDlg(m_pClient->m_pObjChatRecv->m_content.any.roomid);
		if (pDlg != nullptr)
		{
			pDlg->m_ListChat.InsertItem(0, szRecv);
		}
		
	}
	break;
	default:
		break;
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CSelectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSelectDlg::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSelectDlg::OnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSelectDlg::OnClickedButton3)
	ON_MESSAGE(WM_MYSOCKET, &CSelectDlg::OnMysocket)
	ON_MESSAGE(WM_MYCLOSE, &CSelectDlg::OnMyclose)
END_MESSAGE_MAP()


// CSelectDlg 消息处理程序

//进入聊天室1
void CSelectDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChatRoom1 == nullptr)
	{
		m_pChatRoom1 = new CMainDlg(MAIN_DLG1, m_pClient);
		m_pChatRoom1->Create(IDD_MAIN_DIALOG, this);
		m_pChatRoom1->ShowWindow(SW_SHOW);
	}else
	{
		m_pChatRoom1->Create(IDD_MAIN_DIALOG, this);
		m_pChatRoom1->ShowWindow(SW_SHOW);
	}
}

//进入聊天室2
void CSelectDlg::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChatRoom2 == nullptr)
	{
		m_pChatRoom2 = new CMainDlg(MAIN_DLG2, m_pClient);
		m_pChatRoom2->Create(IDD_MAIN_DIALOG, this);
		m_pChatRoom2->ShowWindow(SW_SHOW);
	}
}

//进入聊天室3
void CSelectDlg::OnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChatRoom3 == nullptr)
	{
		m_pChatRoom3 = new CMainDlg(MAIN_DLG3, m_pClient);
		m_pChatRoom3->Create(IDD_MAIN_DIALOG, this);
		m_pChatRoom3->ShowWindow(SW_SHOW);
	}
}

void CSelectDlg::OnDeleteRoom1()
{
	delete m_pChatRoom1;
	m_pChatRoom1 = nullptr;
}

void CSelectDlg::OnDeleteRoom2()
{
 	delete m_pChatRoom2;
 	m_pChatRoom2 = nullptr;
}

void CSelectDlg::OnDeleteRoom3()
{
 	delete m_pChatRoom3;
 	m_pChatRoom3 = nullptr;
}

CMainDlg* CSelectDlg::GetMainDlg(DWORD roomid)
{
	if (roomid == MAIN_DLG1)
	{
		return m_pChatRoom1;
	}
	else if (roomid == MAIN_DLG2)
	{
		return m_pChatRoom2;
	}
	else if (roomid == MAIN_DLG3)
	{
		return m_pChatRoom3;
	}

	return nullptr;
}

afx_msg LRESULT CSelectDlg::OnMyclose(WPARAM wParam, LPARAM lParam)
{

	DWORD roomid = (DWORD)wParam;
	//告诉服务端离开了聊天室
	m_pClient->Send(LEAVEROOM, m_pClient->m_szName, strlen(m_pClient->m_szName) + 1, roomid);
	//销毁对应的堆内存 指针赋空
	if (roomid == MAIN_DLG1)
	{
		OnDeleteRoom1();
	}
	else if (roomid == MAIN_DLG2)
	{
		OnDeleteRoom2();
	}
	else if (roomid == MAIN_DLG3)
	{
		OnDeleteRoom3();
	}

	return 0;
}
