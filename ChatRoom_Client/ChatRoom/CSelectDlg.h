#pragma once
#include "CClientSocket.h"
#include "CMainDlg.h"

// CSelectDlg 对话框

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CClientSocket *pClient, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnClickedButton1();
	afx_msg void OnClickedButton2();
	afx_msg void OnClickedButton3();

	void OnDeleteRoom1();
	void OnDeleteRoom2();
	void OnDeleteRoom3();
	CMainDlg* GetMainDlg(DWORD roomid);
	CClientSocket *m_pClient;
	CMainDlg *m_pChatRoom1;
	CMainDlg *m_pChatRoom2;
	CMainDlg *m_pChatRoom3;
protected:
	afx_msg LRESULT OnMysocket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyclose(WPARAM wParam, LPARAM lParam);
};
