#include "stdafx.h"
#include "CommandHandler.h"

#include <DSAddin.h>

void CommandHandler::showBrowser(const std::string &browser)
{
  getAddIn()->showBrowser(browser, getSelectedText());
}


void CommandHandler::properties()
{
  getAddIn()->modifyProperties();
}

std::string CommandHandler::getSelectedText()
{
  return "";
}

