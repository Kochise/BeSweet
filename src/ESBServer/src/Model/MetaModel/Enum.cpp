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

#include <stdafx.h>

#include "Enum.h"

#include <Model/MetaModel/MetaObjectVisitors.h>

using namespace std;

Enum::EnumImpl::EnumImpl(const string &name, AccessQualifier access, const TagLocation &decl)
: super(name, access, decl)
{
}

Enum::Enum(const string &name, AccessQualifier access, const TagLocation &decl)
: super(smart_ptr<DatastoreType>(new DatastoreType(name, access, decl))) 
{
}

Enum::~Enum()
{
}


void Enum::acceptVisitor(MetaObjectVisitor *v) 
{ 
  v->visitEnum(this); 
}
