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

#ifndef TreeCtrlHelper_H
#define TreeCtrlHelper_H

class TagRetriever
{
private:
  CTreeCtrl *mTree;

public:
  TagRetriever(CTreeCtrl *ctrl) : mTree(ctrl) {}

  MetaObject* getTag(HTREEITEM item)
  { return item ? reinterpret_cast<MetaObject*>(tree().GetItemData(item)) : NULL; }

  MetaObject* getTag(NMHDR* pNMHDR)
  {
	  NM_TREEVIEW* tree = (NM_TREEVIEW*)pNMHDR;
    return tree->itemNew.mask & TVIF_HANDLE ? getTag(tree->itemNew.hItem) : NULL;
  }

  MetaObject* getTagAt(const std::pair<short, short> &mousePosition, bool select)
  {
    return getTagAt(CPoint(mousePosition.first, mousePosition.second), select);
  }

  MetaObject* getTagAt(const CPoint &mousePosition, bool select)
  {
    CPoint pos = mousePosition;
    tree().ScreenToClient(&pos);

    UINT flags;
	  HTREEITEM htItem = tree().HitTest(pos, &flags);
    if(htItem && select && (flags & TVHT_ONITEMICON) || (flags & TVHT_ONITEMLABEL)) {
      tree().Select(htItem, TVGN_CARET);
    }

    return (flags & TVHT_ONITEMICON) || (flags & TVHT_ONITEMLABEL) ? getTag(htItem) : NULL;
  }

private:
  CTreeCtrl& tree() { return *mTree; }

};


#endif