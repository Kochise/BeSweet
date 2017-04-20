# Microsoft Developer Studio Project File - Name="ESBDSAddin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ESBDSAddin - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ESBDSAddin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ESBDSAddin.mak" CFG="ESBDSAddin - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ESBDSAddin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ESBDSAddin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ESBDSAddin"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ESBDSAddin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "objects/Debug"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".." /I "./src" /I "./src/stubs" /I "../../EclipseStyleBrowsers/" /I "../ESBSErver/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /pdb:"objects/debug/ESBDSAddinD.pdb" /debug /machine:I386 /out:"bin/ESBDSAddinD.dll" /pdbtype:sept
# SUBTRACT LINK32 /verbose /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\bin
TargetPath=.\bin\ESBDSAddinD.dll
InputPath=.\bin\ESBDSAddinD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ESBDSAddin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ESBDSAddin___Win32_Release"
# PROP BASE Intermediate_Dir "ESBDSAddin___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "bin"
# PROP Intermediate_Dir "objects/Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /I "./src" /I "./src/stubs" /I "../../EclipseStyleBrowsers/" /I "../ESBSErver/src" /D "WIN32" /D "_NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /dll /pdb:none /machine:I386
# SUBTRACT LINK32 /debug
# Begin Custom Build - Performing registration
OutDir=.\bin
TargetPath=.\bin\ESBDSAddin.dll
InputPath=.\bin\ESBDSAddin.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ESBDSAddin - Win32 Debug"
# Name "ESBDSAddin - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "VC7"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ObjectModelVC7\AddinVC7.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\AddinVC7.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\CommandHandlerVC7.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\CommandHandlerVC7.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\EventHandlerVC7.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\EventHandlerVC7.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\LoggerConsumerVC7.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC7\ObjectModelVC7.h
# End Source File
# End Group
# Begin Group "VC6"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ObjectModelVC6\AddinVC6.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\AddinVC6.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\CommandHandlerVC6.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\CommandHandlerVC6.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\EventHandlerVC6.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\EventHandlerVC6.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\LoggerConsumerVC6.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectModelVC6\ObjectModelVC6.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\BeSweetPreferencesPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\BeSweetPreferencesPanel.h
# End Source File
# Begin Source File

SOURCE=.\src\ComHelper.h
# End Source File
# Begin Source File

SOURCE=.\src\CommandHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CommandHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\DSAddin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DSAddin.h
# End Source File
# Begin Source File

SOURCE=.\src\DSAddinDllModule.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ESBDSAddin.def
# End Source File
# Begin Source File

SOURCE=.\src\EventHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EventHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\src\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\src\VSObjectModelHelper.h
# End Source File
# End Group
# Begin Group "IDL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ESBDSAddin.idl
# ADD MTL /h "src/stubs/ESBDSAddIn.h" /iid "src/stubs/ESBDSAddIn_i.c" /Oicf
# SUBTRACT MTL /nologo
# End Source File
# Begin Source File

SOURCE=..\ESBServer\src\RemoteInterface\COM\ESBServer.idl
# ADD MTL /nologo /I "src" /h "src/stubs/ESBServer.h" /iid "src/stubs/ESBServer_i.c" /Oicf
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\addin_vc6.rgs
# End Source File
# Begin Source File

SOURCE=.\res\addin_vc7.rgs
# End Source File
# Begin Source File

SOURCE=.\res\commands_vc6.rgs
# End Source File
# Begin Source File

SOURCE=.\res\commands_vc7.rgs
# End Source File
# Begin Source File

SOURCE=.\ESBDSAddin.rc
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409 /i "./src"
# End Source File
# Begin Source File

SOURCE=.\res\hierarchy.bmp
# End Source File
# Begin Source File

SOURCE=.\res\methods.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outline.bmp
# End Source File
# Begin Source File

SOURCE=.\res\properties.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TBarLrge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TBarMedm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\types.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspace.bmp
# End Source File
# End Group
# End Target
# End Project
