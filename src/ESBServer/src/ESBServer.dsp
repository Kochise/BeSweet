# Microsoft Developer Studio Project File - Name="ESBServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ESBServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ESBServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ESBServer.mak" CFG="ESBServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ESBServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ESBServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "ESBServer"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ESBServer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../objects/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zd /I "." /I "RemoteInterface/COM" /I "Renderer/MFC" /I "Renderer/MFC/3rdParty" /I "Renderer/MFC/Base" /I "Renderer/MFC/3rdParty/OptionPage" /I "Renderer/MFC/Browsers" /I "Utilities" /I "Model" /I "Model/CTags" /I "Model/Simulators" /I "Controller" /I "Controller/Browsers" /I "../../ESBDSAddin/src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib vfw32.lib Wininet.lib /nologo /subsystem:windows /debug /machine:I386

!ELSEIF  "$(CFG)" == "ESBServer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin"
# PROP Intermediate_Dir "../objects/Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "RemoteInterface/COM" /I "Renderer/MFC" /I "Renderer/MFC/3rdParty" /I "Renderer/MFC/Base" /I "Renderer/MFC/3rdParty/OptionPage" /I "Renderer/MFC/Browsers" /I "Utilities" /I "Model" /I "Model/CTags" /I "Model/Simulators" /I "Controller" /I "Controller/Browsers" /I "../../ESBDSAddin/src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/ESBServer.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vfw32.lib version.lib Wininet.lib /nologo /subsystem:windows /pdb:"../objects/debug/ESBServer.pdb" /debug /machine:I386 /out:"../bin/ESBServerD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ESBServer - Win32 Release"
# Name "ESBServer - Win32 Debug"
# Begin Group "Model"

# PROP Default_Filter ""
# Begin Group "ctags"

# PROP Default_Filter ""
# Begin Group "Loaders_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\CTags\CTagsFileOutlineLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsFileOutlineLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsHierarchyLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsHierarchyLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsMethodLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsMethodLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsOutlineLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsOutlineLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsTypeInfoLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsTypeInfoLoader.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Model\CTags\CTagsDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsDatabase.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsMetaObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsMetaObjectFactory.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsModelBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsModelBuilder.h
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsTag.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\CTags\CTagsTag.h
# End Source File
# End Group
# Begin Group "Simulators"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\Simulators\FileOutlineLoaderSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\Simulators\FileOutlineLoaderSimulator.h
# End Source File
# Begin Source File

SOURCE=.\Model\Simulators\HierarchyLoaderSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\Simulators\HierarchyLoaderSimulator.h
# End Source File
# Begin Source File

SOURCE=.\Model\Simulators\TypeInfoLoaderSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\Simulators\TypeInfoLoaderSimulator.h
# End Source File
# End Group
# Begin Group "MetaModel"

# PROP Default_Filter ""
# Begin Group "MetaTypes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\MetaModel\Class.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Class.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Enum.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Enum.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Field.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Field.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Function.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Method.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Method.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Namespace.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Namespace.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Struct.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Struct.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Typedef.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Typedef.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Union.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Union.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Variable.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Variable.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Model\MetaModel\Inheritable.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\Inheritable.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModel.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelDefines.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelFilter.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelPolicies.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaModelUtilities.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaObject.h
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaObjectVisitors.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\MetaModel\MetaObjectVisitors.h
# End Source File
# End Group
# Begin Group "Loaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Model\FileOutlineLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\HierarchyLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\MethodLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\OutlineLoader.h
# End Source File
# Begin Source File

SOURCE=.\Model\TypeInfoLoader.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Model\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\Editor.h
# End Source File
# Begin Source File

SOURCE=.\Model\FullQualifiedName.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\FullQualifiedName.h
# End Source File
# Begin Source File

SOURCE=.\Model\LoaderBroker.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\LoaderBroker.h
# End Source File
# Begin Source File

SOURCE=.\Model\NameMangler.cpp
# End Source File
# Begin Source File

SOURCE=.\Model\NameMangler.h
# End Source File
# Begin Source File

SOURCE=.\Model\TagLocation.h
# End Source File
# Begin Source File

SOURCE=.\Model\VSWorkspaceLoader.cpp
# End Source File
# End Group
# Begin Group "Controller"

# PROP Default_Filter ""
# Begin Group "Browsers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Controller\Browsers\HierarchyBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\HierarchyBrowser.h
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\OutlineBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\OutlineBrowser.h
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\ScopedNameBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\ScopedNameBrowser.h
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\SelectionListener.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\SelectionListener.h
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\TypeBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\TypeBrowser.h
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\WorkspaceBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\Browsers\WorkspaceBrowser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Controller\ESBServer.cpp

!IF  "$(CFG)" == "ESBServer - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "ESBServer - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Controller\ESBServer.h
# End Source File
# Begin Source File

SOURCE=.\Controller\ESBServerManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Controller\ESBServerManager.h
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Group "MFC"

# PROP Default_Filter ""
# Begin Group "3rd-Party"

# PROP Default_Filter ""
# Begin Group "OptionPage"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionListBox.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionPage.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionPageStaticText.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionPageStaticText.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\OptionSheet.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\SMLDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\SMLDoc.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\SMLStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\OptionPage\SMLStatic.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\Dib.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\MltiTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\MltiTree.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\ResizingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\ResizingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\xySplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\3rdParty\xySplitterWnd.h
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\MFC\Base\AnimatedDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Base\AnimatedDialog.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Base\CustomDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Base\CustomDialog.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Base\MouseEventDispatcher.h
# End Source File
# End Group
# Begin Group "Browsers_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\CustomOutlineFilterRenderer.cpp
# ADD CPP /I "..\..\.."
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\CustomOutlineFilterRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\HierarchyBrowserRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\HierarchyBrowserRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\OutlineBrowserRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Mfc\Browsers\OutlineBrowserRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\PackageBrowserRenderer.cpp
# ADD CPP /I "..\..\.."
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\PackageBrowserRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\ScopedNameBrowserRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Mfc\Browsers\ScopedNameBrowserRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\TypeBrowserRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Mfc\Browsers\TypeBrowserRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Browsers\WorkspaceBrowserRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Mfc\Browsers\WorkspaceBrowserRenderer.h
# End Source File
# End Group
# Begin Group "ProperyPanels"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\AppearancePropertyPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\AppearancePropertyPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\CTagsPropertyPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\CTagsPropertyPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\HierarchyPropertyPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\HierarchyPropertyPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\LoggerPropertiesPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\LoggerPropertiesPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\OutlinePropertyPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\OutlinePropertyPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\PropertiesTree.cpp
# ADD CPP /I "..\..\.."
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Properties\PropertiesTree.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Renderer\MFC\ESBImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\ESBImageList.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\ESBServerCtrlPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\ESBServerCtrlPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\ESBServerManagerRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\ESBServerManagerRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\LoggerPanel.cpp
# ADD CPP /I "..\.."
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\LoggerPanel.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Progressbar.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\Progressbar.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\Mfc\resource.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\TreeCtrlHelper.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\versioncheckpanel.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\MFC\versioncheckpanel.h
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Renderer\Win32\Transparency.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\Win32\Transparency.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Renderer\IconBroker.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\IconDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\IconDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\LabelBroker.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\OutlineRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\TooltipBroker.h
# End Source File
# Begin Source File

SOURCE=.\Renderer\UIProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer\UIProperties.h
# End Source File
# End Group
# Begin Group "RemoteInteface"

# PROP Default_Filter ""
# Begin Group "COM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ESBDSAddin\src\ESBDSAddin.idl
# ADD MTL /h "RemoteInterface\COM\stubs\ESBDSAddin.h" /iid "RemoteInterface\COM\stubs\ESBDSAddin_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\ESBServer.idl

!IF  "$(CFG)" == "ESBServer - Win32 Release"

# ADD MTL /I "../../ESBDSAddin/src" /h "RemoteInterface/COM/stubs/IESBServer.h" /iid "RemoteInterface\COM\stubs\ESBServer_i.c" /Oicf

!ELSEIF  "$(CFG)" == "ESBServer - Win32 Debug"

# ADD MTL /I "../../ESBDSAddin/src" /tlb "../bin/ESBServerD.tlb" /h "RemoteInterface/COM/stubs/IESBServer.h" /iid "RemoteInterface\COM\stubs\ESBServer_i.c" /Oicf

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\ESBServerComModule.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\Com\ESBServerComModule.h
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\IESBServerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\Com\IESBServerImpl.h
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\IESBServerManagerImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\IESBServerManagerImpl.h
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\VSEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteInterface\COM\VSEditor.h
# End Source File
# End Group
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Utilities\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Properties.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\RegExpr.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\smart_ptr.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\Stacktrace.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities\Stacktrace.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\stl_ext.h
# End Source File
# Begin Source File

SOURCE=.\Utilities\StringTokenizer.h
# End Source File
# End Group
# Begin Group "Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\File With Blanks.cpp"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test\foo.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=T:\Welti\main.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE="T:\Welti\My Stacktrace.h"
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test\NamespaceTest.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test\NestedNamespaceInheritanceTest.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test\TestClasses.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Test\TestClasses.h
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\res\abstract256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\class256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\close_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\const256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\enum256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ESBServer.ico
# End Source File
# Begin Source File

SOURCE=.\ESBServer.rc
# End Source File
# Begin Source File

SOURCE=.\res\ESBServer.rc2
# End Source File
# Begin Source File

SOURCE=.\ESBServer.rgs
# End Source File
# Begin Source File

SOURCE=.\ESBServerManager.rgs
# End Source File
# Begin Source File

SOURCE=.\ESBServerMgr.rgs
# End Source File
# Begin Source File

SOURCE=.\res\field256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\function256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\headerfile256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hierarchy.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\inherited_members_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\inherited_members_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lock_hierarchy_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lock_hierarchy_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\members_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\method256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\namespace256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_field_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_field_s256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_fields_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_public_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_public_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_public_s256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_static_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_static_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\no_static_s256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\outline.ico
# End Source File
# Begin Source File

SOURCE=.\res\outline256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\overwriten256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\package_browser.ico
# End Source File
# Begin Source File

SOURCE=.\res\packages_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\previous_hierarchies_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\previous_hierarchies_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\private256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\private_field256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\private_method256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\problem256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\problem_256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\protected256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\protected_field256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\protected_method256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public__only_s256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public_field256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public_method256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public_only__f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\public_only_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\questionmark256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\questionmark_256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\selection_arrow256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sort_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sort_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sort_s256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sourcefile256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\static256.bmp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\res\struct256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\subtype_hierarchy_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\subtype_hierarchy_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\supertyp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\supertype_hierarchy_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\supertype_hierarchy_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type_hie.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type_hierarchy_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\type_hierarchy_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\typebrowser.ico
# End Source File
# Begin Source File

SOURCE=.\res\typedef256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\types_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\union256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\variable256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\virtual256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\workspace.ico
# End Source File
# Begin Source File

SOURCE=.\res\workspace256.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\cxf.ico
# End Source File
# Begin Source File

SOURCE=.\ESBServer.ini
# End Source File
# Begin Source File

SOURCE=.\res\filter.ico
# End Source File
# Begin Source File

SOURCE=.\res\filter_a256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\filter_f256.bmp
# End Source File
# Begin Source File

SOURCE=.\includes.txt
# End Source File
# Begin Source File

SOURCE=.\res\log_wind.ico
# End Source File
# Begin Source File

SOURCE=.\res\prototype256.bmp
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ESBServer : {20005C12-0003-0000-0400-040035070000}
# 	1:13:IDR_ESBSERVER:103
# End Section
