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

#include "LoaderBroker.h"
#include "HierarchyLoader.h"
#include "OutlineLoader.h"

using namespace std;

HierarchyLoader::~HierarchyLoader()
{
}

OutlineLoader::~OutlineLoader()
{
}

LoaderBroker::LoaderBroker()
{
}

LoaderBroker::~LoaderBroker()
{
}

const smart_ptr<LoaderBroker>& LoaderBroker::instance(const smart_ptr<Workspace> &wspace)
{
  typedef std::map<std::string, smart_ptr<LoaderBroker> > BrokerMap;
  static BrokerMap brokers;
  BrokerMap::iterator broker = brokers.find(wspace->getName());
  if(broker == brokers.end()) {
    broker = brokers.insert(broker, make_pair(wspace->getName(), make_smart_ptr(new LoaderBroker())));
  }
  
  return broker->second;
}
