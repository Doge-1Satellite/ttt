// ShellCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>

void EncrypMain(char *Buff, int Size, char *AddTable) //加密/解密服务端文件(Server.dll)
{
	for (int i=0, j=0; i<Size; i++)
	{
		Buff[i] ^= AddTable[j++] % 1753 + 79;
		
		if (i % 5 == 0)
			j = 0;
	}
}

void EncryptPlug(unsigned char *szRec, unsigned long nLen, unsigned long key) //加密插件
{
	unsigned long i;
	unsigned char p;
	
	p = (unsigned char ) key % 1451 + 61;
	for(i = 0; i < nLen; i++)
	{
		*szRec -= p;
		*szRec++ ^= p;
	}
}

void DecryptPlug(unsigned char *szRec, unsigned long nLen, unsigned long key) //解密插件
{
	unsigned long i;
	unsigned char p;
	
	p = (unsigned char ) key % 1451 + 61;
	for(i = 0; i < nLen; i++)
	{
		*szRec ^= p;
		*szRec += p;
		szRec++;
	}
}

BOOL SaveDr32(char *FileName) //生成DriverCode32(DriverCode32.h)
{
	HANDLE hSysFile;
	DWORD dwSysSize;
	LPVOID pSysBuff;
	DWORD BytesRead;
	LPVOID pOutBuff;
	HANDLE hOutFile;
	DWORD BytesWritten;
	
	hSysFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSysFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSysSize  = GetFileSize(hSysFile, 0);
	
	pSysBuff = VirtualAlloc(NULL, dwSysSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pSysBuff == NULL)
	{
		CloseHandle(hSysFile);
		return FALSE;
	}
	if (!ReadFile(hSysFile, pSysBuff, dwSysSize, &BytesRead, NULL))
	{
		VirtualFree(pSysBuff, 0, MEM_RELEASE);
		CloseHandle(hSysFile);
		return FALSE;
	}
	CloseHandle(hSysFile);
	
	pOutBuff = VirtualAlloc(NULL, (dwSysSize*5-1)+((dwSysSize+32-1)/32*2), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pOutBuff == NULL)
	{
		VirtualFree(pSysBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	
	char *pDllChar = (char *)pSysBuff;
	char *pOutChar = (char *)pOutBuff;
	for (DWORD i = 0; i < dwSysSize; i++)
	{
		if (i == dwSysSize - 1)
		{
			sprintf(pOutChar, "0x%0.2X", (unsigned char)*pDllChar++);
			pOutChar += 4;
		}
		else
		{
			sprintf(pOutChar, "0x%0.2X,", (unsigned char)*pDllChar++);
			pOutChar += 5;
		}
		
		if (i % 32 == 31 || i == dwSysSize - 1)
		{
			*pOutChar++ = '\r';
			*pOutChar++ = '\n';
		}
	}
	VirtualFree(pSysBuff, 0, MEM_RELEASE);
	
	char OutData1[] = "/*\r\n DriverCode32 By Anonymity\r\n My QQ ????????\r\n"
		" 直接include此单元，使用 DriverCode32SaveFile(\"xxx.xxx\");即可生成文件\r\n*/\r\n\r\n";
	char OutData2[] = "#ifndef _HEX_DRIVERCODE32_\r\n#define _HEX_DRIVERCODE32_\r\n#include <windows.h>\r\n\r\n";
	char OutData3[64] = {0};
	sprintf(OutData3, "const g_DriverCode32FileSize = %d;\r\n", dwSysSize);
	char OutData4[] = "unsigned char g_DriverCode32FileBuff[] = {\r\n";
	char OutData5[] = "};\r\n\r\n";
	char OutData6[] = "/*\r\nbool DriverCode32SaveFile(char *FileName)\r\n{\r\n\tbool Result = false;"
		"\r\n\tHANDLE hFile;\r\n\tDWORD dwBytesWritten;\r\n"
		"\thFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);\r\n"
		"\tif (hFile == INVALID_HANDLE_VALUE) Result = false;\r\n"
		"\tif (WriteFile(hFile, g_DriverCode32FileBuff, g_DriverCode32FileSize, &dwBytesWritten, NULL)) Result = true;\r\n"
		"\tCloseHandle(hFile);\r\n\treturn Result;\r\n}\r\n*/\r\n\r\n#endif\r\n";
	
	hOutFile = CreateFile("DriverCode32.h", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData1, strlen(OutData1), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData2, strlen(OutData2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData3, strlen(OutData3), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData4, strlen(OutData4), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, pOutBuff, (dwSysSize*5-1)+((dwSysSize+32-1)/32*2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData5, strlen(OutData5), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData6, strlen(OutData6), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	VirtualFree(pOutBuff, 0, MEM_RELEASE);
	CloseHandle(hOutFile);
	return TRUE;
}

BOOL SaveDr64(char *FileName) //生成DriverCode64(DriverCode64.h)
{
	HANDLE hSysFile;
	DWORD dwSysSize;
	LPVOID pSysBuff;
	DWORD BytesRead;
	LPVOID pOutBuff;
	HANDLE hOutFile;
	DWORD BytesWritten;
	
	hSysFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSysFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSysSize  = GetFileSize(hSysFile, 0);
	
	pSysBuff = VirtualAlloc(NULL, dwSysSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pSysBuff == NULL)
	{
		CloseHandle(hSysFile);
		return FALSE;
	}
	if (!ReadFile(hSysFile, pSysBuff, dwSysSize, &BytesRead, NULL))
	{
		VirtualFree(pSysBuff, 0, MEM_RELEASE);
		CloseHandle(hSysFile);
		return FALSE;
	}
	CloseHandle(hSysFile);
	
	pOutBuff = VirtualAlloc(NULL, (dwSysSize*5-1)+((dwSysSize+32-1)/32*2), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pOutBuff == NULL)
	{
		VirtualFree(pSysBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	
	char *pDllChar = (char *)pSysBuff;
	char *pOutChar = (char *)pOutBuff;
	for (DWORD i = 0; i < dwSysSize; i++)
	{
		if (i == dwSysSize - 1)
		{
			sprintf(pOutChar, "0x%0.2X", (unsigned char)*pDllChar++);
			pOutChar += 4;
		}
		else
		{
			sprintf(pOutChar, "0x%0.2X,", (unsigned char)*pDllChar++);
			pOutChar += 5;
		}
		
		if (i % 32 == 31 || i == dwSysSize - 1)
		{
			*pOutChar++ = '\r';
			*pOutChar++ = '\n';
		}
	}
	VirtualFree(pSysBuff, 0, MEM_RELEASE);
	
	char OutData1[] = "/*\r\n DriverCode64 By Anonymity\r\n My QQ ????????\r\n"
		" 直接include此单元，使用 DriverCode64SaveFile(\"xxx.xxx\");即可生成文件\r\n*/\r\n\r\n";
	char OutData2[] = "#ifndef _HEX_DRIVERCODE64_\r\n#define _HEX_DRIVERCODE64_\r\n#include <windows.h>\r\n\r\n";
	char OutData3[64] = {0};
	sprintf(OutData3, "const g_DriverCode64FileSize = %d;\r\n", dwSysSize);
	char OutData4[] = "unsigned char g_DriverCode64FileBuff[] = {\r\n";
	char OutData5[] = "};\r\n\r\n";
	char OutData6[] = "/*\r\nbool DriverCode64SaveFile(char *FileName)\r\n{\r\n\tbool Result = false;"
		"\r\n\tHANDLE hFile;\r\n\tDWORD dwBytesWritten;\r\n"
		"\thFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);\r\n"
		"\tif (hFile == INVALID_HANDLE_VALUE) Result = false;\r\n"
		"\tif (WriteFile(hFile, g_DriverCode64FileBuff, g_DriverCode64FileSize, &dwBytesWritten, NULL)) Result = true;\r\n"
		"\tCloseHandle(hFile);\r\n\treturn Result;\r\n}\r\n*/\r\n\r\n#endif\r\n";
	
	hOutFile = CreateFile("DriverCode64.h", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData1, strlen(OutData1), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData2, strlen(OutData2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData3, strlen(OutData3), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData4, strlen(OutData4), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, pOutBuff, (dwSysSize*5-1)+((dwSysSize+32-1)/32*2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData5, strlen(OutData5), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData6, strlen(OutData6), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	VirtualFree(pOutBuff, 0, MEM_RELEASE);
	CloseHandle(hOutFile);
	return TRUE;
}

BOOL SaveMain(char *FileName, char *AddTable) //生成加密后的文件(Server.dll)
{
	HANDLE hDllFile;
	DWORD dwDllSize;
	LPVOID pDllBuff;
	DWORD BytesRead;
	DWORD BytesWritten;
	
	hDllFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDllFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwDllSize  = GetFileSize(hDllFile, 0);
	
	pDllBuff = VirtualAlloc(NULL, dwDllSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pDllBuff == NULL)
	{
		CloseHandle(hDllFile);
		return FALSE;
	}
	if (!ReadFile(hDllFile, pDllBuff, dwDllSize, &BytesRead, NULL))
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		CloseHandle(hDllFile);
		return FALSE;
	}
	CloseHandle(hDllFile);
	
	EncrypMain((char *)pDllBuff, dwDllSize, AddTable);
	hDllFile = CreateFile(FileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
	if (hDllFile == INVALID_HANDLE_VALUE)
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	if (!WriteFile(hDllFile, pDllBuff, dwDllSize, &BytesWritten, NULL))
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		CloseHandle(hDllFile);
		return FALSE;
	}
	VirtualFree(pDllBuff, 0, MEM_RELEASE);
	CloseHandle(hDllFile);
	return TRUE;
}

BOOL SaveCode(char *FileName) //生成ShellCode(ShellCode.h)
{
	HANDLE hDllFile;
	DWORD dwDllSize;
	LPVOID pDllBuff;
	DWORD BytesRead;
	LPVOID pOutBuff;
	HANDLE hOutFile;
	DWORD BytesWritten;
	
	hDllFile = CreateFile(FileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDllFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwDllSize  = GetFileSize(hDllFile, 0);
	
	pDllBuff = VirtualAlloc(NULL, dwDllSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pDllBuff == NULL)
	{
		CloseHandle(hDllFile);
		return FALSE;
	}
	if (!ReadFile(hDllFile, pDllBuff, dwDllSize, &BytesRead, NULL))
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		CloseHandle(hDllFile);
		return FALSE;
	}
	CloseHandle(hDllFile);
	
	pOutBuff = VirtualAlloc(NULL, (dwDllSize*5-1)+((dwDllSize+32-1)/32*2), MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pOutBuff == NULL)
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	
	char *pDllChar = (char *)pDllBuff;
	char *pOutChar = (char *)pOutBuff;
	for (DWORD i = 0; i < dwDllSize; i++)
	{
		if (i == dwDllSize - 1)
		{
			sprintf(pOutChar, "0x%0.2X", (unsigned char)*pDllChar++);
			pOutChar += 4;
		}
		else
		{
			sprintf(pOutChar, "0x%0.2X,", (unsigned char)*pDllChar++);
			pOutChar += 5;
		}
		
		if (i % 32 == 31 || i == dwDllSize - 1)
		{
			*pOutChar++ = '\r';
			*pOutChar++ = '\n';
		}
	}
	VirtualFree(pDllBuff, 0, MEM_RELEASE);
	
	char OutData1[] = "/*\r\n ShellCode转换器 By Anonymity\r\n My QQ ????????\r\n"
		" 直接include此单元，使用 ShellCodeSaveFile(\"xxx.xxx\");即可生成文件\r\n*/\r\n\r\n";
	char OutData2[] = "#ifndef _HEX_SHELLCODE_\r\n#define _HEX_SHELLCODE_\r\n#include <windows.h>\r\n\r\n";
	char OutData3[64] = {0};
	sprintf(OutData3, "const g_ShellCodeFileSize = %d;\r\n", dwDllSize);
	char OutData4[] = "unsigned char g_ShellCodeFileBuff[] = {\r\n";
	char OutData5[] = "};\r\n\r\n";
	char OutData6[] = "/*\r\nbool ShellCodeSaveFile(char *FileName)\r\n{\r\n\tbool Result = false;"
		"\r\n\tHANDLE hFile;\r\n\tDWORD dwBytesWritten;\r\n"
		"\thFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);\r\n"
		"\tif (hFile == INVALID_HANDLE_VALUE) Result = false;\r\n"
		"\tif (WriteFile(hFile, g_ShellCodeFileBuff, g_ShellCodeFileSize, &dwBytesWritten, NULL)) Result = true;\r\n"
		"\tCloseHandle(hFile);\r\n\treturn Result;\r\n}\r\n*/\r\n\r\n#endif\r\n";
	
	hOutFile = CreateFile("ShellCode.h", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, NULL);
	if (hOutFile == INVALID_HANDLE_VALUE)
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData1, strlen(OutData1), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData2, strlen(OutData2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData3, strlen(OutData3), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData4, strlen(OutData4), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, pOutBuff, (dwDllSize*5-1)+((dwDllSize+32-1)/32*2), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData5, strlen(OutData5), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	if (!WriteFile(hOutFile, OutData6, strlen(OutData6), &BytesWritten, NULL))
	{
		VirtualFree(pOutBuff, 0, MEM_RELEASE);
		CloseHandle(hOutFile);
		return FALSE;
	}
	VirtualFree(pOutBuff, 0, MEM_RELEASE);
	CloseHandle(hOutFile);
	return TRUE;
}

BOOL SavePlug()
{
	HANDLE hDllFile;
	DWORD dwDllSize;
	LPVOID pDllBuff;
	DWORD BytesRead;
	DWORD BytesWritten;
	
	hDllFile = CreateFile("1.dll", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDllFile == INVALID_HANDLE_VALUE)
		return FALSE;
	dwDllSize  = GetFileSize(hDllFile, 0);
	
	pDllBuff = VirtualAlloc(NULL, dwDllSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
	if (pDllBuff == NULL)
	{
		CloseHandle(hDllFile);
		return FALSE;
	}
	if (!ReadFile(hDllFile, pDllBuff, dwDllSize, &BytesRead, NULL))
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		CloseHandle(hDllFile);
		return FALSE;
	}
	CloseHandle(hDllFile);
	
	if (((PIMAGE_DOS_HEADER)pDllBuff)->e_magic == IMAGE_DOS_SIGNATURE)
		EncryptPlug((unsigned char *)pDllBuff, dwDllSize, 1024);
	else
		DecryptPlug((unsigned char *)pDllBuff, dwDllSize, 1024);
	
	hDllFile = CreateFile("2.dll", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, NULL);
	if (hDllFile == INVALID_HANDLE_VALUE)
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		return FALSE;
	}
	if (!WriteFile(hDllFile, pDllBuff, dwDllSize, &BytesWritten, NULL))
	{
		VirtualFree(pDllBuff, 0, MEM_RELEASE);
		CloseHandle(hDllFile);
		return FALSE;
	}
	VirtualFree(pDllBuff, 0, MEM_RELEASE);
	CloseHandle(hDllFile);
	return TRUE;
}

int main(int argc, char* argv[])
{
	unsigned char MyFileTabLe[] = {0xBE, 0x16, 0xCF, 0x52, 0xCD};
	
	if (argc == 3)
	{
		if (argv[1][0] != '-') return -1;
		
		switch (argv[1][1])
		{
		case '3': //输入文件名(QAssist32.sys), 生成"DriverCode32.h"
			return SaveDr32(argv[2]) ? 0 : -1;
		case '6': //输入文件名(QAssist64.sys), 生成"DriverCode64.h"
			return SaveDr64(argv[2]) ? 0 : -1;
		case 'E': //输入未加密文件名(Server.dll), 生成加密后的文件(Server.dll)
			return SaveMain(argv[2], (char *)MyFileTabLe) ? 0 : -1;
		case 'S': //输入已加密文件名(Server.dll), 生成"ShellCode.h"
			return SaveCode(argv[2]) ? 0 : -1;
		default:
			return -1;
		}
	}
	return SavePlug() ? 0 : -1;
}
