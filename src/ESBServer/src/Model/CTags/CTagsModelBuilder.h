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

#ifndef CTagsModelBuilder_H
#define CTagsModelBuilder_H

#include <Model/CTags/CTagsDatabase.h>
#include <Model/MetaModel/MetaModel.h>
#include <Workspace.h>

#include <smart_ptr.h>
#include <list>

namespace CTags {

class ModelBuilder
{
private:
  smart_ptr<Workspace> mWorkspace;
  smart_ptr<LoaderBroker> mBroker;

public:
  typedef std::list<smart_ptr<MetaObject> > TagList;
  
public:
  ModelBuilder(const smart_ptr<Workspace>&);
  std::list<smart_ptr<MetaObject> > createModel(Database::TagTable& table);

private:
  const smart_ptr<LoaderBroker>& broker() { return mBroker; }

  TagList createModel(Database::TagTable& table, size_t level);
  smart_ptr<MetaObject> createObject(Database::TagTable::iterator&, Database::TagTable& table);
};

} //namespace CTags

#endif