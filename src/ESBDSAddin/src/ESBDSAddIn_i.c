/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jun 30 23:26:43 2003
 */
/* Compiler settings for D:\home\marco\cpp\EclipseStyleBrowsers\ESBDSAddin\src\ESBDSAddin.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IESBDSAddin = {0x524A9255,0xA05C,0x4DBC,{0xA5,0xC8,0xF2,0x94,0x83,0xBF,0xA3,0xF4}};


const IID IID_ICommands = {0x18B53036,0x6F47,0x4F97,{0x8A,0x40,0xFD,0xA1,0x79,0x71,0xA6,0xA4}};


const IID LIBID_ESBDSADDINLib = {0xDBAA9264,0x5569,0x4AF1,{0x9F,0xE1,0x22,0x14,0xBC,0xC4,0x32,0x71}};


const CLSID CLSID_ESBDSAddin = {0x5EEA1EDA,0x4D17,0x42F4,{0xA9,0xBA,0x63,0x65,0x7F,0xAF,0x60,0xCA}};


const CLSID CLSID_Commands = {0xB67FD143,0x30F5,0x4C1C,{0x84,0xCC,0x64,0xCB,0xE1,0xAB,0x35,0x0F}};


const CLSID CLSID_ApplicationEvents = {0x97618E7E,0xF082,0x4BBD,{0x84,0xCC,0xCB,0x46,0x72,0x90,0x75,0x3A}};


const CLSID CLSID_DebuggerEvents = {0xBFD7E734,0x695C,0x4092,{0x95,0x66,0x52,0xB5,0x79,0x17,0xBD,0x60}};


#ifdef __cplusplus
}
#endif

