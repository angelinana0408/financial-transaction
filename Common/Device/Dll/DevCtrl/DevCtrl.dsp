# Microsoft Developer Studio Project File - Name="DevCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DevCtrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DevCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DevCtrl.mak" CFG="DevCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DevCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DevCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DevCtrl - Win32 Release"

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
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 shlwapi.lib wininet.lib CmnLib.lib EmvL2Kernel2.lib HS_EMVCORE.lib /nologo /subsystem:windows /dll /machine:I386 /out:"\T1Atm\App\ExeRele/DevCtrl.dll"

!ELSEIF  "$(CFG)" == "DevCtrl - Win32 Debug"

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
# ADD LINK32 shlwapi.lib wininet.lib CmnLib.lib EmvL2Kernel2.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"\T1Atm\App\ExeDebug/DevCtrl.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DevCtrl - Win32 Release"
# Name "DevCtrl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DevApl.cpp
# End Source File
# Begin Source File

SOURCE=.\DevApm.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCallBack.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCmn.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCom.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCsh.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.def
# End Source File
# Begin Source File

SOURCE=.\DevCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\DevEtc.cpp
# End Source File
# Begin Source File

SOURCE=.\DevNet.cpp
# End Source File
# Begin Source File

SOURCE=.\DevNms.cpp
# End Source File
# Begin Source File

SOURCE=.\DevScr.cpp
# End Source File
# Begin Source File

SOURCE=.\DevThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\H\Dll\nhmwi.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\H\Common\Class.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassInclude.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassPointer.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\ClassSetOwner.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\CmnLib.h
# End Source File
# Begin Source File

SOURCE=..\..\H\Common\CmnLibIn.h
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

SOURCE=..\..\H\Dll\nhmwi.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DevCtrl.rc2
# End Source File
# End Group
# End Target
# End Project
