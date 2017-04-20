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

#ifndef WorkspaceBrowser_H
#define WorkspaceBrowser_H

#include "ScopedNameBrowserRenderer.h"
#include "ScopedNameBrowser.h"

#include "Workspace.h"

#include <string>

class WorkspaceBrowserItem : public ScopedNameBrowserItem
{
private:
  std::string  mName;
  std::string  mScope;

public:
  WorkspaceBrowserItem(const std::string &scope, const std::string &name);

  std::string getFile() const ;
  
  std::string getNameLabel() const;
  int getNameIcon() const;

  std::string getScopeLabel() const;
  int getScopeIcon() const;

private:
  std::string extractFileName(const std::string &full) const;
};

class WorkspaceBrowserModel : public ScopedNameBrowserModel
{
public:
  WorkspaceBrowserModel(const Workspace &);

private:
  smart_ptr<ScopedNameBrowserItem> make_item(const std::string &scope, const std::string &name);
};

class WorkspaceBrowser : public ScopedNameBrowser
{
public:
  WorkspaceBrowser();
};



#endif
