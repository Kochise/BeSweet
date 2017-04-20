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

#ifndef OutlineRenderer_H
#define OutlineRenderer_H

#include <string>
#include <list>
#include <Utilities/smart_ptr.h>
#include <Model/MetaModel/MetaModel.h>


class OutlineBrowser;

class OutlineRenderer
{
public:
  virtual void registerListener(OutlineBrowser*) = 0;
  virtual void render() = 0;
  
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void moveToForeground() = 0;

  virtual void setCaption(const std::string &text) = 0;
  virtual std::string getCaption() = 0;
  
  virtual std::pair<short, short> getPosition() = 0;
  virtual void setPosition(short x, short y) = 0;

  virtual void showPopupMenu(MetaObject*, short x, short y) = 0;
};

#endif