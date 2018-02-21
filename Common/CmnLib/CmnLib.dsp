# Microsoft Developer Studio Project File - Name="CmnLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CmnLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CmnLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CmnLib.mak" CFG="CmnLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CmnLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CmnLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CmnLib - Win32 Release"

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
# ADD CPP /nologo /Zp1 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"\T1Atm\App\ExeRele/CmnLib.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CmnLib - Win32 Debug"

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
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"\T1Atm\App\ExeDebug/CmnLibD.dll" /implib:"\T1atm\APP\LibRele/CmnLibD.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CmnLib - Win32 Release"
# Name "CmnLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CmnLib.cpp
# End Source File
# Begin Source File

SOURCE=.\CmnLib.def
# End Source File
# Begin Source File

SOURCE=.\CmnLib.rc
# End Source File
# Begin Source File

SOURCE=.\CodeCvt.cpp
# End Source File
# Begin Source File

SOURCE=.\Cryptogram.cpp
# End Source File
# Begin Source File

SOURCE=.\Delay.cpp
# End Source File
# Begin Source File

SOURCE=.\hy_auth.cpp
# End Source File
# Begin Source File

SOURCE=.\hy_monitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\RunExec.cpp
# End Source File
# Begin Source File

SOURCE=.\ShareMem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Trace.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeCvt.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Crypt"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Crypt\BaseTsd.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\cryptuiapi.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\PopPack.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\PshPack1.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\PshPack2.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\PshPack4.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\PshPack8.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\Security.h
# End Source File
# Begin Source File

SOURCE=.\Crypt\WinCrypt.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CmnLib.h
# End Source File
# Begin Source File

SOURCE=.\CmnLibEx.h
# End Source File
# Begin Source File

SOURCE=.\CmnLibIn.h
# End Source File
# Begin Source File

SOURCE=.\Cryptogram.h
# End Source File
# Begin Source File

SOURCE=.\HanCode.h
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

SOURCE=.\res\CmnLib.rc2
# End Source File
# End Group
# Begin Source File

SOURCE="\T1atm\App\LibRele\MonitorClient.lib"
# End Source File
# Begin Source File

SOURCE="\T1atm\App\LibRele\FJ8000Identify-VS6-MD-Release.lib"
# End Source File
# End Target
# End Project
