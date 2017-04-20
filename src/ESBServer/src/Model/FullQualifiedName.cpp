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

#include "FullQualifiedName.h"

const static const std::string GLOBAL_NAMESPACE = "global namespace";

using namespace std;

const string FullQualifiedName::empty;

FullQualifiedName::FullQualifiedName(const string &fqn)
:  mName(getNameOf(fqn)) 
{
  setScope((getScopeOf(fqn)));
}

FullQualifiedName::FullQualifiedName(const string& scope, const string& name)
: mName(name) 
{
  setScope(scope);
}

string FullQualifiedName::getScopeOf(const string &fqn)
{
  size_t pos = fqn.rfind("::");
  return pos == string::npos ? "" : fqn.substr(0, pos);
}

string FullQualifiedName::getNameOf(const string &fqn)
{
  size_t pos = fqn.rfind("::");
  return pos == string::npos ? fqn : fqn.substr(pos+2, string::npos);
} 

void FullQualifiedName::setScope(const string &scope)
{
  mScope = scope == GLOBAL_NAMESPACE ? empty : scope;
  //mScope = scope.empty() ? GLOBAL_NAMESPACE : scope;
}

const string& FullQualifiedName::scope() const
{
 return mScope == GLOBAL_NAMESPACE ? empty : mScope;
 //return mScope.empty() ? GLOBAL_NAMESPACE : mScope;
}

string FullQualifiedName::toString () const 
{ 
 return scope().empty() ? name() : scope() + "::" + name(); 
}