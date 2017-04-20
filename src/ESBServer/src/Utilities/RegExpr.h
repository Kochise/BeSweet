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

#ifndef RegExpr_H
#define RegExpr_H

#include "StringTokenizer.h"

#include <list>
#include <string>
#include <functional>

class RegExpr : std::unary_function<std::string, bool>
{
private:
	std::list<std::string> mTokens;
  bool mSpecial;
	
public:
	RegExpr(const std::string &expr)
  : mSpecial(expr[0] == '*')
  { mTokens = StringListTokenizer(expr, "*").tokens(); }
	
	bool hasMatch(const std::string expr)
	{
		std::list<std::string>::iterator token = mTokens.begin();
    size_t pos = 0;
    
    //item.startsWith(1stToken)
    if(mSpecial) {
      while(token != mTokens.end() && pos != std::string::npos) {
        pos = expr.find(*token, pos);
        if(pos != std::string::npos) {
          pos = token->length();
          token++;
        }
      }
    } else if(expr.find(*token) == 0) {
      while(token != mTokens.end() && pos != std::string::npos) {
        pos = expr.find(*token, pos);
        if(pos != std::string::npos) {
          pos = token->length();
          token++;
        }
      }
    }

    return token == mTokens.end();
  }

	bool operator()(const std::string &expr)
	{ return hasMatch(expr); } 


};

#endif
