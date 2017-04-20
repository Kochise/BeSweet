; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CustomOutlineFilter
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "resource.h"
LastPage=0

ClassCount=14
Class1=OutlineBrowserRenderer
Class2=AboutDlg
Class3=ESBServerControlPanel
Class4=PropertiesTree
Class5=CTags::PropertyPanel
Class6=Browsers::OutlinePropertyPanel

ResourceCount=23
Resource1=IDD_PACKAGE_BROWSER_PANEL
Resource2=IDD_OUTLINE_BROWSER_PROPERTIES_PANEL
Resource3=IDD_LOGGER_PROPERTIES_PANEL
Resource4=IDD_EMPTY_DIALOG
Resource5=IDD_LOGGER_PANEL
Resource6=IDD_CTAGS_PROPERTIES_PANEL
Resource7=IDD_CUSTOM_OUTLINE_FILTER
Resource8=IDR_OUTLINE_BROWSER_POPUPMENU
Resource9=IDD_DIALOG1
Resource10=IDD_HIERARCHY_BROWSER_PANEL
Class7=HierarchyBrowserRenderer
Resource11=IDD_ESBSERVER_CONRTOL_PANEL
Class8=PackageBrowserRenderer
Resource12=IDR_PREVIOUS_HIERARCHIES_POPUPMENU
Resource13=IDD_ABOUTBOX
Resource14=IDD_VERSION_CHECKING
Resource15=IDR_ESBSERVER_POPUPMENU
Resource16=IDR_WORKSPACE_POPUPMENU
Class9=TagBrosersProperty
Resource17=IDD_PROGRESS
Class10=LoggerPanel
Resource18=IDD_SCOPEDNAME_BROWSER
Class11=VersionCheckPanel
Resource19=IDD_APPEARANCE_PROPERTIES_PANEL
Class12=LoggerPropertiesPanel
Resource20=IDD_HIERARCHY_BROWSER_PROPERTIES_PANEL
Class13=Progressbar
Resource21=IDR_HIERARCHY_BROWSER_POPUPMENU
Resource22=IDD_ESBSERVER_OPTION_PANEL
Class14=CustomOutlineFilter
Resource23=IDR_CUSTOM_OUTLINE_FILTER

[CLS:OutlineBrowserRenderer]
Type=0
BaseClass=ParentClass 
HeaderFile=Renderer\MFC\Browsers\OutlineBrowserRenderer.h
ImplementationFile=Renderer\MFC\Browsers\OutlineBrowserRenderer.cpp
LastObject=ID_ESBSERVER_ABOUT
Filter=D
VirtualFilter=dWC

[CLS:AboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Renderer\MFC\ESBServerManagerRenderer.cpp
ImplementationFile=Renderer\MFC\ESBServerManagerRenderer.cpp
LastObject=AboutDlg
Filter=D
VirtualFilter=dWC

[CLS:ESBServerControlPanel]
Type=0
BaseClass=ESBParent
HeaderFile=Renderer\MFC\ESBServerCtrlPanel.h
ImplementationFile=Renderer\MFC\ESBServerCtrlPanel.cpp
LastObject=IDC_PACKAGES_BROWSER
Filter=N

[CLS:CTags::PropertyPanel]
Type=0
HeaderFile=Renderer\MFC\Properties\CTagsPropertyPanel.h
ImplementationFile=Renderer\MFC\Properties\CTagsPropertyPanel.cpp
LastObject=CTags::PropertyPanel

[CLS:Browsers::OutlinePropertyPanel]
Type=0
BaseClass=RaisableDialog
HeaderFile=Renderer\MFC\Properties\OutlinePropertyPanel.h
ImplementationFile=Renderer\MFC\Properties\OutlinePropertyPanel.cpp
LastObject=Browsers::OutlinePropertyPanel

[CLS:PropertiesTree]
Type=0
BaseClass=CTreeCtrl
HeaderFile=Renderer\MFC\Properties\PropertiesTree.h
ImplementationFile=Renderer\MFC\Properties\PropertiesTree.cpp

[DLG:IDD_SCOPEDNAME_BROWSER]
Type=1
Class=TypeBrowserRenderer
ControlCount=8
Control1=IDC_FILTER,edit,1350631552
Control2=IDC_TYPES,SysListView32,1350680973
Control3=IDC_QUALIFIERS,SysListView32,1350680973
Control4=IDC_CLOSE,button,1342242816
Control5=IDCANCEL,button,1342242816
Control6=IDC_CHOOSE,static,1342308352
Control7=IDC_MATCHES,static,1342308352
Control8=IDC_QUALIFIER_LABEL,static,1342308876

[DLG:IDD_ABOUTBOX]
Type=1
Class=AboutDlg
ControlCount=8
Control1=IDC_THIS_VERSION,static,1342308480
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889
Control4=IDC_HOMEPAGE,static,1342308352
Control5=IDC_LATEST_VERSION,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DOWNLOADPAGE,static,1342308352
Control8=IDC_STATIC,static,1342308352

[DLG:IDD_ESBSERVER_CONRTOL_PANEL]
Type=1
Class=ESBServerControlPanel
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDC_TYPE_BROWSER,button,1342242816
Control3=IDC_OUTLINE_BROWSER,button,1342242816
Control4=IDC_MOUSE_POS,static,1073872896
Control5=IDC_WORKSPACE,button,1342242816
Control6=IDC_METHOD_BROWSER,button,1342242816
Control7=IDC_PREFERENCES,button,1073807360
Control8=IDC_HIERARCHY_BROWSER,button,1342242816
Control9=IDC_PACKAGES_BROWSER,button,1342242816

[DLG:IDD_ESBSERVER_OPTION_PANEL]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_CTAGGER,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[DLG:IDD_CTAGS_PROPERTIES_PANEL]
Type=1
Class=CTags::PropertyPanel
ControlCount=6
Control1=IDC_CTAGS_EXE,edit,1350631552
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BROWSE,button,1342259200
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ADDITIONAL_CTAGS_OPTIONS,edit,1350635972
Control6=IDC_SHOW_CTAGGER,button,1342242819

[MNU:IDR_OUTLINE_BROWSER_POPUPMENU]
Type=1
Class=?
Command1=ID_OUTLINEBROWSER_HEADER
Command2=ID_OUTLINEBROWSER_SOURCE
Command3=ID_OUTLINEBROWSER_HIERARCHY
Command4=ID_OUTLINEBROWSER_INSPECT
CommandCount=4

[MNU:IDR_ESBSERVER_POPUPMENU]
Type=1
Class=?
Command1=ID_ESBSERVER_ESBSERVER
Command2=ID_ESBSERVER_TYPEBROWSER
Command3=ID_ESBSERVER_METHODBROWSER
Command4=ID_ESBSERVER_WORKSPACEBROWSER
Command5=ID_ESBSERVER_REPARSE
Command6=ID_ESBSERVER_PREFERENCES
Command7=ID_ESBSERVER_LOGWINDOW
Command8=ID_ESBSERVER_MAXIMIZE
Command9=ID_ESBSERVER_MINIMIZE
Command10=ID_ESBSERVER_ABOUT
Command11=ID_ESBSERVER_EXIT
CommandCount=11

[MNU:IDR_WORKSPACE_POPUPMENU]
Type=1
Class=?
Command1=ID_REPARSE_OUTLINE
CommandCount=1

[DLG:IDD_OUTLINE_BROWSER_PROPERTIES_PANEL]
Type=1
Class=Browsers::OutlinePropertyPanel
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_HOVER,edit,1350639744
Control3=IDC_SHOW_HEADER_AND_SOURCE_FILE_CONETENT,button,1342242819
Control4=IDC_SHOW_NAMESPACES,button,1342242819
Control5=IDC_STATIC,static,1342177287
Control6=IDC_TOPLEFT,button,1342177321
Control7=IDC_BOTTOMRIGHT,button,1342177289
Control8=IDC_TOPRIGHT,button,1342177289
Control9=IDC_BOTTOMLEFT,button,1342177289
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_HIERARCHY_BROWSER_PANEL]
Type=1
Class=HierarchyBrowserRenderer
ControlCount=3
Control1=IDC_HIERARCHY,SysTreeView32,1350633527
Control2=IDC_OUTLINE_LABEL,static,1342308864
Control3=IDC_OUTLINE_ICON,static,1342177283

[CLS:HierarchyBrowserRenderer]
Type=0
HeaderFile=renderer\mfc\browsers\HierarchyBrowserRenderer.h
ImplementationFile=renderer\mfc\browsers\HierarchyBrowserRenderer.cpp
BaseClass=RaisableDialog
Filter=D
LastObject=HierarchyBrowserRenderer
VirtualFilter=dWC



[DLG:IDD_DIALOG1]
Type=1
Class=PackageBrowserRenderer
ControlCount=5
Control1=IDC_CLASSES,SysListView32,1350631424
Control2=IDC_NAMESPACES,SysListView32,1350631424
Control3=IDC_MEMBERS1,SysListView32,1350631424
Control4=IDC_SPLITTERBAR1,static,1342177287
Control5=IDC_SPLITTERBAR2,static,1342177287

[CLS:PackageBrowserRenderer]
Type=0
HeaderFile=renderer\mfc\browsers\PackageBrowserRenderer.h
ImplementationFile=renderer\mfc\browsers\PackageBrowserRenderer.cpp
BaseClass=RaisableDialog
Filter=D
LastObject=ID_ESBSERVER_ESBSERVER
VirtualFilter=dWC

[DLG:IDD_PACKAGE_BROWSER_PANEL]
Type=1
Class=?
ControlCount=11
Control1=IDC_TYPES,SysListView32,1350664589
Control2=IDC_NAMESPACES,SysListView32,1350664589
Control3=IDC_MEMBERS1,SysListView32,1350664589
Control4=IDC_SPLITTERBAR1,static,1342177295
Control5=IDC_SPLITTERBAR2,static,1342177295
Control6=IDC_NSPACE_CAPTION,button,1342177287
Control7=IDC_MEMBERS_CAPTION,button,1342177287
Control8=IDC_TYPES_CAPTION,button,1342177287
Control9=IDC_STATIC,static,1342177294
Control10=IDC_STATIC,static,1342177294
Control11=IDC_STATIC,static,1342177294

[MNU:IDR_PREVIOUS_HIERARCHIES_POPUPMENU]
Type=1
Class=?
Command1=ID_PREVIOUSHIERARCHIES
CommandCount=1

[DLG:IDD_EMPTY_DIALOG]
Type=1
Class=OutlineBrowserRenderer
ControlCount=0

[MNU:IDR_HIERARCHY_BROWSER_POPUPMENU]
Type=1
Class=?
Command1=ID_HIERARCHY_FOCUSON
Command2=ID_HIERARCHY_FOCUSONX
CommandCount=2

[DLG:IDD_HIERARCHY_BROWSER_PROPERTIES_PANEL]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,static,1342308352
Control2=IDC_HOVER,edit,1350639744
Control3=IDC_STATIC,static,1342177287
Control4=IDC_TOPLEFT,button,1342177321
Control5=IDC_BOTTOMRIGHT,button,1342177289
Control6=IDC_TOPRIGHT,button,1342177289
Control7=IDC_BOTTOMLEFT,button,1342177289
Control8=IDC_SHOW_NAMESPACES,button,1342242819
Control9=IDC_WARN_MULTIPLE_PARENTS,button,1342242819
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_APPEARANCE_PROPERTIES_PANEL]
Type=1
Class=?
ControlCount=28
Control1=IDC_AUTO_RAISE,button,1342242819
Control2=IDC_HIDE,button,1342242819
Control3=IDC_SHOW_RETURN_TYPES,button,1342242819
Control4=IDC_CLOSE_ON_RETURN,button,1342242819
Control5=IDC_SHOW_TOOLTIPS,button,1342242819
Control6=IDC_ROLL,button,1342242819
Control7=IDC_ALLWAYS_ONTOP,button,1342242819
Control8=IDC_ROLL_TIME,edit,1350639744
Control9=IDC_ROLL_WIDTH,edit,1350639744
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_FADE,button,1342242819
Control13=IDC_MIN_OPACITY,edit,1350639744
Control14=IDC_MAX_OPACITY,edit,1350639744
Control15=IDC_FADE_TIME,edit,1350639744
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,button,1342177287
Control22=IDC_ROLL_HEIGHT,edit,1350639744
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_ENLARGE,edit,1350639744
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352

[CLS:TagBrosersProperty]
Type=0
HeaderFile=TagBrosersProperty.h
ImplementationFile=TagBrosersProperty.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_ESBSERVER_ESBSERVER

[DLG:IDD_LOGGER_PANEL]
Type=1
Class=LoggerPanel
ControlCount=1
Control1=IDC_LOG_ENTRY_LIST,SysListView32,1350664193

[CLS:LoggerPanel]
Type=0
HeaderFile=renderer\mfc\LoggerPanel.h
ImplementationFile=renderer\mfc\LoggerPanel.cpp
BaseClass=super
Filter=D
LastObject=ID_ESBSERVER_ABOUT

[DLG:IDD_VERSION_CHECKING]
Type=1
Class=VersionCheckPanel
ControlCount=6
Control1=IDOK,button,1073807360
Control2=IDC_DISABLE,button,1342242819
Control3=IDC_PROGRESS,msctls_progress32,1350565889
Control4=IDC_PREFIX,static,1073872896
Control5=IDC_LINK,static,1073872896
Control6=IDC_POSTFIX,static,1073872896

[CLS:VersionCheckPanel]
Type=0
HeaderFile=renderer\mfc\versioncheckpanel.h
ImplementationFile=renderer\mfc\versioncheckpanel.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_ESBSERVER_ABOUT

[DLG:IDD_LOGGER_PROPERTIES_PANEL]
Type=1
Class=LoggerPropertiesPanel
ControlCount=2
Control1=IDC_LOGLEVEL,combobox,1344339970
Control2=IDC_STATIC,static,1342308352

[CLS:LoggerPropertiesPanel]
Type=0
HeaderFile=renderer\mfc\properties\LoggerPropertiesPanel.h
ImplementationFile=renderer\mfc\properties\LoggerPropertiesPanel.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_ESBSERVER_ABOUT
VirtualFilter=dWC

[DLG:IDD_PROGRESS]
Type=1
Class=?
ControlCount=1
Control1=IDC_PROGRESS,msctls_progress32,1342177281

[CLS:Progressbar]
Type=0
HeaderFile=renderer\mfc\Progressbar.h
ImplementationFile=renderer\mfc\Progressbar.h
BaseClass=CDialog
Filter=D
LastObject=ID_ESBSERVER_ABOUT

[MNU:IDR_CUSTOM_OUTLINE_FILTER]
Type=1
Class=?
CommandCount=0

[DLG:IDD_CUSTOM_OUTLINE_FILTER]
Type=1
Class=CustomOutlineFilter
ControlCount=31
Control1=IDC_TYPE_PUBLIC,button,1342242819
Control2=IDC_TYPE_PROTECTED,button,1342242819
Control3=IDC_TYPE_PRIVATE,button,1342242819
Control4=IDC_TYPE_CLASS,button,1342242819
Control5=IDC_TYPE_STRUCT,button,1342242819
Control6=IDC_TYPE_UNION,button,1342242819
Control7=IDC_TYPE_ENUM,button,1342242819
Control8=IDC_TYPE_TYPEDEF,button,1342242819
Control9=IDC_FIELD_PUBLIC,button,1342242819
Control10=IDC_FIELD_PROTECTED,button,1342242819
Control11=IDC_FIELD_PRIVATE,button,1342242819
Control12=IDC_FIELD_CONST,button,1342242819
Control13=IDC_FIELD_STATIC,button,1342242819
Control14=IDC_METHOD_PUBLIC,button,1342242819
Control15=IDC_METHOD_PROTECTED,button,1342242819
Control16=IDC_METHOD_PRIVATE,button,1342242819
Control17=IDC_METHOD_ABSTRACT,button,1342242819
Control18=IDC_METHOD_VIRTUAL,button,1342242819
Control19=IDC_METHOD_CONST,button,1342242819
Control20=IDC_METHOD_STATIC,button,1342242819
Control21=IDOK,button,1342242817
Control22=IDCANCEL,button,1342242816
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,button,1342177287
Control25=IDC_STATIC,button,1342177287
Control26=IDC_TYPES,edit,1350631552
Control27=IDC_FIELDS,edit,1350631552
Control28=IDC_METHODS,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352

[CLS:CustomOutlineFilter]
Type=0
HeaderFile=renderer\mfc\browsers\CustomOutlineFilterRenderer.h
ImplementationFile=renderer\mfc\browsers\CustomOutlineFilterRenderer.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_TYPES
VirtualFilter=dWC

