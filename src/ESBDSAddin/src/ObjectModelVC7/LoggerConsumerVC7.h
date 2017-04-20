#ifndef LoggerConsumerVC7_H
#define LoggerConsumerVC7_H

namespace VisualStudio7 {

class LoggerConsumer : public ::LoggerConsumer
{
private:
  CComPtr<EnvDTE::OutputWindowPane> mSink;

public:
  LoggerConsumer(EnvDTE::_DTE *dte)
  {
    try {

      getPanes(dte)->Add(CComBSTR("BeSweet"), &mSink);
    } catch(...) {}
  }

  ~LoggerConsumer() 
  {
    //getPanes()-> Remove(CComBSTR("BeSweet"));
    mSink = 0;
  }
  void logEntry(const std::string &msg)
  {
    try {
      mSink->OutputString(CComBSTR(msg.c_str()));
    } catch(...) {}
  }

private:
  CComPtr<EnvDTE::OutputWindowPanes> getPanes(EnvDTE::_DTE *dte) 
  {
    CComPtr<EnvDTE::Windows> windows;
    cex_ = dte->get_Windows(&windows);

    CComPtr<EnvDTE::Window> window;
    cex_ = windows->Item(CComVariant(EnvDTE::vsWindowKindOutput), &window);
  
    CComPtr<IDispatch> tmp;
    cex_ = window->get_Object(&tmp);
    CComQIPtr<EnvDTE::OutputWindow> out(tmp);

    CComPtr<EnvDTE::OutputWindowPanes> panes;
    out->get_OutputWindowPanes(&panes);

    return panes;
  }
};

} //namespace VisualStudio7

#endif

