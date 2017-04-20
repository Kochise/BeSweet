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

#ifndef StringTokenizer_H
#define StringTokenizer_H

#include <string>
#include <list>
#include <vector>

template<class ContainerType>
class StringTokenizer
{
private:
  ContainerType mTokens;

public:
  StringTokenizer(const std::string &str, const std::string &delimiter)
  {
    std::string::size_type tokenEnd = str.find_first_not_of(delimiter, 0);
    std::string::size_type tokenStart     = str.find_first_of(delimiter, tokenEnd);

    while (std::string::npos != tokenStart || std::string::npos != tokenEnd)
    {
       mTokens.push_back(str.substr(tokenEnd, tokenStart - tokenEnd));
       tokenEnd = str.find_first_not_of(delimiter, tokenStart);
       tokenStart = str.find_first_of(delimiter, tokenEnd);
    }
  }

  const ContainerType& tokens() const
  { return mTokens; }

};

typedef StringTokenizer<std::list<std::string> > StringListTokenizer;
typedef StringTokenizer<std::vector<std::string> > StringVectorTokenizer;

#endif
