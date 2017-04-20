/**
/*   Copyright (c) 2003by  Marco Welti
/*
/*   This document is  bound by the  QT Public License
/*   (http://www.trolltech.com/licenses/qpl.html).
/*   See License.txt for more information.
/*
/*
/*
/*   ALL RIGHTS RESERVED.  
/* 
/*   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
/*   OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/*   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/*   ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/*   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/*   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
/*   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/*   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
/*   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/*   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/*   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/* 
/***********************************************************************/

#include "stdafx.h"

#include "Stacktrace.h"

#include <sstream>
#include <iterator>

using namespace std;

void printStacktrace(EXCEPTION_POINTERS *ep, std::list<StackEntry> &s);

///////////////////////// SEHTranslator /////////////////////////////////
SEHTranslator::SEHTranslator()
: mOldHandler(_set_se_translator(translate)) 
{
}

SEHTranslator::~SEHTranslator()
{
  if(mOldHandler) { 
    _set_se_translator(mOldHandler); 
  }
}

void SEHTranslator::translate(UINT id ,EXCEPTION_POINTERS *ep)
{
  switch(id) {
    //case EXCEPTION_NO_MEMORY:					throw no_memory(pexp);				break;
    case EXCEPTION_ACCESS_VIOLATION:	ep->ExceptionRecord->ExceptionInformation [0] ? throw ReadAccessViolationException(ep) : throw WriteAccessViolationException(ep);	break;

    case EXCEPTION_DATATYPE_MISALIGNMENT:		throw DatatypeMisalignementException(ep);	break;
    case EXCEPTION_BREAKPOINT:					    throw BreakpointException(ep);				break;
    case EXCEPTION_SINGLE_STEP:					    throw SingleStepException(ep);			break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		throw ArrayBoundsException(ep);	break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:		throw FloatingPointException::DenormalOperandException(ep);	break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:			throw FloatingPointException::DivideByZeroException(ep);		break;
    case EXCEPTION_FLT_INEXACT_RESULT:			throw FloatingPointException::InexactResultException(ep);		break;
    case EXCEPTION_FLT_INVALID_OPERATION:		throw FloatingPointException::InvalidOperationException(ep);	break;
    case EXCEPTION_FLT_OVERFLOW:				    throw FloatingPointException::OverflowException(ep);			break;
    case EXCEPTION_FLT_STACK_CHECK:				  throw FloatingPointException::StackCheckException(ep);		break;
    case EXCEPTION_FLT_UNDERFLOW:				    throw FloatingPointException::UnderflowException(ep);			break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:			throw IntegerException::DivideByZeroException(ep);		break;
    case EXCEPTION_INT_OVERFLOW:				    throw IntegerException::OverflowException(ep);			break;
    case EXCEPTION_PRIV_INSTRUCTION:			  throw PrivilegedInstructionException(ep);		break;
    case EXCEPTION_IN_PAGE_ERROR:				    throw InPageException(ep);			break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:			throw IllegalInstructionException(ep);	break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:throw NoncontinuableException(ep); break;
    case EXCEPTION_STACK_OVERFLOW:				  throw StackOverflowException(ep);			break;
    case EXCEPTION_INVALID_DISPOSITION:			throw InvalidDispositionException(ep);	break;
    case EXCEPTION_GUARD_PAGE:					    throw GuardPageException(ep);				break;
    case EXCEPTION_INVALID_HANDLE:				  throw InvalidHandleException(ep);			break;
  }
  
  throw SystemException(id, ep);
}

std::string SEHTranslator::name(DWORD code)
{
	switch (code)
	{
	//case EXCEPTION_NO_MEMORY:					return "No Memory";
	case EXCEPTION_ACCESS_VIOLATION :			return "Access Violation";
	case EXCEPTION_DATATYPE_MISALIGNMENT :		return "Datatype Misalignment";
	case EXCEPTION_BREAKPOINT :					return "Breakpoint";
	case EXCEPTION_SINGLE_STEP :				return "Single Step";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :		return "Array Bounds Exceeded";
	case EXCEPTION_FLT_DENORMAL_OPERAND :		return "Float Denormal Operand";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO :			return "Float Divide by Zero";
	case EXCEPTION_FLT_INEXACT_RESULT :			return "Float Inexact Result";
	case EXCEPTION_FLT_INVALID_OPERATION :		return "Float Invalid Operation";
	case EXCEPTION_FLT_OVERFLOW :				return "Float Overflow";
	case EXCEPTION_FLT_STACK_CHECK :			return "Float Stack Check";
	case EXCEPTION_FLT_UNDERFLOW :				return "Float Underflow";
	case EXCEPTION_INT_DIVIDE_BY_ZERO :			return "Integer Divide by Zero";
	case EXCEPTION_INT_OVERFLOW :				return "Integer Overflow";
	case EXCEPTION_PRIV_INSTRUCTION :			return "Privileged Instruction";
	case EXCEPTION_IN_PAGE_ERROR :				return "In Page Error";
	case EXCEPTION_ILLEGAL_INSTRUCTION :		return "Illegal Instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION :	return "Noncontinuable Exception";
	case EXCEPTION_STACK_OVERFLOW :				return "Stack Overflow";
	case EXCEPTION_INVALID_DISPOSITION :		return "Invalid Disposition";
	case EXCEPTION_GUARD_PAGE :					return "Guard Page";
	case EXCEPTION_INVALID_HANDLE :				return "Invalid Handle";
	case 0xE06D7363 :							return "Microsoft C++ Exception";
	default :		
    {
         stringstream text;
         text << "Unknown SEH-Exception" << code;
         return text.str();

    }
	};		
}

std::string SEHTranslator::description(DWORD code)
{
	switch (code)
	{
	//case EXCEPTION_NO_MEMORY:					return "The allocation attempt failed because of a lack of available memory or heap corruption.";
	case EXCEPTION_ACCESS_VIOLATION :			return "The thread attempted to read from or write to a virtual address for which it does not have the appropriate access";
	case EXCEPTION_DATATYPE_MISALIGNMENT :		return "The thread attempted to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries, 32-bit values on 4-byte boundaries, and so on";
	case EXCEPTION_BREAKPOINT :					return "A breakpoint was encountered";
	case EXCEPTION_SINGLE_STEP :				return "A trace trap or other single-instruction mechanism signaled that one instruction has been executed";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :		return "The thread attempted to access an array element that is out of bounds, and the underlying hardware supports bounds checking";
	case EXCEPTION_FLT_DENORMAL_OPERAND :		return "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO :			return "The thread attempted to divide a floating-point value by a floating-point divisor of zero";
	case EXCEPTION_FLT_INEXACT_RESULT :			return "The result of a floating-point operation cannot be represented exactly as a decimal fraction";
	case EXCEPTION_FLT_INVALID_OPERATION :		return "This exception represents any floating-point exception not included in this list";
	case EXCEPTION_FLT_OVERFLOW :				return "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type";
	case EXCEPTION_FLT_STACK_CHECK :			return "The stack overflowed or underflowed as the result of a floating-point operation";
	case EXCEPTION_FLT_UNDERFLOW :				return "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type";
	case EXCEPTION_INT_DIVIDE_BY_ZERO :			return "The thread attempted to divide an integer value by an integer divisor of zero";
	case EXCEPTION_INT_OVERFLOW :				return "The result of an integer operation caused a carry out of the most significant bit of the result";
	case EXCEPTION_PRIV_INSTRUCTION :			return "The thread attempted to execute an instruction whose operation is not allowed in the current machine mode";
	case EXCEPTION_IN_PAGE_ERROR :				return "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network";
	case EXCEPTION_ILLEGAL_INSTRUCTION :		return "The thread tried to execute an invalid instruction";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION :	return "The thread attempted to continue execution after a noncontinuable exception occurred";
	case EXCEPTION_STACK_OVERFLOW :				return "The thread used up its stack";
	case EXCEPTION_INVALID_DISPOSITION :		return "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception";
	case EXCEPTION_GUARD_PAGE :					return "Guard Page Exception";
	case EXCEPTION_INVALID_HANDLE :				return "Invalid Handle Exception";
	case 0xE06D7363 :							return "Microsoft C++ Exception";
	default :		
    {
         stringstream text;
         text << "Unknown SEH-Exception" << code;
         return text.str();

    }
	};		
}

///////////////////////////// Runtime stack ///////////////////////////////
RuntimeStack::RuntimeStack()
{
  gatherStack();
}

RuntimeStack::RuntimeStack(EXCEPTION_POINTERS *ep) 
{
  storeStack(ep);
}

RuntimeStack::~RuntimeStack()
{
}

ostream& operator<< (ostream& os, const RuntimeStack &stack)
{
  return stack.print(os);
}

ostream& operator<< (ostream& os, const StackEntry &entry)
{
  return os << entry.file << "(" << entry.line << "): " << entry.module << " " << entry.symbol;
}

std::ostream & RuntimeStack::print(std::ostream &os) const
{
  copy(mStack.begin(), mStack.end(), ostream_iterator<StackEntry>(os, "\n"));
  return os;
}

void RuntimeStack::gatherStack()
{
  __try {
    RaiseException(0,0,0,0);  
  } __except(storeStack(GetExceptionInformation())) { }
}

int RuntimeStack::storeStack(EXCEPTION_POINTERS *ep)
{
  //std::stringstream stacktrace;
  printStacktrace(ep, mStack);
  
  //mStack = stacktrace.str();
  return EXCEPTION_CONTINUE_EXECUTION;//EXCEPTION_EXECUTE_HANDLER;
}

/////////////////////////////// TraceableException /////////////////////
TraceableException::TraceableException(const std::string &what)
: std::runtime_error(what) 
{
}

TraceableException::TraceableException(const std::string &what, EXCEPTION_POINTERS *ep)
: std::runtime_error(what), mStack(ep) 
{
}

const char* TraceableException::what() const
{
  std::stringstream text;
  text << runtime_error::what() << std::endl << mStack << std::endl;
  mText = text.str();
  return mText.c_str();
}

/////////////////////////////// SystemException /////////////////////
SystemException::SystemException(DWORD id, EXCEPTION_POINTERS *ep) 
: TraceableException(SEHTranslator::name(id), ep) , mId(id)
{
}
  
const char* SystemException::what() const
{
  std::stringstream text;
  text << SEHTranslator::name(mId) << endl << SEHTranslator::description(mId) << endl << getStackTrace();
  mText = text.str();
  return getText().c_str();
}



void bang()
{
  //throw Traceable("hello");
  int i = 0;
  int x = 1/i;
}

void main()
{
  SEHTranslator translator;
  try {
    bang();

     //RuntimeStack stack;
    //stack.printStackTrace(std::cout);

  } catch(std::exception &e) {
    std::cout << "caught exception..." << std::endl;
    std::cout << /*typeid(e).name() << ":" <<*/ e.what() << std::endl;
  }
}


///////////////////////////////////////////
/////////////////////////////////////////
/*
 Copyright (c) 2001 - 2002
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 
 Created: 25.12.2001 19:41:00
 Version: 1.0.2

 Permission to use, copy, modify, distribute and sell this software
 and its documentation for any purpose is hereby granted without fee,
 provided that the above copyright notice appear in all copies and
 that both that copyright notice and this permission notice appear
 in supporting documentation.  Konstantin Boukreev makes no representations
 about the suitability of this software for any purpose.
 It is provided "as is" without express or implied warranty.
 
 sym_engine class incapsulate Symbol Handler and Debugging Service API
	
 the list of used API:
	SymInitialize, SymCleanup, SymGetLineFromAddr, SymGetModuleBase,
	SymGetSymbolInfo, SymGetSymFromAddr, SymGetSymFromName, SymGetSymNext,
	SymLoadModule, SymSetOptions
	StackWalk

 based on code\ideas from John Robbins's book "Debugging application"
 http://www.wintellect.com/robbins
*/

#ifndef _sym_engine_e4b31bc5_8e01_4cda_b5a4_905dde52ac01
#define _sym_engine_e4b31bc5_8e01_4cda_b5a4_905dde52ac01

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN64
#error Win64 is not supported
#endif

#include <imagehlp.h>
#include <ostream>

extern "C" {

WINBASEAPI BOOL WINAPI SwitchToThread(VOID);

typedef DWORD (WINAPI *SymGetModuleBaseFn)( IN HANDLE hProcess, IN DWORD dwAddr );
typedef BOOL  (WINAPI *SymGetSymFromAddrFn)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_SYMBOL Symbol );
typedef BOOL  (WINAPI *StackWalkFn)( DWORD MachineType, HANDLE hProcess, HANDLE hThread, LPSTACKFRAME StackFrame, PVOID ContextRecord, PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine, PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine, PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, PTRANSLATE_ADDRESS_ROUTINE TranslateAddress );
typedef PVOID (WINAPI *SymFunctionTableAccessFn)( HANDLE hProcess, DWORD AddrBase );
typedef BOOL  (WINAPI *SymGetLineFromAddrFn)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE Line );
typedef BOOL  (WINAPI *SymCleanupFn)( IN HANDLE hProcess );
typedef BOOL  (WINAPI *SymInitializeFn)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
typedef DWORD (WINAPI *SymSetOptionsFn)( IN DWORD SymOptions );
typedef DWORD (WINAPI *SymGetOptionsFn)( VOID );
typedef DWORD (WINAPI *SymLoadModuleFn)( IN HANDLE hProcess, IN HANDLE hFile,IN PSTR ImageName, IN PSTR ModuleName, IN DWORD BaseOfDll, IN DWORD SizeOfDll );

typedef BOOL  (WINAPI *SymGetModuleInfoFn)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PIMAGEHLP_MODULE ModuleInfo );
typedef DWORD (WINAPI *UnDecorateSymbolNameFn)( PCSTR DecoratedName, PSTR UnDecoratedName, DWORD UndecoratedLength, DWORD Flags );

}

class debug_api
{
private:
   HMODULE mLib;
   std::string mPDBPath;

   SymGetModuleBaseFn        GetModuleBase;
   SymGetSymFromAddrFn       GetSymbolFromAddr;
   StackWalkFn               MyStackWalk;
   SymFunctionTableAccessFn  FunctionTableAccess;
   SymGetLineFromAddrFn      GetLineFromAddr;
   SymCleanupFn              Cleanup;
   SymInitializeFn           Initialize;
   SymSetOptionsFn           SetOptions;
   SymGetOptionsFn           GetOptions;
   SymLoadModuleFn           LoadModule;

public:
  static debug_api& instance() 
  {
    static debug_api theApi("dbghelp.dll");//theApi("imagehlp.dll");
    return theApi;
  }

  HMODULE get_module_base(HANDLE process, DWORD addr) 
  { return (HMODULE)GetModuleBase(process, addr); };

  bool get_symbol(HANDLE process, DWORD addr, PDWORD displacement, PIMAGEHLP_SYMBOL symbol) 
  { return GetSymbolFromAddr( process,  addr,  displacement,  symbol) == TRUE; }

  bool get_line(HANDLE process, DWORD addr, PDWORD displacement, PIMAGEHLP_LINE line) 
  { return GetLineFromAddr(process,  addr,  displacement,  line) == TRUE; }

  bool stack_walk(HANDLE process,  HANDLE thread, LPSTACKFRAME frame, PVOID context, PREAD_PROCESS_MEMORY_ROUTINE readMemoryRoutine)
  { return MyStackWalk(IMAGE_FILE_MACHINE_I386, process, thread, frame, context, readMemoryRoutine, FunctionTableAccess, GetModuleBase, 0) == TRUE; }
  
  DWORD set_options(DWORD options)
  { return SetOptions(options); }
  
  DWORD get_options()
  { return GetOptions(); }

  bool initialize(HANDLE process, const char* userSearchPath, bool invadeProcess)
  { return Initialize(process, (char*)((/*userSearchPath+*/getPDBSearchPath()).c_str()), invadeProcess) == TRUE; }

  bool cleanup(HANDLE process)
  { return Cleanup(process) == TRUE; }

  DWORD load_module(HANDLE process, HANDLE file , const char * image, const char * module, DWORD baseAddr, DWORD size)
  { return LoadModule(process, file , (char*)image, (char*)module, baseAddr, size); }

  ~debug_api() 
  { /*if(mLib != NULL) FreeLibrary(mLib); mLib = 0;*/ }

private:
  debug_api(const std::string &libname) 
  {
    
    mLib = LoadLibrary(libname.c_str());
   
    if ( mLib == NULL )
    {
      MessageBox(0, ("failed to load " + libname).c_str(), "Error", MB_OK);
      exit(0);
    }

    Cleanup = (SymCleanupFn) GetProcAddress( mLib, "SymCleanup" );
    FunctionTableAccess = (SymFunctionTableAccessFn) GetProcAddress( mLib, "SymFunctionTableAccess" );
    GetLineFromAddr = (SymGetLineFromAddrFn) GetProcAddress( mLib, "SymGetLineFromAddr" );
    GetModuleBase = (SymGetModuleBaseFn) GetProcAddress( mLib, "SymGetModuleBase" );
    //pSGMI = (tSGMI) GetProcAddress( lib, "SymGetModuleInfo" );
    GetOptions = (SymGetOptionsFn) GetProcAddress( mLib, "SymGetOptions" );
    GetSymbolFromAddr = (SymGetSymFromAddrFn) GetProcAddress( mLib, "SymGetSymFromAddr" );
    Initialize = (SymInitializeFn) GetProcAddress( mLib, "SymInitialize" );
    SetOptions = (SymSetOptionsFn) GetProcAddress( mLib, "SymSetOptions" );
    MyStackWalk = (StackWalkFn) GetProcAddress( mLib, "StackWalk" );
    //pUDSN = (tUDSN) GetProcAddress( lib, "UnDecorateSymbolName" );
    LoadModule = (SymLoadModuleFn) GetProcAddress( mLib, "SymLoadModule" );

    if ( Cleanup == NULL || FunctionTableAccess == NULL || GetLineFromAddr == NULL || GetModuleBase == NULL ||
      GetOptions == NULL || GetSymbolFromAddr == NULL || Initialize == NULL || SetOptions == NULL ||
      MyStackWalk == NULL || LoadModule == NULL /*|| pSLM == NULL*/ )
    {
      MessageBox(0, ("not all required functions found in " + libname).c_str(), "Error", MB_OK);
      FreeLibrary( mLib );
      mLib = 0;
      exit(0);
    }
  }

  string getPDBSearchPath() 
  {
    if(mPDBPath.empty()) {
      initializePDBPAth();
    }
    return mPDBPath;
  }

  void initializePDBPAth()
  {
    const size_t size = 8192;
    char buffer[size+1]; //8k should (hopefuly) be enough
    stringstream path;

    // current directory
    if (GetCurrentDirectoryA( size, buffer)) {
      path << buffer;
    }
  
    // dir with executable
    if (GetModuleFileNameA(0, buffer, size))
    {
      string tmp(buffer);
      size_t pos = tmp.find_last_of("\\/");
      if(pos != string::npos) {
        path << ";" << tmp.substr(0, pos);
      }
    }

    // environment variable _NT_SYMBOL_PATH
    const size_t envCnt = 3;
    char *env[envCnt] = {"_NT_SYMBOL_PATH", "_NT_ALTERNATE_SYMBOL_PATH", "SYSTEMROOT"};
    for(int i = 0; i < envCnt; i++) {
      if (GetEnvironmentVariableA( env[i], buffer, size)) {
        path << ";" << buffer;
      }
    }

    mPDBPath = path.str();
  }
};


//HMODULE debug_api::mLib = NULL;

class sym_engine
{
 public:
	sym_engine (unsigned);
	~sym_engine();

	void address(unsigned a)		{ m_address = a; }
	unsigned address(void) const	{ return m_address; }
	
	// symbol handler queries
	unsigned module  (char *, unsigned);
	unsigned symbol  (char *, unsigned, unsigned * = 0);
	unsigned fileline(char *, unsigned, unsigned *, unsigned * = 0);

	// stack walk
	bool stack_first (CONTEXT* pctx);
	bool stack_next  ();

	/*
		format argument
		%m  : module
		%f  : file
		%l  : line
		%ld : line's displacement
		%s  : symbol
		%sd : symbol's displacement
		
		for example	
		"%f(%l) : %m at %s\n"
		"%m.%s + %sd bytes, in %f:line %l\n"
	*/	
	static bool stack_trace(list<StackEntry>&, CONTEXT *, unsigned skip = 0, const char * fmt = default_fmt());
	static bool stack_trace(list<StackEntry>&, unsigned skip = 1, const char * fmt = default_fmt()); 	
	static bool stack_trace(list<StackEntry>&, sym_engine&, CONTEXT *, unsigned skip = 1, const char * fmt = default_fmt());	

 private:
	static const char * default_fmt() { return "%f(%l) : %m at %s\n"; }	
	static bool get_line_from_addr (HANDLE, unsigned, unsigned *, IMAGEHLP_LINE *);
	static unsigned get_module_basename (HMODULE, char *, unsigned);
	
	bool check();

 private:
	unsigned		m_address;
	bool			m_ok;
	STACKFRAME *	m_pframe;
	CONTEXT *		m_pctx;

 private:
	class guard
	{	
	 private:
		guard();
	 public:
		~guard();
		bool init();		
		bool fail() const { return m_ref == -1; }
		static guard & instance() 
		{
			static guard g; 
			return g;
		}
	private:		
		void clear();
		bool load_module(HANDLE, HMODULE);
		int  m_ref;
	};
};

#endif //_sym_engine_e4b31bc5_8e01_4cda_b5a4_905dde52ac01

void printStacktrace(EXCEPTION_POINTERS *ep, std::list<StackEntry> &s)
{
  sym_engine::stack_trace(s, ep->ContextRecord);
}

/*
 Copyright (c) 2001 - 2002
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 
 Created: 25.12.2001 19:41:07
 Version: 1.0.2

 Permission to use, copy, modify, distribute and sell this software
 and its documentation for any purpose is hereby granted without fee,
 provided that the above copyright notice appear in all copies and
 that both that copyright notice and this permission notice appear
 in supporting documentation.  Konstantin Boukreev makes no representations
 about the suitability of this software for any purpose.
 It is provided "as is" without express or implied warranty.

*/

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif // _WIN32_WINNT

#include <winbase.h>
#include <crtdbg.h>
#include <malloc.h> 
#include <tlhelp32.h>


//#pragma comment (lib, "dbghelp")

#ifdef VERIFY
#undef VERIFY
#endif // VERIFY

#ifdef _DEBUG
#define VERIFY(x) _ASSERTE(x)
#else
#define VERIFY(x) (x)
#endif //_DEBUG

#define WORK_AROUND_SRCLINE_BUG

#ifdef _DEBUG
//#if 1
// #define SYM_ENGINE_TRACE_SPIN_COUNT
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////

bool IsNT()
{	
	#if 1
	OSVERSIONINFO vi = { sizeof(vi)};	
	::GetVersionEx(&vi);
	return vi.dwPlatformId == VER_PLATFORM_WIN32_NT;
	#else
	return false;
	#endif
}

HANDLE SymGetProcessHandle()
{
	if (IsNT())
//	if (0)
		return GetCurrentProcess();
	else
		return (HANDLE)GetCurrentProcessId();
}

BOOL __stdcall My_ReadProcessMemory (HANDLE, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead)
{
    return ReadProcessMemory(GetCurrentProcess(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

///////////////////////////////////////////////////////////////////////

sym_engine::sym_engine (unsigned address) 
	: m_address(address), m_ok(false), m_pframe(0)
{
}

sym_engine::~sym_engine()
{
//	if (m_ok) guard::instance().clear();
	delete m_pframe;
}

unsigned sym_engine::module(char * buf, unsigned len)
{
	if (!len || !buf || IsBadWritePtr(buf, len))
		return 0;

	if (!check())
		return 0;

	HANDLE hProc = SymGetProcessHandle();
  HMODULE hMod = debug_api::instance().get_module_base (hProc, m_address);
	if (!hMod) return 0;
	return get_module_basename(hMod, buf, len);	
}

unsigned sym_engine::symbol(char * buf, unsigned len, unsigned * pdisplacement)
{
	if (!len || !buf || 
		IsBadWritePtr(buf, len) ||
		(pdisplacement && IsBadWritePtr(pdisplacement, sizeof(unsigned))))
		return 0;

	if (!check())
		return 0;

	BYTE symbol [ 512 ] ;
	PIMAGEHLP_SYMBOL pSym = (PIMAGEHLP_SYMBOL)&symbol;
	memset(pSym, 0, sizeof(symbol)) ;
	pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) ;
    pSym->MaxNameLength = sizeof(symbol) - sizeof(IMAGEHLP_SYMBOL);

	HANDLE hProc = SymGetProcessHandle();
	DWORD displacement = 0;
  int r = debug_api::instance().get_symbol(hProc, m_address, &displacement, pSym);
	if (!r) return 0;
	if (pdisplacement) 
		*pdisplacement = displacement;

	r = _snprintf(buf, len, "%s()", pSym->Name);
    
	r = r == -1 ? len - 1 : r;
	buf[r] = 0;	
	return r;
}

unsigned sym_engine::fileline (char * buf, unsigned len, unsigned * pline, unsigned * pdisplacement)
{
	if (!len || !buf || 
		IsBadWritePtr(buf, len) || 
		(pline && IsBadWritePtr(pline, sizeof(unsigned))) || 
		(pdisplacement && IsBadWritePtr(pdisplacement, sizeof(unsigned))))
		return 0;

	if (!check())
		return 0;

	IMAGEHLP_LINE img_line;
	memset(&img_line, 0, sizeof(IMAGEHLP_LINE));
	img_line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

	HANDLE hProc = SymGetProcessHandle();
	unsigned displacement = 0;
	if (!get_line_from_addr(hProc, m_address, &displacement, &img_line))
		return 0;
	if (pdisplacement) 
		*pdisplacement = displacement;
	if (pline) 
		*pline = img_line.LineNumber;
	lstrcpynA(buf, img_line.FileName, len);
	return lstrlenA(buf);
}

bool sym_engine::stack_first (CONTEXT* pctx)
{
	if (!pctx || IsBadReadPtr(pctx, sizeof(CONTEXT))) 
		return false;

	if (!check())
		return false;

	if (!m_pframe) 
	{
		m_pframe = new STACKFRAME;
		if (!m_pframe) return false;
	}
				
	memset(m_pframe, 0, sizeof(STACKFRAME));

    #ifdef _X86_
    m_pframe->AddrPC.Offset       = pctx->Eip;
    m_pframe->AddrPC.Mode         = AddrModeFlat;
    m_pframe->AddrStack.Offset    = pctx->Esp;
    m_pframe->AddrStack.Mode      = AddrModeFlat;
    m_pframe->AddrFrame.Offset    = pctx->Ebp;
    m_pframe->AddrFrame.Mode      = AddrModeFlat;
    #else
    m_pframe->AddrPC.Offset       = (DWORD)pctx->Fir;
    m_pframe->AddrPC.Mode         = AddrModeFlat;
    m_pframe->AddrReturn.Offset   = (DWORD)pctx->IntRa;
    m_pframe->AddrReturn.Mode     = AddrModeFlat;
    m_pframe->AddrStack.Offset    = (DWORD)pctx->IntSp;
    m_pframe->AddrStack.Mode      = AddrModeFlat;
    m_pframe->AddrFrame.Offset    = (DWORD)pctx->IntFp;
    m_pframe->AddrFrame.Mode      = AddrModeFlat;
    #endif

	m_pctx = pctx;
	return stack_next();
}

bool sym_engine::stack_next  ()
{
	if (!m_pframe || !m_pctx) 
	{		
		_ASSERTE(0);
		return false;
	}
		
	if (!m_ok)
	{
		_ASSERTE(0);
		return false;
	}
	
	SetLastError(0);
	HANDLE hProc = SymGetProcessHandle();
  BOOL r = debug_api::instance().stack_walk (
				hProc, 
				GetCurrentThread(), 
				m_pframe, 
				m_pctx,
        (PREAD_PROCESS_MEMORY_ROUTINE)My_ReadProcessMemory);

	if (!r || 
		!m_pframe->AddrFrame.Offset)
	{		
		return false;
	}		

	// "Debugging Applications" John Robbins
	// Before I get too carried away and start calculating
	// everything, I need to double-check that the address returned
	// by StackWalk really exists. I've seen cases in which
	// StackWalk returns TRUE but the address doesn't belong to
	// a module in the process.
  DWORD dwModBase = (DWORD)debug_api::instance().get_module_base(hProc, m_pframe->AddrPC.Offset);
	if (!dwModBase) 
	{	
		//_ASSERTE(0);
		return false;
	}

	address(m_pframe->AddrPC.Offset);
	return true;
}

bool sym_engine::get_line_from_addr (HANDLE hProc, unsigned addr, unsigned * pdisplacement, IMAGEHLP_LINE * pLine)
{	 
	#ifdef WORK_AROUND_SRCLINE_BUG

	// "Debugging Applications" John Robbins
    // The problem is that the symbol engine finds only those source
    // line addresses (after the first lookup) that fall exactly on
    // a zero displacement. I'll walk backward 100 bytes to
    // find the line and return the proper displacement.
    DWORD displacement = 0 ;
    while (!debug_api::instance().get_line(hProc, addr - displacement, (PDWORD)pdisplacement, pLine))
    {        
        if (100 == ++displacement)
            return false;        
    }

	// "Debugging Applications" John Robbins
    // I found the line, and the source line information is correct, so
    // change the displacement if I had to search backward to find the source line.
    if (displacement)    
        *pdisplacement = displacement;    
    return true;

	#else 
    return 0 != GetLineFromAddr (hProc, addr, (DWORD *) pdisplacement, pLine);
	#endif
}

unsigned sym_engine::get_module_basename (HMODULE hMod, char * buf, unsigned len)
{
	char filename[MAX_PATH];
	DWORD r = GetModuleFileNameA(hMod, filename, MAX_PATH);
	if (!r) return 0;
	
	char * p = 0;

	// Find the last '\' mark.
	int i = r - 1;
	for (; i >= 0; i--)
	{	
		if (filename[i] == '\\') 
		{
			p = &filename[i + 1]; 
			break;
		} 
	}

	if (!p)
	{
		i = 0;
		p = filename;
	}
	
	
	len = (len - 1 < r - i - 1) ? len - 1 : r - i - 1;
//	len = min(len - 1, r - i - 1);
	memcpy(buf, p, len);
	buf[len] = 0;
	return len;
}

bool sym_engine::check()
{	
	if (!m_ok) 
		m_ok = guard::instance().init(); 
	return m_ok; 
}

sym_engine::guard::guard() 
	: m_ref(0) 
{}

sym_engine::guard::~guard() 
{ 
	clear(); 
}

bool sym_engine::guard::init()
{	
	if (!m_ref) 
	{
		m_ref = -1;
		
		HANDLE hProc = SymGetProcessHandle();
		DWORD  dwPid = GetCurrentProcessId();

		// initializes
    debug_api::instance().set_options (debug_api::instance().get_options()|SYMOPT_DEFERRED_LOADS|SYMOPT_LOAD_LINES);
	//	SymSetOptions (SYMOPT_UNDNAME|SYMOPT_LOAD_LINES);
		if (debug_api::instance().initialize(hProc, 0, TRUE))
		{			
			// enumerate modules
			if (IsNT())		
			{
				typedef BOOL (WINAPI *ENUMPROCESSMODULES)(HANDLE, HMODULE*, DWORD, LPDWORD);

				HINSTANCE hInst = LoadLibrary(_T("psapi.dll"));
				if (hInst)
				{				
					ENUMPROCESSMODULES fnEnumProcessModules = 
						(ENUMPROCESSMODULES)GetProcAddress(hInst, "EnumProcessModules");
					DWORD cbNeeded = 0;
					if (fnEnumProcessModules &&
						fnEnumProcessModules(GetCurrentProcess(), 0, 0, &cbNeeded) &&
						cbNeeded)
					{	
						HMODULE * pmod = (HMODULE *)alloca(cbNeeded);
						DWORD cb = cbNeeded;
						if (fnEnumProcessModules(GetCurrentProcess(), pmod, cb, &cbNeeded))
						{
							m_ref = 0;
							for (unsigned i = 0; i < cb / sizeof (HMODULE); ++i)
							{								
								if (!load_module(hProc, pmod[i]))
								{
								//	m_ref = -1;
								//	break;
									_ASSERTE(0);
								}									
							}							
						}
					}
					else
					{
						_ASSERTE(0);
					}
					VERIFY(FreeLibrary(hInst));
				}
				else
				{
					_ASSERTE(0);
				}
			}
			else
			{
				typedef HANDLE (WINAPI *CREATESNAPSHOT)(DWORD, DWORD);
				typedef BOOL (WINAPI *MODULEWALK)(HANDLE, LPMODULEENTRY32);

				HMODULE hMod = GetModuleHandle(_T("kernel32"));
				CREATESNAPSHOT fnCreateToolhelp32Snapshot = (CREATESNAPSHOT)GetProcAddress(hMod, "CreateToolhelp32Snapshot");
				MODULEWALK fnModule32First = (MODULEWALK)GetProcAddress(hMod, "Module32First");
				MODULEWALK fnModule32Next  = (MODULEWALK)GetProcAddress(hMod, "Module32Next");

				if (fnCreateToolhelp32Snapshot && 
					fnModule32First && 
					fnModule32Next)
				{				
					HANDLE hModSnap = fnCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
					if (hModSnap)
					{
						MODULEENTRY32 me32 = {0};						 
						me32.dwSize = sizeof(MODULEENTRY32);
						if (fnModule32First(hModSnap, &me32))
						{
							m_ref = 0;
							do
							{
								if (!load_module(hProc, me32.hModule))
								{
								//	m_ref = -1;
								//	break;
								}																	
							}
							while(fnModule32Next(hModSnap, &me32));
						}
						VERIFY(CloseHandle(hModSnap));						
					}
				}
			}

			if (m_ref == -1)
			{				
        VERIFY(debug_api::instance().cleanup(SymGetProcessHandle()));
			}
		}
		else
		{
			_ASSERTE(0);
		}
	}
	if (m_ref == -1)
		return false;
	if (0 == m_ref) 
		++m_ref; // lock it once
//	++m_ref;
	return true;
}

void sym_engine::guard::clear()
{
	if (m_ref ==  0) return;
	if (m_ref == -1) return;	
	if (--m_ref == 0)
	{	 
    VERIFY(debug_api::instance().cleanup(SymGetProcessHandle()));
	}
}

bool sym_engine::guard::load_module(HANDLE hProcess, HMODULE hMod)
{	
	char filename[MAX_PATH];
	if (!GetModuleFileNameA(hMod, filename, MAX_PATH))
		return false;
	
	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) return false;

	// "Debugging Applications" John Robbins	
    // For whatever reason, SymLoadModule can return zero, but it still loads the modules. Sheez.
	SetLastError(ERROR_SUCCESS);
  if (!debug_api::instance().load_module(hProcess, hFile, filename, 0, (DWORD)hMod, 0) && 
		ERROR_SUCCESS != GetLastError())
	{
		return false;
	}
	
	return true;
}

bool sym_engine::stack_trace(std::list<StackEntry> &s, CONTEXT * pctx, unsigned skip, const char * fmt)
{
	if (!fmt) return false;	
	sym_engine sym(0);
	return stack_trace(s, sym, pctx, skip, fmt);	
}

/////////////////////////////////////////////
// prints a current thread's stack

struct current_context : CONTEXT
{
	HANDLE	thread;	 
	volatile int signal;
};

static DWORD WINAPI tproc(void * pv)
{		
	current_context * p = reinterpret_cast<current_context*>(pv);
	
	__try
	{	
		// Konstantin, 14.01.2002 17:21:32
		// must wait in spin lock until main thread will leave a ResumeThread (must return back to user context)
		unsigned debug_only = 0;
		while (p->signal) 
		{		
			if (!SwitchToThread())
				Sleep(20); // forces switch to another thread 
			++debug_only;
		}
		#ifdef SYM_ENGINE_TRACE_SPIN_COUNT
		char s[256];
		wsprintf(s, "sym_engine::tproc, spin count %u\n", debug_only);
		OutputDebugString(s);
		#endif;
		
		if (-1 == SuspendThread(p->thread)) 
		{
			p->signal  = -1;
			__leave;
		}
		
		__try
		{
			p->signal = GetThreadContext(p->thread, p) ? 1 : -1;
		}
		__finally
		{
			VERIFY(-1 != ResumeThread(p->thread));
		}		
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	
		p->signal  = -1;
	}	
	return 0;
}

bool sym_engine::stack_trace(std::list<StackEntry>& s, unsigned skip, const char * fmt)
{
	if (!fmt) return false;

	// attempts to get current thread's context
	
	current_context ctx;
	memset(&ctx, 0, sizeof current_context);

	BOOL r = DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), 
		&ctx.thread, 0, 0, DUPLICATE_SAME_ACCESS);

	_ASSERTE(r);
	_ASSERTE(ctx.thread);

	if (!r || !ctx.thread)
		return false;

	ctx.ContextFlags = CONTEXT_CONTROL; // CONTEXT_FULL;
	ctx.signal = -1;

	DWORD dummy;		
	HANDLE worker = CreateThread(0, 0, tproc, &ctx, CREATE_SUSPENDED, &dummy);	
	_ASSERTE(worker);

	if (worker)
	{
		 VERIFY(SetThreadPriority(worker, THREAD_PRIORITY_ABOVE_NORMAL)); //  THREAD_PRIORITY_HIGHEST
		if (-1 != ResumeThread(worker))
		{
			unsigned debug_only = 0;
										// Konstantin, 14.01.2002 17:21:32
			ctx.signal = 0;				// only now the worker thread can get this thread context
			while (!ctx.signal)
				++debug_only; // wait in spin		
			#ifdef SYM_ENGINE_TRACE_SPIN_COUNT
			char s[256];
			wsprintf(s, "sym_engine::stack_trace, spin count %u\n", debug_only);
			OutputDebugString(s);
			#endif
		}			
		else
		{
			VERIFY(TerminateThread(worker, 0));
		}

		VERIFY(CloseHandle(worker));		
	}
	
	VERIFY(CloseHandle(ctx.thread));

	if (ctx.signal == -1)
	{
		_ASSERTE(0);
		return false;
	}
		
	// now it can print stack	
	sym_engine sym(0);
	stack_trace(s, sym, &ctx, skip, fmt);
	return true;
}

bool sym_engine::stack_trace(std::list<StackEntry>& s, sym_engine& sym, 
		CONTEXT * pctx, unsigned skip, const char * fmt)
{	
	if (!sym.stack_first(pctx)) 
		return false;
					
	char buf [512] = {0};
	char fbuf[512] = {0};
	char sbuf[512] = {0};

	
	do
	{
    StackEntry entry;

		if (!skip)
		{			
			unsigned ln = 0;
			unsigned ld = 0;
			unsigned sd = 0;
			char *   pf	= 0;
			char *   ps = 0;
			
			for (char * p = (char *)fmt; *p; ++p)
			{				
        stringstream os;
	      os << std::dec;

        if (*p == '%')
				{
					++p; // skips '%'
					char c = *p;
					switch (c)
					{
					case 'm':							
						os << (sym.module(buf, sizeof(buf)) ? buf : "?.?");
            entry.module = os.str();
						break;
					case 'f':
            if (!pf) {						
							pf = (sym.fileline(fbuf, sizeof(fbuf), &ln, &ld)) ? fbuf : " ";
            }
						os << pf;
            entry.file = os.str();
						break;
					case 'l':
            if (!pf) {							
							pf = (sym.fileline(fbuf, sizeof(fbuf), &ln, &ld)) ? fbuf : " ";
            }
						if (*(p + 1) == 'd') { os << ld; ++p; }
						else os << ln;	
            os >> entry.line;
						break;
					case 's':
						if (!ps)
							ps = sym.symbol(sbuf, sizeof(sbuf), &sd) ? sbuf : "?()";
						if (*(p + 1) == 'd') { os << sd; ++p; }
						else os << ps;

            entry.symbol = os.str();
						break;
					case '%':
						os << '%';
						break;
					default:
						os << '%' << c;	// prints unknown format's argument
						break;
					}
				}
				else
				{
					os << *p;
				}
			}
      s.push_back(entry);
		}
		else
		{
			--skip;
		}
	}
	while (/*os.good() &&*/ sym.stack_next());
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
///*////////////////////////////////////////////////////////////////////////////
// *  Project:
// *    Memory_and_Exception_Trace
// *
// * ///////////////////////////////////////////////////////////////////////////
// *	File:
// *		Stackwalker.cpp
// *
// *	Remarks:
// *    Dumps memory leaks (unreleased allocations)
// *    Dumps the stack of an thread if an exepction occurs
// *
// *  Known bugs:
// *    - If the allocation-RequestID wrap, then allocations will get lost...
// *
// *	Author:
// *		Jochen Kalmbach, Germany
// *
// *//////////////////////////////////////////////////////////////////////////////
//
//#include <windows.h>
//#include <string>
//#include <vector>
//#include <iostream>
//#include <fstream>
//
//#include <stdlib.h>
//#include <crtdbg.h>
//#include <tchar.h>
//
//using namespace std;
//
//void PrintStacktrace(EXCEPTION_POINTERS *ep, std::ostream &os);
//void printStacktrace(EXCEPTION_POINTERS *ep, std::ostream &os)
//{
//  PrintStacktrace(ep, os);
//}
//
////
//// the form of the output file
//static eAllocCheckOutput g_CallstackOutputType = ACOutput_Advanced;//ACOutput_Simple;
//
//
//// Size of Callstack-trace in bytes (0x500 => appr. 5-9 functions, depending on parameter count for each function)
////#define MAX_ESP_LEN_BUF 0x500
//
//
//#ifdef _IMAGEHLP_
//#error "'imagehlp.h' should only included here, not before this point! Otherwise there are some problems!"
//#endif
//#pragma pack( push, before_imagehlp, 8 )
//#include <imagehlp.h>
//#pragma pack( pop, before_imagehlp )
//#if API_VERSION_NUMBER < 7  // ImageHelp-Version is older.... so define it by mayself
//// The following definition is only available with VC++ 6.0 or higher, so include it here
//extern "C" {
////
//// source file line data structure
////
//typedef struct _IMAGEHLP_LINE
//{
//    DWORD                       SizeOfStruct;           // set to sizeof(IMAGEHLP_LINE)
//    DWORD                       Key;                    // internal
//    DWORD                       LineNumber;             // line number in file
//    PCHAR                       FileName;               // full filename
//    DWORD                       Address;                // first instruction of line
//} IMAGEHLP_LINE, *PIMAGEHLP_LINE;
//#define SYMOPT_LOAD_LINES        0x00000010
//}  // extern "C"
//#endif
//
//
//// Forward definitions of functions:
//static void ShowStackRM( HANDLE hThread, CONTEXT& c, std::ostream &os, PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryFunction, HANDLE hProcess);
//
//
//// Global data:
//static BOOL g_bInitialized = FALSE;
//static HINSTANCE g_hImagehlpDll = NULL;
//
//static CRITICAL_SECTION g_csFileOpenClose = {0};
//
//
//#define gle (GetLastError())
//string getErrorText()
//{
//  LPVOID lpMsgBuf;
//    FormatMessage( 
//        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//        FORMAT_MESSAGE_FROM_SYSTEM | 
//        FORMAT_MESSAGE_IGNORE_INSERTS,
//        NULL,
//        GetLastError(),
//        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
//        (LPTSTR) &lpMsgBuf,
//        0,
//        NULL 
//    );
//    string text = (LPCTSTR)lpMsgBuf;
//    LocalFree(lpMsgBuf);
//    return text;
//}
//
//#define lenof(a) (sizeof(a) / sizeof((a)[0]))
//#define MAXNAMELEN 1024 // max name length for found symbols
//#define IMGSYMLEN ( sizeof IMAGEHLP_SYMBOL )
//#define TTBUFLEN 8096 // for a temp buffer (2^13)
//
//// SymCleanup()
//typedef BOOL (__stdcall *tSC)( IN HANDLE hProcess );
//tSC pSC = NULL;
//
//// SymFunctionTableAccess()
//typedef PVOID (__stdcall *tSFTA)( HANDLE hProcess, DWORD AddrBase );
//tSFTA pSFTA = NULL;
//
//// SymGetLineFromAddr()
//typedef BOOL (__stdcall *tSGLFA)( IN HANDLE hProcess, IN DWORD dwAddr,
//  OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE Line );
//tSGLFA pSGLFA = NULL;
//
//// SymGetModuleBase()
//typedef DWORD (__stdcall *tSGMB)( IN HANDLE hProcess, IN DWORD dwAddr );
//tSGMB pSGMB = NULL;
//
//// SymGetModuleInfo()
//typedef BOOL (__stdcall *tSGMI)( IN HANDLE hProcess, IN DWORD dwAddr, OUT PIMAGEHLP_MODULE ModuleInfo );
//tSGMI pSGMI = NULL;
//
//// SymGetOptions()
//typedef DWORD (__stdcall *tSGO)( VOID );
//tSGO pSGO = NULL;
//
//// SymGetSymFromAddr()
//typedef BOOL (__stdcall *tSGSFA)( IN HANDLE hProcess, IN DWORD dwAddr,
//  OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_SYMBOL Symbol );
//tSGSFA pSGSFA = NULL;
//
//// SymInitialize()
//typedef BOOL (__stdcall *tSI)( IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess );
//tSI pSI = NULL;
//
//// SymLoadModule()
//typedef DWORD (__stdcall *tSLM)( IN HANDLE hProcess, IN HANDLE hFile,
//  IN PSTR ImageName, IN PSTR ModuleName, IN DWORD BaseOfDll, IN DWORD SizeOfDll );
//tSLM pSLM = NULL;
//
//// SymSetOptions()
//typedef DWORD (__stdcall *tSSO)( IN DWORD SymOptions );
//tSSO pSSO = NULL;
//
//// StackWalk()
//typedef BOOL (__stdcall *tSW)( DWORD MachineType, HANDLE hProcess,
//  HANDLE hThread, LPSTACKFRAME StackFrame, PVOID ContextRecord,
//  PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,
//  PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
//  PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine,
//  PTRANSLATE_ADDRESS_ROUTINE TranslateAddress );
//tSW pSW = NULL;
//
//// UnDecorateSymbolName()
//typedef DWORD (__stdcall WINAPI *tUDSN)( PCSTR DecoratedName, PSTR UnDecoratedName,
//  DWORD UndecoratedLength, DWORD Flags );
//tUDSN pUDSN = NULL;
//
//
//
//struct ModuleEntry
//{
//  std::string imageName;
//  std::string moduleName;
//  DWORD baseAddress;
//  DWORD size;
//};
//typedef std::vector< ModuleEntry > ModuleList;
//typedef ModuleList::iterator ModuleListIter;
//
//// **************************************** ToolHelp32 ************************
//#define MAX_MODULE_NAME32 255
//#define TH32CS_SNAPMODULE   0x00000008
//#pragma pack( push, 8 )
//typedef struct tagMODULEENTRY32
//{
//    DWORD   dwSize;
//    DWORD   th32ModuleID;       // This module
//    DWORD   th32ProcessID;      // owning process
//    DWORD   GlblcntUsage;       // Global usage count on the module
//    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
//    BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
//    DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
//    HMODULE hModule;            // The hModule of this module in th32ProcessID's context
//    char    szModule[MAX_MODULE_NAME32 + 1];
//    char    szExePath[MAX_PATH];
//} MODULEENTRY32;
//typedef MODULEENTRY32 *  PMODULEENTRY32;
//typedef MODULEENTRY32 *  LPMODULEENTRY32;
//#pragma pack( pop )
//
//
//
//static bool GetModuleListTH32(ModuleList& modules, DWORD pid, std::ostream &os)
//{
//  // CreateToolhelp32Snapshot()
//  typedef HANDLE (__stdcall *tCT32S)(DWORD dwFlags, DWORD th32ProcessID);
//  // Module32First()
//  typedef BOOL (__stdcall *tM32F)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
//  // Module32Next()
//  typedef BOOL (__stdcall *tM32N)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
//
//  // try both dlls...
//  const TCHAR *dllname[] = { _T("kernel32.dll"), _T("tlhelp32.dll") };
//  HINSTANCE hToolhelp;
//  tCT32S pCT32S;
//  tM32F pM32F;
//  tM32N pM32N;
//
//  HANDLE hSnap;
//  MODULEENTRY32 me;
//  me.dwSize = sizeof(me);
//  bool keepGoing;
//  ModuleEntry e;
//  int i;
//
//  for (i = 0; i<lenof(dllname); i++ )
//  {
//    hToolhelp = LoadLibrary( dllname[i] );
//    if (hToolhelp == NULL)
//      continue;
//    pCT32S = (tCT32S) GetProcAddress(hToolhelp, "CreateToolhelp32Snapshot");
//    pM32F = (tM32F) GetProcAddress(hToolhelp, "Module32First");
//    pM32N = (tM32N) GetProcAddress(hToolhelp, "Module32Next");
//    if ( pCT32S != 0 && pM32F != 0 && pM32N != 0 )
//      break; // found the functions!
//    FreeLibrary(hToolhelp);
//    hToolhelp = NULL;
//  }
//
//  if (hToolhelp == NULL)
//    return false;
//
//  hSnap = pCT32S( TH32CS_SNAPMODULE, pid );
//  if (hSnap == (HANDLE) -1)
//    return false;
//
//  keepGoing = !!pM32F( hSnap, &me );
//  while (keepGoing)
//  {
//    e.imageName = me.szExePath;
//    e.moduleName = me.szModule;
//    e.baseAddress = (DWORD) me.modBaseAddr;
//    e.size = me.modBaseSize;
//    modules.push_back( e );
//    keepGoing = !!pM32N( hSnap, &me );
//  }
//
//  CloseHandle(hSnap);
//  FreeLibrary(hToolhelp);
//
//  return modules.size() != 0;
//}  // GetModuleListTH32
//
//
//// **************************************** PSAPI ************************
//typedef struct _MODULEINFO {
//    LPVOID lpBaseOfDll;
//    DWORD SizeOfImage;
//    LPVOID EntryPoint;
//} MODULEINFO, *LPMODULEINFO;
//
//static bool GetModuleListPSAPI(ModuleList &modules, DWORD pid, HANDLE hProcess, std::ostream &os)
//{
//  // EnumProcessModules()
//  typedef BOOL (__stdcall *tEPM)(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded );
//  // GetModuleFileNameEx()
//  typedef DWORD (__stdcall *tGMFNE)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
//  // GetModuleBaseName()
//  typedef DWORD (__stdcall *tGMBN)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize );
//  // GetModuleInformation()
//  typedef BOOL (__stdcall *tGMI)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO pmi, DWORD nSize );
//
//  HINSTANCE hPsapi;
//  tEPM pEPM;
//  tGMFNE pGMFNE;
//  tGMBN pGMBN;
//  tGMI pGMI;
//
//  DWORD i;
//  ModuleEntry e;
//  DWORD cbNeeded;
//  MODULEINFO mi;
//  HMODULE *hMods = 0;
//  char *tt = 0;
//
//  hPsapi = LoadLibrary( _T("psapi.dll") );
//  if ( hPsapi == 0 )
//    return false;
//
//  modules.clear();
//
//  pEPM = (tEPM) GetProcAddress( hPsapi, "EnumProcessModules" );
//  pGMFNE = (tGMFNE) GetProcAddress( hPsapi, "GetModuleFileNameExA" );
//  pGMBN = (tGMFNE) GetProcAddress( hPsapi, "GetModuleBaseNameA" );
//  pGMI = (tGMI) GetProcAddress( hPsapi, "GetModuleInformation" );
//  if ( pEPM == 0 || pGMFNE == 0 || pGMBN == 0 || pGMI == 0 )
//  {
//    // we couldn´t find all functions
//    FreeLibrary( hPsapi );
//    return false;
//  }
//
//  hMods = (HMODULE*) malloc(sizeof(HMODULE) * (TTBUFLEN / sizeof HMODULE));
//  tt = (char*) malloc(sizeof(char) * TTBUFLEN);
//
//  if ( ! pEPM( hProcess, hMods, TTBUFLEN, &cbNeeded ) )
//  {
//    os << "EPM failed, GetLastError =" << gle << endl;
//    goto cleanup;
//  }
//
//  if ( cbNeeded > TTBUFLEN )
//  {
//    os << "More than %lu module handles. Huh?" << lenof( hMods ) << endl;
//    goto cleanup;
//  }
//
//  for ( i = 0; i < cbNeeded / sizeof hMods[0]; i++ )
//  {
//    // base address, size
//    pGMI(hProcess, hMods[i], &mi, sizeof mi );
//    e.baseAddress = (DWORD) mi.lpBaseOfDll;
//    e.size = mi.SizeOfImage;
//    // image file name
//    tt[0] = 0;
//    pGMFNE(hProcess, hMods[i], tt, TTBUFLEN );
//    e.imageName = tt;
//    // module name
//    tt[0] = 0;
//    pGMBN(hProcess, hMods[i], tt, TTBUFLEN );
//    e.moduleName = tt;
//
//    modules.push_back(e);
//  }
//
//cleanup:
//  if (hPsapi)
//    FreeLibrary(hPsapi);
//  free(tt);
//  free(hMods);
//
//  return modules.size() != 0;
//}  // GetModuleListPSAPI
//
//
//static bool GetModuleList(ModuleList& modules, DWORD pid, HANDLE hProcess, std::ostream &os)
//{
//  // first try toolhelp32
//  if (GetModuleListTH32(modules, pid, os)) {
//    return true;
//  }
//    
//  // then try psapi
//  return GetModuleListPSAPI(modules, pid, hProcess, os);
//}  // GetModuleList
//
//
//static void EnumAndLoadModuleSymbols( HANDLE hProcess, DWORD pid, std::ostream &os )
//{
//  static ModuleList modules;
//
//  // fill in module list
//  GetModuleList(modules, pid, hProcess, os);
//
//  for (ModuleListIter it = modules.begin(); it != modules.end(); ++ it )
//  {
//    // SymLoadModule() wants writeable strings but helps says it's in-parameter only...
//    pSLM( hProcess, 0, const_cast<char*>(it->imageName.c_str()), const_cast<char*>(it->moduleName.c_str()), it->baseAddress, it->size );
//  }
//}
//
//static int InitStackWalk(void)
//{
//  if (g_bInitialized != FALSE)
//    return 0;  // already initialized
//
//  // we load imagehlp.dll dynamically because the NT4-version does not
//  // offer all the functions that are in the NT5 lib
//  g_hImagehlpDll = LoadLibrary( _T("imagehlp.dll") );
//  if ( g_hImagehlpDll == NULL )
//  {
//    printf( "LoadLibrary( \"imagehlp.dll\" ): GetLastError = %lu\n", gle );
//    g_bInitialized = FALSE;
//    return 1;
//  }
//
//  pSC = (tSC) GetProcAddress( g_hImagehlpDll, "SymCleanup" );
//  pSFTA = (tSFTA) GetProcAddress( g_hImagehlpDll, "SymFunctionTableAccess" );
//  pSGLFA = (tSGLFA) GetProcAddress( g_hImagehlpDll, "SymGetLineFromAddr" );
//  pSGMB = (tSGMB) GetProcAddress( g_hImagehlpDll, "SymGetModuleBase" );
//  pSGMI = (tSGMI) GetProcAddress( g_hImagehlpDll, "SymGetModuleInfo" );
//  pSGO = (tSGO) GetProcAddress( g_hImagehlpDll, "SymGetOptions" );
//  pSGSFA = (tSGSFA) GetProcAddress( g_hImagehlpDll, "SymGetSymFromAddr" );
//  pSI = (tSI) GetProcAddress( g_hImagehlpDll, "SymInitialize" );
//  pSSO = (tSSO) GetProcAddress( g_hImagehlpDll, "SymSetOptions" );
//  pSW = (tSW) GetProcAddress( g_hImagehlpDll, "StackWalk" );
//  pUDSN = (tUDSN) GetProcAddress( g_hImagehlpDll, "UnDecorateSymbolName" );
//  pSLM = (tSLM) GetProcAddress( g_hImagehlpDll, "SymLoadModule" );
//
//  if ( pSC == NULL || pSFTA == NULL || pSGMB == NULL || pSGMI == NULL ||
//    pSGO == NULL || pSGSFA == NULL || pSI == NULL || pSSO == NULL ||
//    pSW == NULL || pUDSN == NULL || pSLM == NULL )
//  {
//    printf( "GetProcAddress(): some required function not found.\n" );
//    FreeLibrary( g_hImagehlpDll );
//    g_bInitialized = FALSE;
//    return 1;
//  }
//
//  g_bInitialized = TRUE;
//  InitializeCriticalSection(&g_csFileOpenClose);
//  return 0;
//}
//
//
//// #################################################################################
//// #################################################################################
//// Here the Stackwalk-Part begins.
////   Some of the code is from an example from a book 
////   But I couldn´t find the reference anymore... sorry...
////   If someone knowns, please let me know...
//// #################################################################################
//// #################################################################################
//
//
//// if you use C++ exception handling: install a translator function
//// with set_se_translator(). In the context of that function (but *not*
//// afterwards), you can either do your stack dump, or save the CONTEXT
//// record as a local copy. Note that you must do the stack sump at the
//// earliest opportunity, to avoid the interesting stackframes being gone
//// by the time you do the dump.
//
//// status: 
//// - EXCEPTION_CONTINUE_SEARCH: exception wird weitergereicht
//// - EXCEPTION_CONTINUE_EXECUTION: 
//// - EXCEPTION_EXECUTE_HANDLER:
//
//
//void PrintStacktrace(EXCEPTION_POINTERS *ep, std::ostream &os) {
//  HANDLE hThread;
//  DuplicateHandle( GetCurrentProcess(), GetCurrentThread(),
//  GetCurrentProcess(), &hThread, 0, false, DUPLICATE_SAME_ACCESS );
//  ShowStackRM(hThread, *(ep->ContextRecord), os, NULL, GetCurrentProcess());
//  CloseHandle( hThread );
//
//}
//
//static void ShowStackRM( HANDLE hThread, CONTEXT& c, std::ostream &os, PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryFunction, HANDLE hSWProcess) {
//  // normally, call ImageNtHeader() and use machine info from PE header
//  DWORD imageType = IMAGE_FILE_MACHINE_I386;
//  HANDLE hProcess = GetCurrentProcess(); // hProcess normally comes from outside
//  int frameNum; // counts walked frames
//  DWORD offsetFromSymbol; // tells us how far from the symbol we were
//  DWORD offsetFromLine; // tells us how far from the line we were
//  DWORD symOptions; // symbol handler settings
//
//  static IMAGEHLP_SYMBOL *pSym = NULL;
//  char undName[MAXNAMELEN]; // undecorated name
//  char undFullName[MAXNAMELEN]; // undecorated name with all shenanigans
//  IMAGEHLP_MODULE Module;
//  IMAGEHLP_LINE Line;
//
//  std::string symSearchPath;
//
//  static bFirstTime = TRUE;
//
//
//  STACKFRAME s; // in/out stackframe
//  memset( &s, '\0', sizeof s );
//
//  if ( (g_bInitialized == FALSE) && (bFirstTime == TRUE) ) {
//    InitStackWalk();
//  }
//
//  if (g_bInitialized == FALSE)
//  {
//    // Could not init!!!!
//    bFirstTime = FALSE;
//    os << "Stackwalker not initialized (or was not able to initialize)!" << endl;
//    return;
//  }
//
//// Critical section begin...
//  EnterCriticalSection(&g_csFileOpenClose);
//
//
//  // NOTE: normally, the exe directory and the current directory should be taken
//  // from the target process. The current dir would be gotten through injection
//  // of a remote thread; the exe fir through either ToolHelp32 or PSAPI.
//
//  if (pSym == NULL) {
//    pSym = (IMAGEHLP_SYMBOL *) malloc( IMGSYMLEN + MAXNAMELEN );
//    if (!pSym) goto cleanup;  // not enough memory...
//  }
//
//
//  if (bFirstTime) {
//
//    CHAR *tt, *p;
//
//    tt = (CHAR*) malloc(sizeof(CHAR) * TTBUFLEN); // Get the temporary buffer
//    if (!tt) goto cleanup;  // not enough memory...
//
//    // build symbol search path from:
//    symSearchPath = "";
//    // current directory
//    if ( GetCurrentDirectoryA( TTBUFLEN, tt ) )
//      symSearchPath += tt + std::string( ";" );
//    // dir with executable
//    if ( GetModuleFileNameA( 0, tt, TTBUFLEN ) )
//    {
//      for ( p = tt + strlen( tt ) - 1; p >= tt; -- p )
//      {
//        // locate the rightmost path separator
//        if ( *p == '\\' || *p == '/' || *p == ':' )
//          break;
//      }
//      // if we found one, p is pointing at it; if not, tt only contains
//      // an exe name (no path), and p points before its first byte
//      if ( p != tt ) // path sep found?
//      {
//        if ( *p == ':' ) // we leave colons in place
//          ++ p;
//        *p = '\0'; // eliminate the exe name and last path sep
//        symSearchPath += tt + std::string( ";" );
//      }
//    }
//    // environment variable _NT_SYMBOL_PATH
//    if ( GetEnvironmentVariableA( "_NT_SYMBOL_PATH", tt, TTBUFLEN ) )
//      symSearchPath += tt + std::string( ";" );
//    // environment variable _NT_ALTERNATE_SYMBOL_PATH
//    if ( GetEnvironmentVariableA( "_NT_ALTERNATE_SYMBOL_PATH", tt, TTBUFLEN ) )
//      symSearchPath += tt + std::string( ";" );
//    // environment variable SYSTEMROOT
//    if ( GetEnvironmentVariableA( "SYSTEMROOT", tt, TTBUFLEN ) )
//      symSearchPath += tt + std::string( ";" );
//
//
//
//    if ( symSearchPath.size() > 0 ) // if we added anything, we have a trailing semicolon
//      symSearchPath = symSearchPath.substr( 0, symSearchPath.size() - 1 );
//
//    // why oh why does SymInitialize() want a writeable string?
//    strncpy( tt, symSearchPath.c_str(), TTBUFLEN );
//    tt[TTBUFLEN - 1] = '\0'; // if strncpy() overruns, it doesn't add the null terminator
//
//    // init symbol handler stuff (SymInitialize())
//    if ( ! pSI( hProcess, tt, false ) )
//    {
//      os << "SymInitialize(): GetLastError = " << gle << endl;
//      if (tt) free( tt );
//      goto cleanup;
//    }
//
//    // SymGetOptions()
//    symOptions = pSGO();
//    symOptions |= SYMOPT_LOAD_LINES;
//    symOptions &= ~SYMOPT_UNDNAME;
//    symOptions &= ~SYMOPT_DEFERRED_LOADS;
//    pSSO( symOptions ); // SymSetOptions()
//
//    // Enumerate modules and tell imagehlp.dll about them.
//    // On NT, this is not necessary, but it won't hurt.
//    EnumAndLoadModuleSymbols( hProcess, GetCurrentProcessId(), os );
//
//    if (tt) 
//      free( tt );
//  }  // bFirstTime = TRUE
//  bFirstTime = FALSE;
//
//  // init STACKFRAME for first call
//  // Notes: AddrModeFlat is just an assumption. I hate VDM debugging.
//  // Notes: will have to be #ifdef-ed for Alphas; MIPSes are dead anyway,
//  // and good riddance.
//  s.AddrPC.Offset = c.Eip;
//  s.AddrPC.Mode = AddrModeFlat;
//  s.AddrFrame.Offset = c.Ebp;
//  s.AddrFrame.Mode = AddrModeFlat;
//
//  memset( pSym, '\0', IMGSYMLEN + MAXNAMELEN );
//  pSym->SizeOfStruct = IMGSYMLEN;
//  pSym->MaxNameLength = MAXNAMELEN;
//
//  memset( &Line, '\0', sizeof Line );
//  Line.SizeOfStruct = sizeof Line;
//
//  memset( &Module, '\0', sizeof Module );
//  Module.SizeOfStruct = sizeof Module;
//
//  for (frameNum = 0; ; ++frameNum)
//  {
//    // get next stack frame (StackWalk(), SymFunctionTableAccess(), SymGetModuleBase())
//    // if this returns ERROR_INVALID_ADDRESS (487) or ERROR_NOACCESS (998), you can
//    // assume that either you are done, or that the stack is so hosed that the next
//    // deeper frame could not be found.
//    // CONTEXT need not to be suplied if imageTyp is IMAGE_FILE_MACHINE_I386!
//    if ( ! pSW( imageType, hSWProcess, hThread, &s, NULL, ReadMemoryFunction, pSFTA, pSGMB, NULL ) )
//      break;
//
//    if (g_CallstackOutputType == ACOutput_Advanced)
//      os << frameNum << endl;
//
//    if ( s.AddrPC.Offset == 0 )
//    {
//      // Special case: If we are here, we have no valid callstack entry!
//      switch(g_CallstackOutputType)
//      {
//      case ACOutput_Simple:
//        os << "(-nosymbols- PC == 0)" << endl;
//        break;
//      case ACOutput_Advanced:
//        os << "   (-nosymbols- PC == 0)" << endl;
//        break;
//      }
//    }
//    else
//    {
//      // we seem to have a valid PC
//      undName[0] = 0;
//      undFullName[0] = 0;
//      offsetFromSymbol = 0;
//      // show procedure info (SymGetSymFromAddr())
//      if ( ! pSGSFA( hProcess, s.AddrPC.Offset, &offsetFromSymbol, pSym ) )
//      {
//        if (g_CallstackOutputType == ACOutput_Advanced)
//        {
//          if(gle != 487)
//            os << "   SymGetSymFromAddr(): GetLastError = " << getErrorText() << endl;
//          else
//            os << "   no stack information :(" << endl;
//        }
//      }
//      else
//      {
//        // UnDecorateSymbolName()
//        pUDSN( pSym->Name, undName, MAXNAMELEN, UNDNAME_NAME_ONLY );
//        pUDSN( pSym->Name, undFullName, MAXNAMELEN, UNDNAME_COMPLETE );
//        if (g_CallstackOutputType == ACOutput_Advanced)
//        {
//          if (strlen(undName) > 0)
//            os <<  "     " << undName << " " << (long) offsetFromSymbol << " bytes" << endl;
//          else
//          {
//            os << "     Sig:  " << pSym->Name << (long) offsetFromSymbol << " bytes" << endl;
//            strcpy(undName, pSym->Name);
//          }
//          os << "     Decl: " << undFullName << endl;
//        }
//      }
//
//      // show line number info, NT5.0-method (SymGetLineFromAddr())
//      offsetFromLine = 0;
//      if ( pSGLFA != NULL )
//      { // yes, we have SymGetLineFromAddr()
//        if ( ! pSGLFA( hProcess, s.AddrPC.Offset, &offsetFromLine, &Line ) )
//        {
//          if ( (gle != 487) && (frameNum > 0) )  // ignore error for first frame
//          {
//             os  << "SymGetLineFromAddr(): GetLastError = " << gle << endl;
//          }
//          os  << "no file info :(" << endl;
//        }
//        else
//        {
//          switch(g_CallstackOutputType)
//          {
//          case ACOutput_Advanced:
//            os << "     Line: " << Line.FileName << "(" << Line.LineNumber <<") "<< offsetFromLine << " bytes" << endl;
//            break;
//          case ACOutput_Simple:
//            os << "     " << Line.FileName << "(" << Line.LineNumber <<") "<< offsetFromLine << " bytes (" << undName << ")" << endl;
//            break;
//          }
//        }
//      } // yes, we have SymGetLineFromAddr()
//
//      // show module info (SymGetModuleInfo())
//      if ( (g_CallstackOutputType == ACOutput_Advanced))
//      {
//        if ( ! pSGMI( hProcess, s.AddrPC.Offset, &Module ) )
//        {
//          if (g_CallstackOutputType == ACOutput_Advanced)
//            os << "SymGetModuleInfo): GetLastError = " << gle << endl;
//        }
//        else
//        { // got module info OK
//          char ty[80];
//          switch ( Module.SymType )
//          {
//          case SymNone:
//            strcpy( ty, "-nosymbols-" );
//            break;
//          case SymCoff:
//            strcpy( ty, "COFF" );
//            break;
//          case SymCv:
//            strcpy( ty, "CV" );
//            break;
//          case SymPdb:
//            strcpy( ty, "PDB" );
//            break;
//          case SymExport:
//            strcpy( ty, "-exported-" );
//            break;
//          case SymDeferred:
//            strcpy( ty, "-deferred-" );
//            break;
//          case SymSym:
//            strcpy( ty, "SYM" );
//            break;
//          /* // TODO: #if API_VERSION_NUMBER >= 9 ?
//          case SymDia:
//            strcpy( ty, "DIA" );
//            break;*/
//          default:
//            _snprintf( ty, sizeof ty, "symtype=%ld", (long) Module.SymType );
//            break;
//          }
//        } // got module info OK
//      }
//    } // we seem to have a valid PC
//
//    // no return address means no deeper stackframe
//    if ( s.AddrReturn.Offset == 0 )
//    {
//      // avoid misunderstandings in the printf() following the loop
//      SetLastError( 0 );
//      break;
//    }
//
//  } // for ( frameNum )
//
//cleanup:
//  //if (pSym) free( pSym );
//  if (os) {
//    os << endl << endl;
//  }
//
//  LeaveCriticalSection(&g_csFileOpenClose);
//// Critical section end...
//}  // ShowStackRM
