// CRecordDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "CRecordDlg.h"
#include "afxdialogex.h"
#include "CMainDlg.h"


// CRecordDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECORD_DIALOG, pParent)
{

}

CRecordDlg::~CRecordDlg()
{
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListRecord);
}


BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListRecord.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListRecord.InsertColumn(0, L"发送者", 0, 50);
	m_ListRecord.InsertColumn(1, L"接收者", 0, 50);
	m_ListRecord.InsertColumn(2, L"信息", 0, 300);
	UpdateList();
	return TRUE;
}

void CRecordDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CRecordDlg::UpdateList()
{
	//删除之前的聊天记录
	m_ListRecord.DeleteAllItems();

	CMainDlg *pParent = (CMainDlg*)GetParent();
	auto &vec = pParent->m_pClient->m_vMsgRecord;
	DWORD dwCount = vec.size();
	CString str;
	for (DWORD i = 0; i < dwCount; ++i)
	{
		//发送者
		str = vec[i].szFrom;
		m_ListRecord.InsertItem(i, str.GetBuffer());
		//接受者
		str = vec[i].szTo;
		m_ListRecord.SetItemText(i, 1, str.GetBuffer());
		//内容
		str = vec[i].szContent;
		m_ListRecord.SetItemText(i, 2, str.GetBuffer());
	}
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
END_MESSAGE_MAP()


// CRecordDlg 消息处理程序
