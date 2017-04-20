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

#ifndef SelectionListener_H
#define SelectionListener_H

class MouseEvent;

#include <Model/MetaModel/MetaModel.h>
#include <list>

class SelectionListener
{
public:
  virtual ~SelectionListener() {}
  virtual void selectionChanged(const std::list<MetaObject*>&) const = 0;
  virtual void selectionChanged(const MetaObject&, const MouseEvent&) const = 0;
};

class SingleSelectionListener : public SelectionListener
{
private:
  //empty impelmentation
  virtual void selectionChanged(const std::list<MetaObject*>&) const; 
};

class MultiSelectionListener : public SelectionListener
{
private:
  //empty impelmentation
  virtual void selectionChanged(const MetaObject&, const MouseEvent&) const;
};

class DefaultSingleSelectionListener : public SingleSelectionListener
{
public:
  virtual void selectionChanged(const MetaObject&, const MouseEvent&) const;
  
private:
  virtual void gotoHeader(const MetaObject&) const;
  virtual void gotoSource(const MetaObject&) const; 
  virtual void gotoFile(const TagLocation &location) const;  
};

#endif