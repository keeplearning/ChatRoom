#pragma once
class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();
	//初始化客户端套接字，并尝试链接服务器
	bool ConnectServer(CHAR* szIp, WORD port);
	//负责创建线程用来接收服务器的信息
	TCHAR *Recv();
	bool Send(CHATPURPOSE purpose, CHAR* bufSend, DWORD dwLen, DWORD roomid);
	bool Close();
private:
	//recv function
	TCHAR* RecvForAnonumous();
	TCHAR* RecvForChat();
	TCHAR* RecvForUpdateUserList();
	TCHAR* RecvForOne2One();
	TCHAR* RecvForRegister();
	TCHAR* RecvForLogin();
	TCHAR* RecvAddFriend();
	TCHAR* RecvSearchUser();
	TCHAR* RecvForGetMsgRecord();
	//send function
	void SendForAnonumous(CHAR* bufSend, DWORD dwLen, DWORD roomid);
	void SendForLeave(CHAR* bufSend, DWORD dwLen, DWORD roomid);
	void SendForChat(CHAR* bufSend, DWORD dwLen, DWORD roomid);
	void SendForOne2One(CHAR* bufSend, DWORD dwLen, DWORD roomid);
	void SendForRegister(CHAR* bufSend, DWORD dwLen);
	void SendForLogin(CHAR* bufSend, DWORD dwLen);
	void SendAddFriend(CHAR* bufSend, DWORD dwLen);
	void SendSearchUser(CHAR* bufSend, DWORD dwLen);
	void SendForGetMsgRecord(CHAR* bufSend, DWORD dwLen);
public:
	SOCKET m_socket;
	CHATSEND *m_pObjChatRecv;
	char m_szName[50];
	char m_szToName[50];
	CHATUPDATEUSER *m_pObjUpdate;
	CHATONE2ONE *m_pObjOne2One;
	TCHAR m_bufRecv[BUFMSG];
	HANDLE m_hEvent;
	vector<CHATMSGRECORD>m_vMsgRecord;
};

