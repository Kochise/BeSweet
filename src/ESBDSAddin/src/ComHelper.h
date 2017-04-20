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

#ifndef ComHelper_H
#define ComHelper_H

#include <string>
#include <sstream>
#include <typeinfo>

#include <comdef.h>

class COMExceptionThrower
{ 
public:
	COMExceptionThrower( void){}
	~COMExceptionThrower( void){}

	/*COMExceptionThrower( const HRESULT errCode)
	{
		this->operator=(errCode);
	}*/
		
protected:
	// don't allow regular copy constructor call
	COMExceptionThrower( const COMExceptionThrower&);

public:
  const COMExceptionThrower& operator=( const HRESULT errCode) const throw( _com_error)
	{
		if ( FAILED( errCode))
			_com_raise_error( errCode);
			
		return *this;
	}
};

inline std::string asString(const _com_error &e)
{
  std::stringstream ss;
	ss << "Com-Exception \"" << e.ErrorMessage() << "\" " << std::hex << _com_error::WCodeToHRESULT(e.WCode());
  return ss.str();
}

#define HResult HRESULT STDMETHODCALLTYPE

template<class T>
CComPtr<T> new_instance()
{
  CComObject<T> *object;
  cex_ = CComObject<T>::CreateInstance(&object);

  return CComPtr<T>(object);
}

template<class ChildType, class ParentType>
bool is_instance_of(ParentType parent)
{
  CComQIPtr<ChildType> child(parent);
  return child != 0;
}

extern COMExceptionThrower cex_;

void log_std_exception(const std::string &class_name, const std::string &method_name, const std::exception &e);
void log_com_exception(const std::string &class_name, const std::string &method_name, const _com_error &e);
void log_unknown_exception(const std::string &class_name, const std::string &method_name);

#define SANDBOX_BEGIN(class_name, method_name)                      \
  static const std::string sbox_class_name(class_name);             \
  static const std::string sbox_method_name(method_name);           \
  try {                                                             \



#define SANDBOX_END()                                               \
  } catch(std::exception &stde) {                                   \
      log_std_exception(sbox_class_name, sbox_method_name, stde);   \
                                                                    \
  } catch(_com_error &come) {                                       \
      log_com_exception(sbox_class_name, sbox_method_name, come);   \
                                                                    \
  } catch(...) {                                                    \
      log_unknown_exception(sbox_class_name, sbox_method_name);     \
  }                                                                 \

inline std::ostream& operator<<(std::ostream &os, const BSTR &bstr)
{
  return os << std::string(bstr_t(bstr));
}

#endif