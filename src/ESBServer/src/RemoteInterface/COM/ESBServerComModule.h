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

#ifndef ESBServerCOMModule_H
#define ESBServerCOMModule_H

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

#include <string>

inline std::string asString(const BSTR &bstr)
{
  return std::string(CString(bstr).GetBuffer(0));
}

class ComModuleGuard
{
private:
  UINT mResourceID;
  CComModule &mModule;
  std::string mPIDFile;

public:
  ComModuleGuard(CComModule & module, UINT id);
  ~ComModuleGuard();


private:
  void deinit();
  std::string getFileName();
};




class ESBServerComModule : public CComModule
{
private:
	bool mIsATLInitialized;

public:
	ESBServerComModule();

	static ESBServerComModule& instance();

	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;

	
	bool InitializeATL();
	void UninitializeATL();

};


#endif
