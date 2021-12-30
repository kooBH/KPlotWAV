#include "KWidgetPlotWAV.h"
#include <QApplication>

int main(int argc, char* argv[]) {

  QApplication app(argc, argv);

#ifdef _WIN32
//  ShowWindow(::GetConsoleWindow(), SW_HIDE);
  ShowWindow(::GetConsoleWindow(), SW_SHOW);

  KWidgetPlotWAV widget;
  widget.show();
 #endif
  return app.exec();
}