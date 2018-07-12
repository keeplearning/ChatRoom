#include "stdafx.h"
#include "CClientSocket.h"


CClientSocket::CClientSocket():
	m_socket(0),
	m_pObjChatRecv(nullptr),
	m_szName(),
	m_pObjUpdate(nullptr),
	m_bufRecv(),
	m_pObjOne2One(nullptr),
	m_hEvent(0)
{
}


CClientSocket::~CClientSocket()
{
}

bool CClientSocket::ConnectServer(CHAR* szIp, WORD port)
{
	m_hEvent = CreateEvent(
		NULL,  //��ȫ��������NULL��ʾĬ��
		FALSE, //true:�ֹ����ã�false:�Զ�����
		TRUE,  //��ʼ״̬true:����̬  false:�Ǽ���̬
		NULL   //�¼����������
	);
	//��ʼ��
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}
	//�����׽���
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//���÷�������ַ
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(szIp);
	serverAddr.sin_port = htons(port);
	//���ӷ�����
	int nRet = connect(m_socket, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		closesocket(m_socket);
		m_socket = NULL;
		WSACleanup();
		return false;
	}
	return true;
}

TCHAR* CClientSocket::Recv()
{
	//������Ϣ
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_socket, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error");
		return nullptr;
	}
	m_pObjChatRecv = &ct;
	switch (ct.m_type)
	{
	case ANONYMOUS:
		return RecvForAnonumous();
	case CHAT:
		return RecvForChat();
	case ONE2ONE:
		return RecvForOne2One();
	case REGISTER:
		return RecvForRegister();
	case LOGIN:
		return RecvForLogin();
	case SEARCHUSER:
		return RecvSearchUser();
	case FILETRANS:
		break;
	case ADDFRIEND:
		return RecvAddFriend();
	case MSGRECORD:
		return RecvForGetMsgRecord();
	case UPDATEUSER:
		return RecvForUpdateUserList();
	default:
		break;
	}
	return nullptr;
}

bool CClientSocket::Send(CHATPURPOSE purpose, CHAR* bufSend, DWORD dwLen, DWORD roomid)
{
	switch (purpose)
	{
	case ANONYMOUS:
		SendForAnonumous(bufSend, dwLen, roomid);
		break;
	case LEAVEROOM:
		SendForLeave(bufSend, dwLen, roomid);
	case CHAT:
		SendForChat(bufSend, dwLen, roomid);
		break;
	case ONE2ONE:
		SendForOne2One(bufSend, dwLen, roomid);
		break;
	case REGISTER:
		SendForRegister(bufSend, dwLen);
		break;
	case LOGIN:
		SendForLogin(bufSend, dwLen);
		break;
	case ADDFRIEND:
		SendAddFriend(bufSend, dwLen);
		break;
	case SEARCHUSER:
		SendSearchUser(bufSend, dwLen);
		break;
	case FILETRANS:
		break;
	case MSGRECORD:
		SendForGetMsgRecord(bufSend, dwLen);
		break;

	default:
		break;
	}

	return true;
}

//�ر��׽���
bool CClientSocket::Close()
{
	WSACleanup();
	closesocket(m_socket);
	return true;
}

TCHAR* CClientSocket::RecvForAnonumous()
{
	wsprintf(m_bufRecv, _T("%S����������\n"), m_pObjChatRecv->m_content.any.buf);
	return m_bufRecv;
}

TCHAR* CClientSocket::RecvForChat()
{
	//����
	for (int i = 0; i < m_pObjChatRecv->m_content.chat.dwLen; ++i)
	{
		m_pObjChatRecv->m_content.chat.buf[i] ^= 15;
	}
	wsprintf(m_bufRecv, _T("%S"), m_pObjChatRecv->m_content.chat.buf);
	return m_bufRecv;
}

TCHAR* CClientSocket::RecvForUpdateUserList()
{
	m_pObjUpdate = new CHATUPDATEUSER;
	memcpy_s(m_pObjUpdate, sizeof(CHATUPDATEUSER), &m_pObjChatRecv->m_content.upd,
		sizeof(CHATUPDATEUSER));
	return nullptr;
}

TCHAR* CClientSocket::RecvForOne2One()
{
	m_pObjOne2One = new CHATONE2ONE;
	memcpy_s(m_pObjOne2One, sizeof(CHATONE2ONE), &m_pObjChatRecv->m_content.o2o, sizeof(CHATONE2ONE));
	return nullptr;
}

TCHAR* CClientSocket::RecvForRegister()
{
	if (!strcmp(m_pObjChatRecv->m_content.reg.szName, "success"))
	{
		return _T("ע��ɹ���");
	}
	return _T("ע��ʧ�ܣ�");
}

TCHAR* CClientSocket::RecvForLogin()
{
	if (!strcmp(m_pObjChatRecv->m_content.reg.szName, "success"))
	{
		return _T("��½�ɹ���");
	}
	return _T("��½ʧ�ܣ�");
}

TCHAR* CClientSocket::RecvAddFriend()
{
	if (!strcmp(m_pObjChatRecv->m_content.reg.szName, "success"))
	{
		MessageBox(NULL, _T("��ӳɹ���"),_T("��Ӻ���"), MB_OK);
	}
	else
	{
		MessageBox(NULL, _T("���ʧ�ܣ�"), _T("��Ӻ���"), MB_OK);
	}
	return nullptr;
}

TCHAR* CClientSocket::RecvSearchUser()
{
	if (!strcmp(m_pObjChatRecv->m_content.reg.szName, "nobody"))
	{
		MessageBox(NULL, _T("���޴��ˣ�"), _T("���Һ���"), MB_OK);
	}
	else if (!strcmp(m_pObjChatRecv->m_content.reg.szName, "online"))
	{
		MessageBox(NULL, _T("�������ߣ�˫��1v1���죡"), _T("���Һ���"), MB_OK);
	}
	else 
	{
		MessageBox(NULL, _T("���Ѳ�����"), _T("���Һ���"), MB_OK);
	}
	return nullptr;
}

TCHAR* CClientSocket::RecvForGetMsgRecord()
{
	if (!strcmp(m_pObjChatRecv->m_content.rec.szFrom, "~~~end~~~"))
	{
		SetEvent(m_hEvent);
	}
	else
	{
		m_vMsgRecord.push_back(m_pObjChatRecv->m_content.rec);
	}
	return nullptr;
}

void CClientSocket::SendForAnonumous(CHAR* bufSend, DWORD dwLen, DWORD roomid)
{
	CHATSEND ct = { ANONYMOUS };
	strcpy_s(ct.m_content.any.buf, bufSend);
	ct.m_content.any.dwLen = dwLen;
	ct.m_content.any.roomid = roomid;
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForLeave(CHAR* bufSend, DWORD dwLen, DWORD roomid)
{
	CHATSEND ct = { LEAVEROOM };
	strcpy_s(ct.m_content.cl.buf, bufSend);
	ct.m_content.cl.dwLen = dwLen;
	ct.m_content.cl.roomid = roomid;
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForChat(CHAR* bufSend, DWORD dwLen, DWORD roomid)
{
	CHATSEND ct = { CHAT };
	strcpy_s(ct.m_content.chat.buf, m_szName);
	strcat_s(ct.m_content.chat.buf, ":");
	strcat_s(ct.m_content.chat.buf, bufSend);
	ct.m_content.chat.dwLen = strlen(ct.m_content.chat.buf) + 1;
	ct.m_content.chat.roomid = roomid;
	//����
	for (int i = 0; i < ct.m_content.chat.dwLen; ++i)
	{
		ct.m_content.chat.buf[i] ^= 15;
	}
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForOne2One(CHAR* bufSend, DWORD dwLen, DWORD roomid)
{
	CHATSEND ct = { ONE2ONE };
	memcpy_s(ct.m_content.o2o.szName, strlen(m_szName), m_szName, strlen(m_szName));
	memcpy_s(ct.m_content.o2o.szToName,strlen(m_szToName), m_szToName, strlen(m_szToName));
	memcpy_s(ct.m_content.o2o.szContent, strlen(bufSend), bufSend, strlen(bufSend));
	ct.m_content.o2o.roomid = roomid;
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForRegister(CHAR* bufSend, DWORD dwLen)
{
	CHATSEND ct = { REGISTER };
	char *pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.reg.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.reg.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForLogin(CHAR* bufSend, DWORD dwLen)
{
	CHATSEND ct = { LOGIN };
	char *pwd = nullptr;
	strtok_s(bufSend, ":", &pwd);
	memcpy_s(ct.m_content.log.szName, pwd - bufSend, bufSend, pwd - bufSend);
	memcpy_s(ct.m_content.log.szPwd, strlen(pwd), pwd, strlen(pwd));
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendAddFriend(CHAR* bufSend, DWORD dwLen)
{
	CHATSEND ct = { ADDFRIEND };
	char *frd = nullptr;
	strtok_s(bufSend, ":", &frd);
	memcpy_s(ct.m_content.adf.szName, frd - bufSend, bufSend, frd - bufSend);
	memcpy_s(ct.m_content.adf.szFrd, strlen(frd), frd, strlen(frd));
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendSearchUser(CHAR* bufSend, DWORD dwLen)
{
	CHATSEND ct = { SEARCHUSER };
	memcpy_s(ct.m_content.seu.szName, dwLen, bufSend, dwLen);
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}

void CClientSocket::SendForGetMsgRecord(CHAR* bufSend, DWORD dwLen)
{
	CHATSEND ct = { MSGRECORD };
	send(m_socket, (char*)&ct, sizeof(ct), NULL);
}
