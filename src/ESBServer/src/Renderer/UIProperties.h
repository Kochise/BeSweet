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

#ifndef UIProperties_H
#define UIProperties_H

#include <string>

namespace BeSweet
{
  extern const std::string CHECK_4_NEW_VERSION;
  extern const std::string START_COUNTER;
  extern const std::string INSTALL_TIME;
}

namespace Browsers
{
  extern const std::string SHOW_RETURNTYPES;
  extern const std::string SHOW_TOOLTIPS;
  extern const std::string HOVERTIME;
  extern const std::string RAISECORNER;
  extern const std::string CLOSE_ON_RETURN;
  extern const std::string RAISE_ON_MOUSE_HOOVER;
  extern const std::string HIDE_ON_MOUSE_EXIT;
  extern const std::string ROLL_UNROLL;
  extern const std::string FADE_UNFADE;
  extern const std::string STAY_ALLWAYS_ONTOP;
  extern const std::string ROLL_ANIMATION;
  extern const std::string FADE_ANIMATION;
  extern const std::string FUZZY_SIZE;
  
  namespace Outline
  {
    extern const std::string HOVERTIME;
    extern const std::string RAISECORNER;
    extern const std::string POSITION;

    extern const std::string SHOW_HEADER_AND_SOURCE;
    extern const std::string SHOW_NAMESPACES;
  }
  
  namespace Hierarchy
  {
    extern const std::string HOVERTIME;
    extern const std::string RAISECORNER;
    extern const std::string POSITION;

    extern const std::string SHOW_FQ_NAMES;    
    extern const std::string WARN_MULTIPLE_PARENTS;
  }
}

#endif