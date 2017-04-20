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

#ifndef CTagsTypeInfoLoader_H
#define CTagsTypeInfoLoader_H

#include <Model/TypeInfoLoader.h>
#include <Model/CTags/CTagsTag.h>
#include <Model/CTags/CTagsDatabase.h>

#include "smart_ptr.h"

#include <vector>
#include <list>
#include <string>

class Workspace;

namespace CTags {

class Database;

class TypeInfoLoader : public ::TypeInfoLoader
{
private:
  smart_ptr<Workspace> mWorkspace;

public:  
  TypeInfoLoader(const smart_ptr<Workspace> &);

  virtual TypeList load();
  virtual TypeList load(const std::string &type_name);

private:
  TypeList make(const Database::TagTable&);
  smart_ptr<Database> db();

};

} //namespace CTags

#endif
