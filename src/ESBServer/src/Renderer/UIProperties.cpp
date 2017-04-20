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

#include "UIProperties.h"
#include <string>

namespace BeSweet
{
  const std::string CHECK_4_NEW_VERSION("besweet.check4newversion");
  const std::string START_COUNTER("besweet.startcounter");
  const std::string INSTALL_TIME("besweet.installtime");
}

namespace Browsers
{
  const std::string HOVERTIME("browsers.hovertime");
  const std::string RAISECORNER("browsers.raisecorner");
  const std::string SHOW_RETURNTYPES("browsers.returntypes");
  const std::string SHOW_TOOLTIPS("browsers.tooltips");
  const std::string CLOSE_ON_RETURN("browsers.closeonreturn");
  const std::string RAISE_ON_MOUSE_HOOVER("browsers.raiseonmousehover");
  const std::string HIDE_ON_MOUSE_EXIT("browsers.hideonmouseexit");
  const std::string STAY_ALLWAYS_ONTOP("browsers.allwaysontop");

  
  const std::string ROLL_UNROLL("browsers.rollunroll");
  const std::string ROLL_ANIMATION("browsers.rollanimation");

  const std::string FADE_UNFADE("browsers.fadeunfade");
  const std::string FADE_ANIMATION("browsers.fadeanimation");

  const std::string FUZZY_SIZE("browsers.fuzzysize");

  namespace Outline
  {
    const std::string HOVERTIME("browsers.outline.hovertime");
    const std::string RAISECORNER("browsers.outline.raisecorner");
    const std::string POSITION("browsers.outline.pos");

    const std::string SHOW_HEADER_AND_SOURCE("browsers.outline.headerandsource");
    const std::string SHOW_NAMESPACES("browsers.outline.namespaces");
  }

  namespace Hierarchy
  {
    const std::string HOVERTIME("browsers.hierarchy.hovertime");
    const std::string RAISECORNER("browsers.hierarchy.raisecorner");
    const std::string POSITION("browsers.hierarchy.pos");

    const std::string SHOW_FQ_NAMES("browsers.hierarchy.fqn");  
    const std::string WARN_MULTIPLE_PARENTS("browsers.hierarchy.warnmulitpleparents");
  }
}