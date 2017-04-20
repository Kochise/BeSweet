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

#include "VSEditor.h"

#include <RemoteInterface/COM/stubs/ESBDSAddin_i.c>

VisualStudioEditor::VisualStudioEditor(const std::string &file)
: mFile(file)
{
  log(GENERAL, REMOTE_EDITOR, eInfo) << "will search for besweet addin " << endlog;
  
  HRESULT hr = mAddin.CoCreateInstance(CLSID_ESBDSAddin);//, NULL, CLSCTX_LOCAL_SERVER, IID_IESBDSAddin, reinterpret_cast< void**>(&mAddin));
  if(FAILED(hr))
  {
    log(GENERAL, REMOTE_EDITOR, eError) << "besweet addin not found !!" << _com_error(hr).ErrorMessage() << endlog;
  }
}

VisualStudioEditor::VisualStudioEditor(const CComPtr<IESBDSAddin> &addin, const std::string &file)
: mFile(file), mAddin(addin)
{
}

void VisualStudioEditor::openFile(const std::string &file)
{
  mFile = file;
}

void VisualStudioEditor::goToLine(size_t line)
{
  Tracer trc(GENERAL, REMOTE_SRV, "VisualStudioEditor::goToLine");
  
  if(mAddin != NULL) {
    log(GENERAL, REMOTE_EDITOR, eInfo) << "opening file \"" << mFile << "\"at line " << line << endlog;
    mAddin->gotoLine( CComBSTR(mFile.c_str()), line);
  }
}


VisualStudioEditorBroker::VisualStudioEditorBroker(const CComPtr<IESBDSAddin> &addin)
: mAddin(addin)
{
}


smart_ptr<Editor> VisualStudioEditorBroker::getEditor(const std::string file)
{
  return make_smart_ptr(new VisualStudioEditor(mAddin, file));
}


