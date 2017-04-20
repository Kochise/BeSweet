#ifndef CommandHandler_H
#define CommandHandler_H

class BeSweetAddin;

#include <string>

//template<class AddinType>
class CommandHandler
{

public:
  CommandHandler() : mAddin(NULL) {}
  virtual ~CommandHandler(){}
  
  virtual void properties();
  virtual void showBrowser(const std::string &browser);

  void setAddIn(BeSweetAddin *addin) { mAddin = addin; }
  BeSweetAddin* getAddIn() { return mAddin; }

protected:
  virtual std::string getSelectedText();

private:
  BeSweetAddin *mAddin;

};

//#include <DSAddin.h>
//
//void CommandHandler::showBrowser(const std::string &browser)
//{
//  getAddIn()->showBrowser(browser, getSelectedText());
//}
//
//
//void CommandHandler::properties()
//{
//  getAddIn()->modifyProperties();
//}
//
//std::string CommandHandler::getSelectedText()
//{
//  return "";
//}
#endif