#pragma once


// CRecordDlg 对话框

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRecordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	CListCtrl m_ListRecord;
	afx_msg void OnCancel();
	void UpdateList();
};
