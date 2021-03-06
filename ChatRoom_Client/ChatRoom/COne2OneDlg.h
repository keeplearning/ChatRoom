#pragma once


// COne2OneDlg 对话框

class COne2OneDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COne2OneDlg)

public:
	COne2OneDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~COne2OneDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ONE2ONE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListChat;
	CString m_strSend;
	CString m_strTitle;
	afx_msg void OnClickedButton1();
	afx_msg void OnCancel();
};
