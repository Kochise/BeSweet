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

#ifndef VS_Editor_H
#define VS_Editor_H

#include <stl_ext.h>

#include <atlbase.h>
#include <comdef.h>

#include <RemoteInterface/COM/stubs/ESBDSAddin.h>
#include <Model/Editor.h>


class VisualStudioEditor : public Editor
{
private:
  std::string mFile;
  CComPtr<IESBDSAddin> mAddin;
  
public:
  VisualStudioEditor(const std::string&);
  VisualStudioEditor(const CComPtr<IESBDSAddin>&, const std::string&);
  
  void openFile(const std::string&);
  void goToLine(size_t);
  
private:
};

class VisualStudioEditorBroker : public EditorBroker
{
private:
  CComPtr<IESBDSAddin> mAddin;
  
public:
  VisualStudioEditorBroker(const CComPtr<IESBDSAddin>&);
  smart_ptr<Editor> getEditor(const std::string wspace);
};

#endif