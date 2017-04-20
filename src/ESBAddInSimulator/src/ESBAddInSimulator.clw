; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=ESBAddInSimulatorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ESBAddInSimulator.h"

ClassCount=2
Class1=ESBAddInSimulatorApp
Class2=ESBAddInSimulatorDlg

ResourceCount=3
Resource2=IDD_ESBADDINSIMULATOR_DIALOG
Resource1=IDR_MAINFRAME
Resource3=IDR_MENU

[CLS:ESBAddInSimulatorApp]
Type=0
HeaderFile=ESBAddInSimulator.h
ImplementationFile=ESBAddInSimulator.cpp
Filter=N

[CLS:ESBAddInSimulatorDlg]
Type=0
HeaderFile=ESBAddInSimulatorDlg.h
ImplementationFile=ESBAddInSimulatorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_XXX_CLOSEWORKSPACE



[DLG:IDD_ESBADDINSIMULATOR_DIALOG]
Type=1
Class=ESBAddInSimulatorDlg
ControlCount=1
Control1=IDC_WORKSPACE,SysTreeView32,1350631591

[MNU:IDR_MENU]
Type=1
Class=ESBAddInSimulatorDlg
Command1=ID_SHOW
Command2=ID_REPARSE
Command3=ID_XXX_LOADWORKSPACE
Command4=ID_XXX_CLOSEWORKSPACE
Command5=ID_XXX_RAISEOUTLINEBROWSER
Command6=ID_XXX_RAISEHIERARCHYBROWSER
Command7=ID_XXX_RAISETYPEBROWSER
Command8=ID_XXX_RAISEMETHODBROWSER
Command9=ID_XXX_RAISEWORKSPACEBROWSER
CommandCount=9

