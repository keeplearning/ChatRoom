// CSearchDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "CSearchDlg.h"
#include "afxdialogex.h"


// CSearchDlg 对话框

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CClientSocket *pClient, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEARCH_DIALOG, pParent)
	, m_strSearch(_T(""))
	, m_pClient(pClient)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strSearch);
	DDV_MaxChars(pDX, m_strSearch, 256);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSearchDlg::OnClickedButton1)
END_MESSAGE_MAP()


// CSearchDlg 消息处理程序


void CSearchDlg::OnClickedButton1()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	if (m_strSearch.IsEmpty())
	{
		return;
	}
	CStringA str = CW2A(m_strSearch.GetBuffer(), CP_THREAD_ACP);
	m_pClient->Send(SEARCHUSER, str.GetBuffer(), str.GetLength() + 1, 0);
}
