#ifndef ObjectModelVC7_H
#define ObjectModelVC7_H

#if _MSC_VER > 1200  // VC7
    //The following #import imports the IDTExtensibility2 interface based on it's LIBID
    #import "libid:AC0714F2-3D04-11D1-AE7D-00A0C90F26F4" version("1.0") lcid("0")  raw_interfaces_only named_guids
    //The following #import imports MSO based on it's LIBID
    #import "libid:2DF8D04C-5BFA-101B-BDE5-00AA0044DE52" version("2.2") lcid("0") raw_interfaces_only named_guids
    //The following #import imports DTE based on it's LIBID
    #import "libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2" version("7.0") lcid("0") raw_interfaces_only named_guids
#else
    #include <ObjectModelVC7/VSObjectModel/MSADDNDR.tlh>
    #include <ObjectModelVC7/VSObjectModel/MSO.tlh>
    #include <ObjectModelVC7/VSObjectModel/dte.tlh>
#endif

DEFINE_GUID(CLSID_Connect,0x90AC5CE1,0xCCE9,0x461B,0xA6,0x5D,0x5E,0xC1,0x3F,0xEA,0x49,0x7E);

#endif