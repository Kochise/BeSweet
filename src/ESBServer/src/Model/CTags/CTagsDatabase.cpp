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

#include "stdafx.h"

#include "CTagsDatabase.h"
#include "Properties.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

namespace CTags
{
   extern const std::string CTAGS_EXE;
   extern const std::string ADDITIONAL_OPTIONS;
   extern const std::string SHOW_CTAGS_WINDOW;

const int TypeIs::ANY_TYPE = eClass | eStruct | eUnion | eEnum | eTypedef;

Database::Database(const std::string& name)
{
  mTags = loadTags(name);
}

Database::Database(const list<string>& files)
{
  setupTagTable(files);
}


Database::Database(const Workspace& workspace)
{  
  setupTagTable(workspace.getFiles());
}

void Database::setupTagTable(const std::list<std::string>&files)
{
  string out = "tags.tags.txt";
  createTagFile(files, out);
  
  //mTags.splice(mTags.end(), loadTags(out));
  TagTable table = loadTags(out);
  mTags.splice(mTags.end(), table);
}

void Database::refreshTagsReferencingFile(const std::string &file)
{
  list<string> files;
  files.push_back(file);

	eraseTagsReferencingFile(file);
  setupTagTable(files);
}


void Database::eraseTagsReferencingFile(const std::string &file)
{
  mTags.erase(remove_if(mTags.begin(), mTags.end(), bind1st(isInFile(), file)), mTags.end());
}
 
Database::TagTable Database::loadTags(const std::string &dbname)
{
  ifstream tagDatabase(dbname.c_str());
  if(!tagDatabase) {
    throw invalid_argument("file <" + dbname + "> not found");
  }

  string line;
  TagTable result;

  while(!tagDatabase.eof()) {
    getline(tagDatabase, line);
    
    if(!line.empty() && line.find("!_TAG_") == string::npos) {
      try {
        result.push_back(Tag(line));

      } catch(std::exception &e) {
        log(GENERAL, CTAGS, eError) << "exception occurred while parsing line " << line << endl << e.what() << endlog;
      }
    }
  }
  return result;
}

void Database::createTagFile(const std::list<std::string> &fileList, const std::string &out)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );


    std::set<string> files(fileList.begin(), fileList.end());
    ofstream file("ctags.filelist");
    copy(files.begin(), files.end(), ostream_iterator<string>(file, "\n"));
    file.close();
    
    string options = PropertiesManager::instance().getProperty(ADDITIONAL_OPTIONS, "");
    replace_if(options.begin(), options.end(), is_cntrl, ' ');
    stringstream cmd;
    cmd << PropertiesManager::instance().getProperty(CTAGS_EXE) << " "
        << " -o " << out
        << " --excmd=pattern --sort=no --c++-types=+csumfp-d --fields=+naimKzsS "
        << options //" --options=ctags.options " 
        << " -L ctags.filelist "
        << ends;

    // Start the child process. 
    size_t flags = fileList.size() > 1 || PropertiesManager::instance().getProperty(SHOW_CTAGS_WINDOW, "0") == "1" ? 0 : CREATE_NO_WINDOW;
    
    if( !CreateProcess( 
        NULL, // No module name (use command line). 
        const_cast<char*>(cmd.str().c_str()), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        flags,                // creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
    ::MessageBox(NULL, ("failed to execute \n" + cmd.str()).c_str(), "ERROR", MB_OK | MB_ICONERROR);
    }

    //Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager& DatabaseManager::instance()
{
  static DatabaseManager theInstance;
  return theInstance;
}

smart_ptr<Database> DatabaseManager::getDatabase(const smart_ptr<Workspace> &wspace)
{
  DBMap::iterator db = mManagedDatabases.find(wspace->getName());
  if(db == mManagedDatabases.end()) {
    smart_ptr<Database> xdb(new Database(*wspace));
    mManagedDatabases[wspace->getName()] = xdb;
    return xdb;
  }

  return db->second;
}

smart_ptr<Database> DatabaseManager::removeDatabase(const smart_ptr<Workspace> &wspace)
{
  mManagedDatabases.erase(wspace->getName());
  return smart_ptr<Database>();
}

} //namespace CTags
