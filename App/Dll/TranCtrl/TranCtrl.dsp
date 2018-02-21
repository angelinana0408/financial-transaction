# Microsoft Developer Studio Project File - Name="TranCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TranCtrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TranCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TranCtrl.mak" CFG="TranCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TranCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TranCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TranCtrl - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "\T1Atm\App\LibRele"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "_AFXEXT" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 CmnLib.lib DevCtrl.lib ProCtrl.lib NetCtrl.lib ComCtrl.lib LanCtrl.lib wininet.lib LcTCPClient.lib AccApi.lib HS_EMVCORE.lib /nologo /subsystem:windows /dll /machine:I386 /out:"\T1Atm\App\ExeRele/TranCtrl.dll"

!ELSEIF  "$(CFG)" == "TranCtrl - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "\T1Atm\App\LibDebug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CmnLib.lib DevCtrl.lib ProCtrl.lib NetCtrl.lib ComCtrl.lib wininet.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"\T1Atm\App\ExeDebug/TranCtrl.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TranCtrl - Win32 Release"
# Name "TranCtrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Builder Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\H\Builder\TranBeginProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranDepProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranEndProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranIcProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranInqProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranPbProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTranProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTransProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranWithProc.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TranAgentPorc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranAtmpProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCmn.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.def
# End Source File
# Begin Source File

SOURCE=.\TranCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\TranDevProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranDvr.cpp
# End Source File
# Begin Source File

SOURCE=.\TranHostProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranInputProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranLibProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranMainProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranMenuProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranNmsProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranOutputProc.cpp
# End Source File
# Begin Source File

SOURCE=.\TranTermProc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Builder Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\H\Builder\TranCtrlProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranCtrlProc_STA.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranFuncProc_STA.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranRetCode.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Builder\TranTranProc.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\H\Tran\AccApi.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\CmnLib.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\Comm.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ConstDef.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevCmn.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\DevEtc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\IcEmv.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranAgentProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranAtmpProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmn.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnEtc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnHost.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranCmnUser.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranDevProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranHostProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranInputProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranLibProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranMainProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranMenuProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranNmsProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranOutputProc.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Tran\TranTermProc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\TranCtrl.rc2
# End Source File
# End Group
# End Target
# End Project
