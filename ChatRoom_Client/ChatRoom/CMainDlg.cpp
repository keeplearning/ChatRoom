// CMainDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "CMainDlg.h"
#include "afxdialogex.h"
#include "COne2OneDlg.h"
#include "CSearchDlg.h"
#include "CSelectDlg.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(DWORD roomid, CClientSocket *pClient, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent),
	m_pClient(pClient)
	, m_strSend(_T("")),
	m_pRecordDlg(nullptr),
	m_roomid(roomid)
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strSend);
	DDV_MaxChars(pDX, m_strSend, 256);
	DDX_Control(pDX, IDC_LIST2, m_ListName);
	DDX_Control(pDX, IDC_LIST3, m_ListChat);
}

BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString title;
	title.Format(_T("聊天室%d"), m_roomid - 10000);
	SetWindowTextW(title);
	ModifyStyleEx(0, WS_EX_APPWINDOW);
	m_ListName.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListName.InsertColumn(0, L"在线列表", 0, 100);
	m_ListChat.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListChat.InsertColumn(0, L"聊天内容", 0, 500);
	//告诉服务端自己的名字
	m_pClient->Send(ANONYMOUS, m_pClient->m_szName, strlen(m_pClient->m_szName) + 1, m_roomid);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnClickedSend)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMainDlg::OnDblclkList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMainDlg::OnRclickList2)
	ON_COMMAND_RANGE(ID_SUBMENU, ID_SUBMENU2, &CMainDlg::OnRClickMenu)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


void CMainDlg::OnClickedSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		return;
	}
	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(CHAT, str.GetBuffer(), str.GetLength() + 1, m_roomid);
	m_ListChat.InsertItem(0, L"你说:" + m_strSend);
	m_strSend.Empty();
	UpdateData(FALSE);
}

void CMainDlg::InsertOrDeleteUser(CHATUPDATEUSER &upd)
{
 	WCHAR wch[50] = { 0 };
 	MultiByteToWideChar(CP_ACP, 0, upd.buf, -1, wch, 50);
	CString strName;
	strName.Format(_T("%s"), wch);
	if (upd.bAdd)
	{
		m_ListName.InsertItem(0, strName);
	}
	else
	{
		int n = m_ListName.GetItemCount();
		for (int i = 0; i < n; ++i)
		{
			CString str = m_ListName.GetItemText(i, 0);
			if (!str.Compare(strName))
			{
				m_ListName.DeleteItem(i);
				m_ListChat.InsertItem(0, strName + "退出聊天室");
				break;
			}
		}
	}
}

void CMainDlg::ChatForOne2One(CHATONE2ONE &o2o)
{
	CString strName(o2o.szName);
	CString strContent(o2o.szContent);
	//如果没有窗口直接创建，有了窗口直接调用窗口指针操作
	COne2OneDlg *pDlg = nullptr;
	if (m_map.find(strName) == m_map.end())
	{
		//创建私聊窗口
		pDlg = new COne2OneDlg;
		pDlg->Create(IDD_ONE2ONE_DIALOG, this);
		pDlg->SetWindowTextW(strName.GetBuffer());
		m_map[strName] = pDlg;
	}
	else
	{
		pDlg = (COne2OneDlg*)m_map[strName];
	}
	pDlg->ShowWindow(SW_SHOW);
	pDlg->m_ListChat.InsertItem(0, strName + L":" + strContent);
}

//鼠标双击事件
void CMainDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (pNMItemActivate->iItem == -1)
	{
		return;
	}

	CString strTitle = m_ListName.GetItemText(pNMItemActivate->iItem, 0);
	COne2OneDlg *pDlg = nullptr;
	if (m_map.find(strTitle) == m_map.end())
	{
		//新建1v1聊天对话框
		pDlg = new COne2OneDlg;
		//把当前窗口指定为私聊窗口的父窗口
		pDlg->Create(IDD_ONE2ONE_DIALOG, this);
		//设置窗口标题为要聊天的目标用户名称
		pDlg->SetWindowTextW(strTitle.GetBuffer());
		m_map[strTitle] = pDlg;
	}
	else
	{
		pDlg = (COne2OneDlg*)m_map[strTitle];
	}
	pDlg->ShowWindow(SW_SHOW);
}

//右键单击事件
void CMainDlg::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CPoint pt;
	GetCursorPos(&pt);
	m_ListName.ScreenToClient(&pt);
	LVHITTESTINFO info;
	info.pt = pt;
	//获取鼠标点击当前行的行数
	m_nItem = m_ListName.HitTest(&info);
	if (m_nItem != -1)
	{
		CMenu menu;
		CPoint point;
		GetCursorPos(&point);
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, ID_SUBMENU, L"添加好友");
		menu.AppendMenu(MF_STRING, ID_SUBMENU1, L"搜索好友");
		menu.AppendMenu(MF_STRING, ID_SUBMENU2, L"查询聊天记录");
		menu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

//单击子菜单事件
void CMainDlg::OnRClickMenu(UINT id)
{
	switch (id)
	{
	//添加好友
	case ID_SUBMENU:
	{
		CString strFriend = m_ListName.GetItemText(m_nItem, 0);
		//当前用户:要添加的用户
		CString strUserFrd(m_pClient->m_szName);
		strUserFrd += L":";
		strUserFrd += strFriend;
		CStringA strSend = CW2A(strUserFrd.GetBuffer(), CP_THREAD_ACP);
		m_pClient->Send(ADDFRIEND, strSend.GetBuffer(), strSend.GetLength() + 1, 0);
	}
	break;
	//搜索好友
	case ID_SUBMENU1:
	{
		CSearchDlg dlgSearch(m_pClient);
		dlgSearch.DoModal();
	}
	break;
	//查询聊天记录
	case ID_SUBMENU2:
	{
		DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
		if (dwRet == WAIT_FAILED || dwRet == WAIT_TIMEOUT) return;
		m_pClient->Send(MSGRECORD, NULL, NULL, 0);
		m_pClient->m_vMsgRecord.clear();
		SetTimer(0x1001, 1000, NULL);
	}
	break;
	default:
		break;
	}
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 0x1001)
	{
		DWORD dwRet = WaitForSingleObject(m_pClient->m_hEvent, 100);
		if (dwRet == WAIT_OBJECT_0)
		{
			KillTimer(nIDEvent);
			SetEvent(m_pClient->m_hEvent);
			//显示新窗口
			if (m_pRecordDlg)
			{
				//更新数据
				m_pRecordDlg->UpdateList();
				//显示窗口
				m_pRecordDlg->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pRecordDlg = new CRecordDlg;
				m_pRecordDlg->Create(IDD_RECORD_DIALOG, this);
				m_pRecordDlg->ShowWindow(SW_SHOW);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMainDlg::OnClose()
{
	CDialogEx::OnClose();
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSelectDlg *pParent = (CSelectDlg*)GetParent();
	pParent->PostMessage(WM_MYCLOSE, (WPARAM)m_roomid, 0);
}
