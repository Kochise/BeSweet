/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Jul 06 22:40:24 2003
 */
/* Compiler settings for D:\home\marco\cpp\EclipseStyleBrowsers\ESBServer\src\RemoteInterface\COM\ESBServer.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __IESBServer_h__
#define __IESBServer_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IESBServer_FWD_DEFINED__
#define __IESBServer_FWD_DEFINED__
typedef interface IESBServer IESBServer;
#endif 	/* __IESBServer_FWD_DEFINED__ */


#ifndef __IESBServerManager_FWD_DEFINED__
#define __IESBServerManager_FWD_DEFINED__
typedef interface IESBServerManager IESBServerManager;
#endif 	/* __IESBServerManager_FWD_DEFINED__ */


#ifndef __ESBServerCoClass_FWD_DEFINED__
#define __ESBServerCoClass_FWD_DEFINED__

#ifdef __cplusplus
typedef class ESBServerCoClass ESBServerCoClass;
#else
typedef struct ESBServerCoClass ESBServerCoClass;
#endif /* __cplusplus */

#endif 	/* __ESBServerCoClass_FWD_DEFINED__ */


#ifndef __ESBServerManagerCoClass_FWD_DEFINED__
#define __ESBServerManagerCoClass_FWD_DEFINED__

#ifdef __cplusplus
typedef class ESBServerManagerCoClass ESBServerManagerCoClass;
#else
typedef struct ESBServerManagerCoClass ESBServerManagerCoClass;
#endif /* __cplusplus */

#endif 	/* __ESBServerManagerCoClass_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "ESBDSAddin.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IESBServer_INTERFACE_DEFINED__
#define __IESBServer_INTERFACE_DEFINED__

/* interface IESBServer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IESBServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("05E470CC-BE7D-434E-9890-1226E9682FB9")
    IESBServer : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE fileContentChanged( 
            BSTR file) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE showFileOutline( 
            BSTR file) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE loadWorkspace( 
            BSTR packages) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE showBrowser( 
            BSTR browser,
            BSTR what) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IESBServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IESBServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IESBServer __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IESBServer __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IESBServer __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IESBServer __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IESBServer __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IESBServer __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *fileContentChanged )( 
            IESBServer __RPC_FAR * This,
            BSTR file);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *showFileOutline )( 
            IESBServer __RPC_FAR * This,
            BSTR file);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadWorkspace )( 
            IESBServer __RPC_FAR * This,
            BSTR packages);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *showBrowser )( 
            IESBServer __RPC_FAR * This,
            BSTR browser,
            BSTR what);
        
        END_INTERFACE
    } IESBServerVtbl;

    interface IESBServer
    {
        CONST_VTBL struct IESBServerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IESBServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IESBServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IESBServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IESBServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IESBServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IESBServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IESBServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IESBServer_fileContentChanged(This,file)	\
    (This)->lpVtbl -> fileContentChanged(This,file)

#define IESBServer_showFileOutline(This,file)	\
    (This)->lpVtbl -> showFileOutline(This,file)

#define IESBServer_loadWorkspace(This,packages)	\
    (This)->lpVtbl -> loadWorkspace(This,packages)

#define IESBServer_showBrowser(This,browser,what)	\
    (This)->lpVtbl -> showBrowser(This,browser,what)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServer_fileContentChanged_Proxy( 
    IESBServer __RPC_FAR * This,
    BSTR file);


void __RPC_STUB IESBServer_fileContentChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServer_showFileOutline_Proxy( 
    IESBServer __RPC_FAR * This,
    BSTR file);


void __RPC_STUB IESBServer_showFileOutline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServer_loadWorkspace_Proxy( 
    IESBServer __RPC_FAR * This,
    BSTR packages);


void __RPC_STUB IESBServer_loadWorkspace_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServer_showBrowser_Proxy( 
    IESBServer __RPC_FAR * This,
    BSTR browser,
    BSTR what);


void __RPC_STUB IESBServer_showBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IESBServer_INTERFACE_DEFINED__ */


#ifndef __IESBServerManager_INTERFACE_DEFINED__
#define __IESBServerManager_INTERFACE_DEFINED__

/* interface IESBServerManager */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IESBServerManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9BC2C2AE-1571-411A-A6EF-6D926A538424")
    IESBServerManager : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE newWorkspaceServer( 
            BSTR name,
            IESBDSAddin __RPC_FAR *vs,
            IESBServer __RPC_FAR *__RPC_FAR *srv) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getWorkspaceServer( 
            BSTR name,
            IESBServer __RPC_FAR *__RPC_FAR *srv) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE removeWorkspaceServer( 
            BSTR name) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IESBServerManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IESBServerManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IESBServerManager __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IESBServerManager __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IESBServerManager __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IESBServerManager __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IESBServerManager __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IESBServerManager __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *newWorkspaceServer )( 
            IESBServerManager __RPC_FAR * This,
            BSTR name,
            IESBDSAddin __RPC_FAR *vs,
            IESBServer __RPC_FAR *__RPC_FAR *srv);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getWorkspaceServer )( 
            IESBServerManager __RPC_FAR * This,
            BSTR name,
            IESBServer __RPC_FAR *__RPC_FAR *srv);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *removeWorkspaceServer )( 
            IESBServerManager __RPC_FAR * This,
            BSTR name);
        
        END_INTERFACE
    } IESBServerManagerVtbl;

    interface IESBServerManager
    {
        CONST_VTBL struct IESBServerManagerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IESBServerManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IESBServerManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IESBServerManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IESBServerManager_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IESBServerManager_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IESBServerManager_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IESBServerManager_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IESBServerManager_newWorkspaceServer(This,name,vs,srv)	\
    (This)->lpVtbl -> newWorkspaceServer(This,name,vs,srv)

#define IESBServerManager_getWorkspaceServer(This,name,srv)	\
    (This)->lpVtbl -> getWorkspaceServer(This,name,srv)

#define IESBServerManager_removeWorkspaceServer(This,name)	\
    (This)->lpVtbl -> removeWorkspaceServer(This,name)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServerManager_newWorkspaceServer_Proxy( 
    IESBServerManager __RPC_FAR * This,
    BSTR name,
    IESBDSAddin __RPC_FAR *vs,
    IESBServer __RPC_FAR *__RPC_FAR *srv);


void __RPC_STUB IESBServerManager_newWorkspaceServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServerManager_getWorkspaceServer_Proxy( 
    IESBServerManager __RPC_FAR * This,
    BSTR name,
    IESBServer __RPC_FAR *__RPC_FAR *srv);


void __RPC_STUB IESBServerManager_getWorkspaceServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IESBServerManager_removeWorkspaceServer_Proxy( 
    IESBServerManager __RPC_FAR * This,
    BSTR name);


void __RPC_STUB IESBServerManager_removeWorkspaceServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IESBServerManager_INTERFACE_DEFINED__ */



#ifndef __ESBServerLib_LIBRARY_DEFINED__
#define __ESBServerLib_LIBRARY_DEFINED__

/* library ESBServerLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ESBServerLib;

EXTERN_C const CLSID CLSID_ESBServerCoClass;

#ifdef __cplusplus

class DECLSPEC_UUID("A40D669B-A4F9-4A10-B93F-D240B7A12B67")
ESBServerCoClass;
#endif

EXTERN_C const CLSID CLSID_ESBServerManagerCoClass;

#ifdef __cplusplus

class DECLSPEC_UUID("1F535CB1-E631-4317-A23B-9402B9D2A383")
ESBServerManagerCoClass;
#endif
#endif /* __ESBServerLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
