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

#ifndef LabelBroker_H
#define LabelBroker_H

#include <Model/MetaModel/MetaModel.h>
#include <Renderer/UIProperties.h>

#include <Utilities/Properties.h>

#include <string>

class LabelBroker : public MetaObjectVisitorAdaptor
{
private:
  bool mShowReturnType;
  bool mShowFQN;
  std::string mLabel;

public:
  LabelBroker()
  {  
    mShowReturnType = PropertiesManager::instance().getProperty(Browsers::SHOW_RETURNTYPES, "1") == "1";  
    mShowFQN = PropertiesManager::instance().getProperty(Browsers::Hierarchy::SHOW_FQ_NAMES, "0") == "1";  
  }
  
  std::string getLabel(const MetaObject &tag)
  { 
    const_cast<MetaObject&>(tag).acceptVisitor(this);
    return mLabel;
  }

private:
  void setLabel(const std::string &label) { mLabel = label; }

  void visitTag(MetaObject *tag) { setLabel(mShowFQN ? tag->getFQN().toString() : tag->getFQN().name()); }
  void visitField(Field *field) { setLabel(getSignature(*field)); }
  void visitVariable(Variable *variable) { setLabel(getSignature(*variable)); }
  void visitMethod(Method *method) { setLabel(getSignature(*method)); }
  void visitFunction(Function *function) { setLabel(getSignature(*function)); }

  std::string getSignature(const Function &function)
  {
    return function.getFQN().name() + "()" + (mShowReturnType ? " : " + function.getReturnType() : "");
  }

  std::string getSignature(const Variable &variable)
  {
    return variable.getFQN().name() + (mShowReturnType ? " : " + variable.getType() : "");
  }

};

#endif
