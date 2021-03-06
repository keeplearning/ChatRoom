// COne2OneDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "COne2OneDlg.h"
#include "afxdialogex.h"
#include "CMainDlg.h"

// COne2OneDlg 对话框

IMPLEMENT_DYNAMIC(COne2OneDlg, CDialogEx)

COne2OneDlg::COne2OneDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ONE2ONE_DIALOG, pParent)
	, m_strSend(_T(""))
{

}

COne2OneDlg::~COne2OneDlg()
{
}

void COne2OneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListChat);
	DDX_Text(pDX, IDC_EDIT1, m_strSend);
	DDV_MaxChars(pDX, m_strSend, 256);
}

BOOL COne2OneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListChat.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_ListChat.InsertColumn(0, L"聊天内容", 0, 500);
	return TRUE;
}

BEGIN_MESSAGE_MAP(COne2OneDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &COne2OneDlg::OnClickedButton1)
END_MESSAGE_MAP()


// COne2OneDlg 消息处理程序


void COne2OneDlg::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strSend.IsEmpty())
	{
		return;
	}
	//将宽字符转化为多字节
	CStringA str = CW2A(m_strSend.GetBuffer(), CP_THREAD_ACP);
	CMainDlg *pParent = (CMainDlg*)GetParent();
	//将要收到消息的人名字保存
	CString strTitle;
	GetWindowTextW(strTitle);
	CStringA str1 = CW2A(strTitle.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(pParent->m_pClient->m_szToName, str1.GetBuffer());

	pParent->m_pClient->Send(ONE2ONE, str.GetBuffer(), str.GetLength() + 1, pParent->m_roomid);
	//自己说的话显示在自己的对话框上
	m_ListChat.InsertItem(0, L"你说:" + m_strSend);
	m_strSend.Empty();
	UpdateData(FALSE);
}

void COne2OneDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

