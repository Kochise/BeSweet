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
#include "FileOutlineLoaderSimulator.h"

using FileOutlineLoader::TagList;
using namespace std;

namespace Simulators {

TagList FileOutlineLoader::load(const string&)
{
  TagList result;
//  list<Field> parameters;
//	
//	list<Method> methods;
//	parameters.push_back(Field("int", "key"));
//	parameters.push_back(Field("float", "value"));
//	methods.push_back(Method("void", "foo", parameters));
//
//	parameters.clear();
//	methods.push_back(Method("int", "bar", parameters));
//	
//	list<Field> fields;
//	fields.push_back(Field("int", "mId"));
//	fields.push_back(Field("std::string", "mName"));
//
//  list<smart_ptr<MetaObject> > innerTypes;
//  innerTypes.push_back(smart_ptr<CppType>(new Class("Inner", fields, methods)));
//	
//  smart_ptr<MetaObject> c = smart_ptr<MetaObject>(new Class("Foo", fields, methods));
//  c->setInnerTags(innerTypes);
//  
//	result.push_back(c);
//	result.push_back(smart_ptr<MetaObject>(new Function("void", "aFunction", list<Field>())));

	return result;
}

}
