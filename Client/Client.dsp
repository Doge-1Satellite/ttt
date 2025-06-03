# Microsoft Developer Studio Project File - Name="Client" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Client - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Client.mak" CFG="Client - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Client - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Client - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Client - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /GX /Zi /Od /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x401 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 SHLWAPI.LIB nafxcw.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/Emin.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Client - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Client___Win32_Debug"
# PROP BASE Intermediate_Dir "Client___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /w /W0 /Gm /GX /ZI /Od /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX"stdafx.h" /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../common/libxvidcore.lib SHLWAPI.LIB nafxcw.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/Client.exe"
# ADD LINK32 SHLWAPI.LIB nafxcwd.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../Bin/Client.exe" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Client - Win32 Release"
# Name "Client - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\common\Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BmpToAvi.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\include\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Build.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangeGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckPass.cpp
# End Source File
# Begin Source File

SOURCE=..\common\cJSON.c
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.rc
# End Source File
# Begin Source File

SOURCE=.\ClientDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientView.cpp
# End Source File
# Begin Source File

SOURCE=.\include\CpuUsage.cpp
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\DllDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DropEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\HtmlCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Icons.cpp
# End Source File
# Begin Source File

SOURCE=.\IniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\include\IOCPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\LockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MD5.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryModule.c
# End Source File
# Begin Source File

SOURCE=.\MsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyToolsKit.cpp
# End Source File
# Begin Source File

SOURCE=.\Play.cpp
# End Source File
# Begin Source File

SOURCE=.\ProxyMapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\QQInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SBDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SerAttribute.cpp
# End Source File
# Begin Source File

SOURCE=.\SerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Setting.cpp
# End Source File
# Begin Source File

SOURCE=.\SeuQQwry.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Signature.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashScreenEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Start.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TabView.cpp
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\common\xvidsdk\XvidDec.cpp
# End Source File
# Begin Source File

SOURCE=.\ZXPortMap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AudioDlg.h
# End Source File
# Begin Source File

SOURCE=.\BmpToAvi.h
# End Source File
# Begin Source File

SOURCE=.\BrowseForFolder.h
# End Source File
# Begin Source File

SOURCE=.\control\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\include\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\Build.h
# End Source File
# Begin Source File

SOURCE=.\ChangeGroup.h
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\CheckPass.h
# End Source File
# Begin Source File

SOURCE=..\common\cJSON.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\ClientDoc.h
# End Source File
# Begin Source File

SOURCE=.\ClientView.h
# End Source File
# Begin Source File

SOURCE=.\decode.h
# End Source File
# Begin Source File

SOURCE=.\Dib.h
# End Source File
# Begin Source File

SOURCE=.\DllDlg.h
# End Source File
# Begin Source File

SOURCE=.\DropEdit.h
# End Source File
# Begin Source File

SOURCE=.\EasySize.h
# End Source File
# Begin Source File

SOURCE=.\FileManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileTransferModeDlg.h
# End Source File
# Begin Source File

SOURCE=.\HtmlCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Icons.h
# End Source File
# Begin Source File

SOURCE=.\IniFile.h
# End Source File
# Begin Source File

SOURCE=.\include\IOCPServer.h
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.h
# End Source File
# Begin Source File

SOURCE=.\LFileName.h
# End Source File
# Begin Source File

SOURCE=.\LockDlg.h
# End Source File
# Begin Source File

SOURCE=.\LogView.h
# End Source File
# Begin Source File

SOURCE=..\common\macros.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MapDlg.h
# End Source File
# Begin Source File

SOURCE=.\MD5.h
# End Source File
# Begin Source File

SOURCE=.\MsgBox.h
# End Source File
# Begin Source File

SOURCE=.\MyToolsKit.h
# End Source File
# Begin Source File

SOURCE=.\Play.h
# End Source File
# Begin Source File

SOURCE=.\ProxyMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\QQInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SBDestination.h
# End Source File
# Begin Source File

SOURCE=.\ScreenSpyDlg.h
# End Source File
# Begin Source File

SOURCE=.\SerAttribute.h
# End Source File
# Begin Source File

SOURCE=.\SerManager.h
# End Source File
# Begin Source File

SOURCE=.\Setting.h
# End Source File
# Begin Source File

SOURCE=.\SeuQQwry.h
# End Source File
# Begin Source File

SOURCE=.\ShellDlg.h
# End Source File
# Begin Source File

SOURCE=.\Signature.h
# End Source File
# Begin Source File

SOURCE=.\Start.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysInfo.h
# End Source File
# Begin Source File

SOURCE=.\SystemDlg.h
# End Source File
# Begin Source File

SOURCE=.\TabView.h
# End Source File
# Begin Source File

SOURCE=.\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\UpdateDlg.h
# End Source File
# Begin Source File

SOURCE=.\WebCamDlg.h
# End Source File
# Begin Source File

SOURCE=..\common\xvidsdk\XvidDec.h
# End Source File
# Begin Source File

SOURCE=.\ZXPortMap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ImagIoc\001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\003.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\004.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\004.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\008.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\009.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\010.ico
# End Source File
# Begin Source File

SOURCE=".\res\icos\1-IP.ico"
# End Source File
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1.cur
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\1.ico
# End Source File
# Begin Source File

SOURCE=".\res\icos\10-Double.ico"
# End Source File
# Begin Source File

SOURCE=.\res\1001.ico
# End Source File
# Begin Source File

SOURCE=.\res\102.ico
# End Source File
# Begin Source File

SOURCE=.\res\11.bmp
# End Source File
# Begin Source File

SOURCE=..\Dll\12.ico
# End Source File
# Begin Source File

SOURCE=.\res\1234.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\128.ico
# End Source File
# Begin Source File

SOURCE=..\Dll\13.ico
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171200_bntlh.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171201_ci8cf.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171207_ea1yz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171213_gejmj.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171213_sw53g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171217_gytol.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171217_ne601.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171219_hn4rg.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171219_rf4oo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171219_vpdrc.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171220_hitf2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171222_vk3u9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171225_bzfny.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171230_6m8os.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171232_ru3kr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171233_ikeo1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171237_voiwi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171240_xowel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171240_z1rac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171241_7addj.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171241_fzbqi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1_20101215171241_mrm3k.bmp
# End Source File
# Begin Source File

SOURCE=".\res\icos\2-os.ico"
# End Source File
# Begin Source File

SOURCE=.\res\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newbmp\2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\2.cur
# End Source File
# Begin Source File

SOURCE=.\res\202.ico
# End Source File
# Begin Source File

SOURCE=.\res\21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\231.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\243.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\244.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\245.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\251.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\251.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\255.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\257.ico
# End Source File
# Begin Source File

SOURCE=".\res\icos\3-CPU.ico"
# End Source File
# Begin Source File

SOURCE=.\res\3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\3.cur
# End Source File
# Begin Source File

SOURCE=.\res\334.ico
# End Source File
# Begin Source File

SOURCE=".\res\icos\4-Memory.ico"
# End Source File
# Begin Source File

SOURCE=.\res\4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\4.cur
# End Source File
# Begin Source File

SOURCE=".\res\icos\5-disk.ico"
# End Source File
# Begin Source File

SOURCE=.\res\5.bmp
# End Source File
# Begin Source File

SOURCE=".\res\icos\6-UserName.ico"
# End Source File
# Begin Source File

SOURCE=.\res\6.bmp
# End Source File
# Begin Source File

SOURCE=".\res\icos\7-Active.ico"
# End Source File
# Begin Source File

SOURCE=".\res\icos\8-Anti.ico"
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\add.ico
# End Source File
# Begin Source File

SOURCE=.\res\AppWindows.ico
# End Source File
# Begin Source File

SOURCE=..\Dll\AppWindows.ico
# End Source File
# Begin Source File

SOURCE=.\res\audio.ico
# End Source File
# Begin Source File

SOURCE=.\res\bar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\barnew.bmp
# End Source File
# Begin Source File

SOURCE=.\res\barnew2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\be_caref.ico
# End Source File
# Begin Source File

SOURCE=.\res\Becareful.ico
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\bitbug_favicon (1).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (1).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (10).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (11).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (13).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (15).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (2).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (3).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (4).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (5).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (6).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (7).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (8).ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\bitbug_favicon (9).ico"
# End Source File
# Begin Source File

SOURCE=.\res\bitbug_favicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\bitbug_favicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\bitbug_favicon.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bitmap_3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bitmap_4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bitmap_5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp194.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconew\Build.ico
# End Source File
# Begin Source File

SOURCE=.\res\Cam.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\cam.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\chat.ico
# End Source File
# Begin Source File

SOURCE=.\res\Client.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\Client.ico
# End Source File
# Begin Source File

SOURCE=.\res\Client.rc2
# End Source File
# Begin Source File

SOURCE=.\res\cmdshell.ico
# End Source File
# Begin Source File

SOURCE=.\res\DHLICO.ico
# End Source File
# Begin Source File

SOURCE=.\res\dot.cur
# End Source File
# Begin Source File

SOURCE=.\res\dword.ico
# End Source File
# Begin Source File

SOURCE=.\res\event.ico
# End Source File
# Begin Source File

SOURCE=..\Dll\event.ico
# End Source File
# Begin Source File

SOURCE=.\res\EventError.ico
# End Source File
# Begin Source File

SOURCE=.\res\explorer.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\father_i.ico
# End Source File
# Begin Source File

SOURCE=".\res\iconew\favicon-20150820095732270.ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\favicon-20150820095732430.ico"
# End Source File
# Begin Source File

SOURCE=".\res\iconew\favicon-20150820095732916.ico"
# End Source File
# Begin Source File

SOURCE=.\res\app\FavoritesUrl.ico
# End Source File
# Begin Source File

SOURCE=.\res\File.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\FILE.ico
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarDisable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FileToolBarHot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\flie_a.ico
# End Source File
# Begin Source File

SOURCE=.\res\gh0st.ico
# End Source File
# Begin Source File

SOURCE=.\res\harrow.cur
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\ico9.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\icon209.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\icon209.ico
# End Source File
# Begin Source File

SOURCE=..\Dll\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_A.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_C.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_D.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_E.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_F.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\Icon_G.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\icon_qq.ico
# End Source File
# Begin Source File

SOURCE=.\res\ID_ICON_MAIN.ico
# End Source File
# Begin Source File

SOURCE=.\res\IDBToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\IDI_ICON1.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\idr_1.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\IDR_MAINFRAME.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImagIoc\IDR_MAINFRAME.ico
# End Source File
# Begin Source File

SOURCE=.\res\app\IEHistory.ico
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(105).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(106).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(107).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(111).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(179).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(74).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\imageres.dll(SHIDI_SHIELD_INTERNAL).ico"
# End Source File
# Begin Source File

SOURCE=.\res\Informacion.ico
# End Source File
# Begin Source File

SOURCE=.\res\jpgxp.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\keyboard.ico
# End Source File
# Begin Source File

SOURCE=.\res\keyboard.ico
# End Source File
# Begin Source File

SOURCE=.\res\Load1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Load2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Load3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Load4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Load5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Load6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconew\map.ico
# End Source File
# Begin Source File

SOURCE=.\res\menu51.bmp
# End Source File
# Begin Source File

SOURCE=..\Dll\MFC.ico
# End Source File
# Begin Source File

SOURCE=..\Dll\res\MFC.ico
# End Source File
# Begin Source File

SOURCE=.\res\mini.bmp
# End Source File
# Begin Source File

SOURCE=.\MyPass.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\MyPass.ico
# End Source File
# Begin Source File

SOURCE=.\res\Mypsd_2080_201011191737420001B.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NetBot.ico
# End Source File
# Begin Source File

SOURCE=.\res\app\NetState.ico
# End Source File
# Begin Source File

SOURCE=.\res\new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\newbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NoCam.ico
# End Source File
# Begin Source File

SOURCE=.\res\OK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\iconew\Prank.ico
# End Source File
# Begin Source File

SOURCE=.\res\Process.ico
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\QQ.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\regedit.ico
# End Source File
# Begin Source File

SOURCE=.\res\regedit.ico
# End Source File
# Begin Source File

SOURCE=.\res\regsz.ico
# End Source File
# Begin Source File

SOURCE=.\res\remote.ico
# End Source File
# Begin Source File

SOURCE=.\res\safe.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\screem.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\screen.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\service.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico2\Servts0.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\Servts0.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico2\Servts1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\Servts1.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico2\Servts2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\Servts2.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\setting.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\shell.ico
# End Source File
# Begin Source File

SOURCE=.\res\app\SoftWare.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\sound.ico
# End Source File
# Begin Source File

SOURCE=.\res\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TOOLBAR\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\sysico_0.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\sysinfo.ico
# End Source File
# Begin Source File

SOURCE=.\res\syskm.ico
# End Source File
# Begin Source File

SOURCE=.\res\iconew\system.ico
# End Source File
# Begin Source File

SOURCE=.\res\system.ico
# End Source File
# Begin Source File

SOURCE=.\res\systray1.ico
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=..\Dll\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\GuoQiICO\Tux.ico
# End Source File
# Begin Source File

SOURCE=.\res\icos\updll.ico
# End Source File
# Begin Source File

SOURCE=.\res\upload.ico
# End Source File
# Begin Source File

SOURCE=.\res\webcam.ico
# End Source File
# Begin Source File

SOURCE=.\res\zhuji.bmp
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����͢.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����ݽ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��ɳ����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ϺͰͲ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�µ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ĵ�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ͰͶ�˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ͳ����¼�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�͹���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ͻ�˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�׶���˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������Լ��֯.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ʱ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ά��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��˹���Ǻͺ�����ά��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����ɷ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��¡��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\��������.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\�����ն�.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\�����ͻ�.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�¹�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����ǹ���Э��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������ҹ�Э.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��϶��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ɱ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��ý�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\�������.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ա���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�չ�(��).ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�չ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ױ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��˹�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����ɴ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��³����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ű�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\���ڳ���.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Э.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�鶼��˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������˹˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����Ǳ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ձȺ���ͬ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ô�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\�����ɹ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\����ʧ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����կ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\���̼�¼.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ݿ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Ͳ�Τ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����¡.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Ħ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ص���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���޵���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ά��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ȹ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���Ϲ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��֧��ʿ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\¬ɭ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\¬����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����˹��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ά.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ܶ�Ⱥ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ë����˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ë��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����������֯.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ɹ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ϼ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��³.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ܿ�������������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ħ������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ħ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ħ�ɸ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Īɣ�ȿ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ī����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ױ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ϸ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�Ჴ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ն�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ų��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ŷ������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\��Ļ���.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ձ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��ʿ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����߶�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����ά��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ڼӶ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����·˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ɳ�ذ�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\icos\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ʥ��������������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ʥ���ĺ���ά˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ʥ¬����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ʥ����ŵ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ʥ��ɭ�غ͸����ɶ�˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\��Ƶ���.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\˹������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\˹�工��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\˹��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\˹��ʿ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�յ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������Ⱥ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\̨��ʡ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\̩��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\̫ƽ��ͬ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\̹ɣ����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�������Ͷ�͸�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Ѷqqͼ������7.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Ѷqqͼ������8.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ͻ��˹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ͼ��¬.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\�˳�����.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��Ŭ��ͼ.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Σ������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ί������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\�ļ�����.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\�ļ���1.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ڸɴ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ڿ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���ȱ��˹̹.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��IP���ݿ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����Ħ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ϣ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\ϵͳ����.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\ϵͳ����.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�¼���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ҳ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��ɫ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ӡ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\ӡ��������.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ӣ����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Ӣ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\��������.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\��������.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Լ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\Խ��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�ޱ���.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\է��.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�з�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�й�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\�ظ�����.ico"
# End Source File
# Begin Source File

SOURCE=".\res\����.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\TOOLBAR\ע���.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\�³.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\��ٸ�.ico"
# End Source File
# Begin Source File

SOURCE=".\res\GuoQiICO\쳼�.ico"
# End Source File
# End Group
# Begin Source File

SOURCE=".\res\����ǩ��\360�������.cer"
# End Source File
# Begin Source File

SOURCE=".\res\����ǩ��\360��ʿ.cer"
# End Source File
# Begin Source File

SOURCE=.\res\Html\Baidu.html
# End Source File
# Begin Source File

SOURCE=.\res\Html\BaiduE.html
# End Source File
# Begin Source File

SOURCE=.\res\Html\Gaode.html
# End Source File
# Begin Source File

SOURCE=.\res\Html\GaodeE.html
# End Source File
# Begin Source File

SOURCE=.\res\Sound\Login.wav
# End Source File
# Begin Source File

SOURCE=.\res\Sound\Offline.wav
# End Source File
# Begin Source File

SOURCE=".\res\����ǩ��\QQ����.cer"
# End Source File
# Begin Source File

SOURCE=.\res\Sound\Setting.wav
# End Source File
# Begin Source File

SOURCE=.\res\Skins\SkinH.she
# End Source File
# Begin Source File

SOURCE=.\res\UPX\UPX.exe
# End Source File
# Begin Source File

SOURCE=.\res\xp.xml
# End Source File
# Begin Source File

SOURCE=".\res\����ǩ��\�ṷ.cer"
# End Source File
# Begin Source File

SOURCE=".\res\����ǩ��\��ѶQQ.cer"
# End Source File
# Begin Source File

SOURCE=".\res\����ǩ��\Ѹ��.cer"
# End Source File
# End Target
# End Project
# Section Client : {3614C646-3B3B-4DE7-A81E-930E3F2127B3}
# 	2:5:Class:CWMPErrorItem
# 	2:10:HeaderFile:wmperroritem.h
# 	2:8:ImplFile:wmperroritem.cpp
# End Section
# Section Client : {94D55E95-3FAC-11D3-B155-00C04F79FAA6}
# 	2:5:Class:CWMPMedia
# 	2:10:HeaderFile:wmpmedia.h
# 	2:8:ImplFile:wmpmedia.cpp
# End Section
# Section Client : {EC21B779-EDEF-462D-BBA4-AD9DDE2B29A7}
# 	2:5:Class:CWMPNetwork
# 	2:10:HeaderFile:wmpnetwork.h
# 	2:8:ImplFile:wmpnetwork.cpp
# End Section
# Section Client : {4A976298-8C0D-11D3-B389-00C04F68574B}
# 	2:5:Class:CWMPStringCollection
# 	2:10:HeaderFile:wmpstringcollection.h
# 	2:8:ImplFile:wmpstringcollection.cpp
# End Section
# Section Client : {8363BC22-B4B4-4B19-989D-1CD765749DD1}
# 	2:5:Class:CWMPMediaCollection
# 	2:10:HeaderFile:wmpmediacollection.h
# 	2:8:ImplFile:wmpmediacollection.cpp
# End Section
# Section Client : {10A13217-23A7-439B-B1C0-D847C79B7774}
# 	2:5:Class:CWMPPlaylistCollection
# 	2:10:HeaderFile:wmpplaylistcollection.h
# 	2:8:ImplFile:wmpplaylistcollection.cpp
# End Section
# Section Client : {CFAB6E98-8730-11D3-B388-00C04F68574B}
# 	2:5:Class:CWMPCdrom
# 	2:10:HeaderFile:wmpcdrom.h
# 	2:8:ImplFile:wmpcdrom.cpp
# End Section
# Section Client : {74C09E02-F828-11D2-A74B-00A0C905F36E}
# 	2:5:Class:CWMPControls
# 	2:10:HeaderFile:wmpcontrols.h
# 	2:8:ImplFile:wmpcontrols.cpp
# End Section
# Section Client : {679409C0-99F7-11D3-9FB7-00105AA620BB}
# 	2:5:Class:CWMPPlaylistArray
# 	2:10:HeaderFile:wmpplaylistarray.h
# 	2:8:ImplFile:wmpplaylistarray.cpp
# End Section
# Section Client : {8DA61686-4668-4A5C-AE5D-803193293DBE}
# 	2:5:Class:CWMPDVD
# 	2:10:HeaderFile:wmpdvd.h
# 	2:8:ImplFile:wmpdvd.cpp
# End Section
# Section Client : {6C497D62-8919-413C-82DB-E935FB3EC584}
# 	2:5:Class:CWMPPlayer4
# 	2:10:HeaderFile:wmpplayer4.h
# 	2:8:ImplFile:wmpplayer4.cpp
# End Section
# Section Client : {40897764-CEAB-47BE-AD4A-8E28537F9BBF}
# 	2:5:Class:CWMPPlayerApplication
# 	2:10:HeaderFile:wmpplayerapplication.h
# 	2:8:ImplFile:wmpplayerapplication.cpp
# End Section
# Section Client : {6BF52A52-394A-11D3-B153-00C04F79FAA6}
# 	2:21:DefaultSinkHeaderFile:wmpplayer4.h
# 	2:16:DefaultSinkClass:CWMPPlayer4
# End Section
# Section Client : {9104D1AB-80C9-4FED-ABF0-2E6417A6DF14}
# 	2:5:Class:CWMPSettings
# 	2:10:HeaderFile:wmpsettings.h
# 	2:8:ImplFile:wmpsettings.cpp
# End Section
# Section Client : {A12DCF7D-14AB-4C1B-A8CD-63909F06025B}
# 	2:5:Class:CWMPError
# 	2:10:HeaderFile:wmperror.h
# 	2:8:ImplFile:wmperror.cpp
# End Section
# Section Client : {4F2DF574-C588-11D3-9ED0-00C04FB6E937}
# 	2:5:Class:CWMPClosedCaption
# 	2:10:HeaderFile:wmpclosedcaption.h
# 	2:8:ImplFile:wmpclosedcaption.cpp
# End Section
# Section Client : {EE4C8FE2-34B2-11D3-A3BF-006097C9B344}
# 	2:5:Class:CWMPCdromCollection
# 	2:10:HeaderFile:wmpcdromcollection.h
# 	2:8:ImplFile:wmpcdromcollection.cpp
# End Section
# Section Client : {D5F0F4F1-130C-11D3-B14E-00C04F79FAA6}
# 	2:5:Class:CWMPPlaylist
# 	2:10:HeaderFile:wmpplaylist.h
# 	2:8:ImplFile:wmpplaylist.cpp
# End Section
