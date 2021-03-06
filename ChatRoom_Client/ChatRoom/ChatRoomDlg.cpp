
// ChatRoomDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ChatRoom.h"
#include "ChatRoomDlg.h"
#include "afxdialogex.h"
#include "CSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChatRoomDlg 对话框



CChatRoomDlg::CChatRoomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOM_DIALOG, pParent)
	, m_szAccount(_T(""))
	, m_szPwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szAccount);
	DDV_MaxChars(pDX, m_szAccount, 256);
	DDX_Text(pDX, IDC_EDIT2, m_szPwd);
	DDV_MaxChars(pDX, m_szPwd, 256);
}

BEGIN_MESSAGE_MAP(CChatRoomDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_BUTTON1, &CChatRoomDlg::OnClickedLogin)
	ON_COMMAND(IDC_BUTTON2, &CChatRoomDlg::OnClickedRegister)
	ON_COMMAND(IDC_BUTTON3, &CChatRoomDlg::OnClickedNoname)
END_MESSAGE_MAP()


// CChatRoomDlg 消息处理程序

BOOL CChatRoomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChatRoomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChatRoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CChatRoomDlg::OnClickedRegister()
{
	//注册
	UpdateData(TRUE);
	if (m_szAccount.IsEmpty() || m_szPwd.IsEmpty())
	{
		MessageBox(L"账号/密码不能为空！");
		return;
	}

	//链接服务器
	if (!m_client.ConnectServer("127.0.0.1",1234))
	{
		MessageBox(L"链接服务器失败", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}
	
	CString strSend = m_szAccount;
	strSend += L":" + m_szPwd;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(REGISTER, str.GetBuffer(), str.GetLength() + 1, 0);
	
	TCHAR *szCh = m_client.Recv();
	MessageBox(szCh);
}

void CChatRoomDlg::OnClickedLogin()
{
	//登陆
	UpdateData(TRUE);
	if (m_szAccount.IsEmpty() || m_szPwd.IsEmpty())
	{
		MessageBox(L"账号/密码不能为空！");
		return;
	}

	//链接服务器
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"链接服务器失败", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}
	//设置当前用户名
	CStringA strName = CW2A(m_szAccount.GetBuffer(), CP_THREAD_ACP);
	strcpy_s(m_client.m_szName, strName.GetBuffer());

	CString strSend = m_szAccount;
	strSend += L":" + m_szPwd;
	CStringA str = CW2A(strSend.GetBuffer(), CP_THREAD_ACP);
	m_client.Send(LOGIN, str.GetBuffer(), str.GetLength() + 1, 0);

	TCHAR *szCh = m_client.Recv();
	if (wcscmp(szCh, L"登陆成功！") == 0)
	{
		ShowWindow(SW_HIDE);
		//显示聊天窗口
		CSelectDlg obj(&m_client);
		obj.DoModal();
		//隐藏登陆窗口并关闭
		CChatRoomDlg::OnClose();
	}
	else
	{
		MessageBox(szCh);
	}
}

void CChatRoomDlg::OnClickedNoname()
{
	//匿名聊天
	//链接服务器
	if (!m_client.ConnectServer("127.0.0.1", 1234))
	{
		MessageBox(L"链接服务器失败", L"Error", MB_OK | MB_ICONWARNING);
		return;
	}
	ShowWindow(SW_HIDE);
	//显示聊天窗口
	CSelectDlg obj(&m_client);
	obj.DoModal();
	//隐藏登陆窗口并关闭
	CChatRoomDlg::OnClose();
}
