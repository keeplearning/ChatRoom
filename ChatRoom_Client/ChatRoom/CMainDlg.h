#pragma once
#include "CClientSocket.h"
#include "CRecordDlg.h"
// CMainDlg 对话框


class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(DWORD roomid, CClientSocket *pClient, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CClientSocket * m_pClient;
	CString m_strSend;
	afx_msg void OnClickedSend();
	CListCtrl m_ListName;
	CListCtrl m_ListChat;
	std::map<CString, CDialogEx*>m_map;
	int m_nItem;
	CRecordDlg *m_pRecordDlg;
	DWORD m_roomid;//窗口ID
	void InsertOrDeleteUser(CHATUPDATEUSER &upd);
	void ChatForOne2One(CHATONE2ONE &o2o);
public:
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickMenu(UINT id);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
