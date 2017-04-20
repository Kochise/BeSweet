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

#ifndef TooltipBroker_H
#define TooltipBroker_H

#include <Model/MetaModel/MetaModel.h>
#include <Renderer/UIProperties.h>

#include <Utilities/Properties.h>

#include <string>
#include <sstream>

class TooltipBroker : public MetaObjectVisitorAdaptor
{
private:
  std::string mTooltip;

public:
  TooltipBroker()
  {  }
  
  std::string getTooltip(const MetaObject &tag)
  { 
    const_cast<MetaObject&>(tag).acceptVisitor(this);
    return mTooltip;
  }

private:
  void setTooltip(const std::string &label) { mTooltip = label; }

  void visitTag(MetaObject *tag) { setTooltip(tag->getFQN()); }
//  void visitField(Field *field) { setLabel(getSignature(*field)); }
//  void visitVariable(Variable *variable) { setLabel(getSignature(*variable)); }
  void visitMethod(Method *method) { setTooltip(getSignature(*method)); }
  void visitFunction(Function *function) { setTooltip(getSignature(*function)); }

  std::string getSignature(const Function &f)
  {
    stringstream tooltip;
    tooltip << f.getReturnType() << " " << f.getFQN() << "(";
  
    if(!f.getParameters().empty()) {
      transform(f.getParameters().begin(), --f.getParameters().end(), ostream_iterator<string>(tooltip, ","), mem_fun_ref(&Field::getType));
      tooltip << (--f.getParameters().end())->getType();
    }

    tooltip << ")";

    return tooltip.str();
  }

  std::string getSignature(const Method &m)
  {
    stringstream  tooltip;
  
    if(m.isStatic()) {
      tooltip << "static ";

    } else if(m.isVirtual() || m.isAbstract()) {
      tooltip << "virtual ";
    }

    tooltip << getSignature((Function)m);

    if(m.isConst()) {
      tooltip << " const";
    }
    if(m.isAbstract()) {
      tooltip << " = 0";
    }

    return tooltip.str();
  }

  /*std::string getSignature(const Variable &variable)
  {
    return variable.getFQN().name() + (mShowReturnType ? " : " + variable.getType() : "");
  }*/

};

#endif
