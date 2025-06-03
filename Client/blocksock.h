// blocksock.h
#include <afxsock.h>		// MFC socket extensions
//#include <afxinet.h>		// MFC WinInet

typedef const struct sockaddr* LPCSOCKADDR;

class CMyBlockSocketException : public CException
{
	DECLARE_DYNAMIC(CMyBlockSocketException)
public:
// ���캯��
	CMyBlockSocketException(char* pchMessage);

public:
	~CMyBlockSocketException() {}
	virtual BOOL GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,
		PUINT pnHelpContext = NULL);
private:
	int m_nError;
	CString m_strMessage;
};


class CSocketAddress : public sockaddr_in
{
public:
	// ���캯��
	CSocketAddress()
	{ sin_family = AF_INET;
	  sin_port = 0;
	  sin_addr.s_addr = 0; } // ȱʡֵ
	CSocketAddress(const SOCKADDR& sa) 
	{ memcpy(this, &sa, sizeof(SOCKADDR)); }
	CSocketAddress(const SOCKADDR_IN& sin)
	{ memcpy(this, &sin, sizeof(SOCKADDR_IN)); }
	CSocketAddress(const ULONG ulAddr, const USHORT ushPort = 0)
	//�������������ֽ�˳��
	{ sin_family = AF_INET;
	  sin_port = htons(ushPort);
	  sin_addr.s_addr = htonl(ulAddr); }
	CSocketAddress(const char* pchIP, const USHORT ushPort = 0)
	//�����ʮ���Ƹ�ʽ��IP��ַ
	{ sin_family = AF_INET;
	  sin_port = htons(ushPort);
	  sin_addr.s_addr = inet_addr(pchIP); } //�����ֽ�˳��
	//�Դ����ʮ���Ƹ�ʽ���ص�ַ
	CString DottedDecimal()
	{ return inet_ntoa(sin_addr); }
	//��ö˿ں͵�ַ
	USHORT Port() const
	{ return ntohs(sin_port); }
	ULONG IPAddr() const
	{ return ntohl(sin_addr.s_addr); }
	//�Բ��������أ������Ч��
	const CSocketAddress& operator=(const SOCKADDR& sa)
	{ memcpy(this, &sa, sizeof(SOCKADDR));
	  return *this; }
	const CSocketAddress& operator=(const SOCKADDR_IN& sin)
	{ memcpy(this, &sin, sizeof(SOCKADDR_IN));
	  return *this; }
	operator SOCKADDR()
	{ return *((LPSOCKADDR) this); }
	operator LPSOCKADDR()
	{ return (LPSOCKADDR) this; }
	operator LPSOCKADDR_IN()
	{ return (LPSOCKADDR_IN) this; }
};

class CMyBlockSocket : public CObject
{
	DECLARE_DYNAMIC(CMyBlockSocket)
public:
	SOCKET m_hSocket;
	CMyBlockSocket() { m_hSocket = NULL; }
	void Cleanup();
	void Create(int nType = SOCK_STREAM);
	void Close();
	void Bind(LPCSOCKADDR psa);
	void Listen();
	void Connect(LPCSOCKADDR psa);
	BOOL Accept(CMyBlockSocket& s, LPSOCKADDR psa);
	int Send(const char* pch, const int nSize, const int nSecs);
	int Write(const char* pch, const int nSize, const int nSecs);
	int Receive(char* pch, const int nSize, const int nSecs);
	int SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, 
		const int nSecs);
	int ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, 
		const int nSecs);
	void GetPeerAddr(LPSOCKADDR psa);
	void GetSockAddr(LPSOCKADDR psa);
	static CSocketAddress GetHostByName(const char* pchName, 
		const USHORT ushPort = 0);
	static const char* GetHostByAddr(LPCSOCKADDR psa);
	operator SOCKET()
		{ return m_hSocket; }
};

class CMyHttpBlockSocket : public CMyBlockSocket
{
public:
	DECLARE_DYNAMIC(CMyHttpBlockSocket)
	enum {nSizeRecv = 1000}; //�����ջ������Ĵ�С(>HTTPͷ��Ϣ�ĳ���)
	CMyHttpBlockSocket();
	~CMyHttpBlockSocket();
	int ReadHttpHeaderLine(char* pch, const int nSize, const int nSecs);
	int ReadHttpResponse(char* pch, const int nSize, const int nSecs);
private:
	char* m_pReadBuf; // ��ȡ������
	int m_nReadBuf; // ��ȡ�������е��ֽ���
};