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

#include "MetaModelUtilities.h"

using namespace std;

RootFinder::RootMap RootFinder::findRootOf(const smart_ptr<Inheritable> &i)
{
  typedef list<smart_ptr<Inheritable> >::const_iterator Iterator;
  
  RootMap result;
  
  if(i->getParents().empty()) {
    list<smart_ptr<Inheritable> > tmp;
    tmp.push_back(i);
    result.insert(make_pair(0, tmp));
    
  } else {
  
    for(Iterator x = i->getParents().begin(); x != i->getParents().end(); ++x) {
      RootMap tmp = findRootOf(*x);
      for(RootMap::iterator aRoot = tmp.begin(); aRoot != tmp.end(); ++aRoot) {
        result.insert(make_pair(aRoot->first+1, aRoot->second));
      }
    }
  }
  
  return result;
}

typedef list<smart_ptr<Inheritable> > PoEList;

bool IsRelativeOf::operator()(const smart_ptr<Inheritable> &r1, const smart_ptr<Inheritable> &r2) const
{ 
  return isParentOf(r1, r2) || isParentOf(r2, r1);
}

bool IsRelativeOf::isParentOf(const smart_ptr<Inheritable> &parent, const smart_ptr<Inheritable> &child)
{    
  if(parent->equals(*child)) {
    return true;
  }
  
  for(PoEList::const_iterator candidate = child->getParents().begin(); candidate != child->getParents().end(); ++candidate) {
    if(isParentOf(parent, *candidate)) {
      return true;
    }
  }
  
  return false;
}

bool IsRelativeOf::isExtendorOf(const smart_ptr<Inheritable> &child, const smart_ptr<Inheritable> &parent)
{    
  if(parent->equals(*child)) {
    return true;
  }
  
  for(PoEList::const_iterator candidate = parent->getExtendors().begin(); candidate != parent->getExtendors().end(); ++candidate) {
    if(isExtendorOf(child, *candidate)) {
      return true;
    }
  }
  
  return false;
}

