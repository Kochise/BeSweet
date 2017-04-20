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

#ifndef IconBroker_H
#define IconBroker_H

#include <Model/MetaModel/MetaModel.h>
#include <Renderer/IconDescriptor.h>

#include <map>



template<typename Icon>
class IconBroker
{
protected:
  typedef std::map<MetaObjectType, std::map<size_t, Icon> > IconMap;

private:
  IconMap mIcons;

public:
  Icon getIcon(MetaObjectType type)
  { 
    return mIcons[type][0]; 
  }

  Icon getIcon(const MetaObject &tag, bool undecorated = false)
  {  
    IconDescriptor descriptor;
    const_cast<MetaObject&>(tag).acceptVisitor(&descriptor);
    return undecorated ? getIcon(descriptor.type) : getIcon(descriptor);
  }
  
  Icon getIcon(const IconDescriptor &descriptor)
  {
    MetaObjectType type = descriptor.type;
    size_t mask = descriptor.decorators.hash();

    map<size_t, Icon>::iterator image = mIcons[type].find(mask);
    if(image == mIcons[type].end()) {
		  mIcons[type][mask] = createIcon(descriptor);
    }
	  return mIcons[type][mask];
  }

protected:
  void initialize(const IconMap &icons)
  {
    mIcons = icons;
  }

private:
	virtual Icon createIcon(const IconDescriptor&) = 0;

};

#endif