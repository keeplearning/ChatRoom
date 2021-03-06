#pragma once
#include "CClientSocket.h"

// CSearchDlg 对话框

class CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CClientSocket *pClient, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSearchDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSearch;
	afx_msg void OnClickedButton1();
	CClientSocket * m_pClient;
};
