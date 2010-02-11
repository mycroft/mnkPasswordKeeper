#include <stdio.h>

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "mnkPasswordKeeperMainWindow.H"


int main(int argn, char **argv)
{
  QApplication app(argn, argv);

  QTranslator qtTranslator;
  QTranslator myappTranslator;

  if(QLocale::system().name().contains("fr_FR") == true) {
    qtTranslator.load("mnkPasswordKeeper_french", ".");
    app.installTranslator(&qtTranslator);
  }

  mnkPasswordKeeperMainWindow mw;
  mw.show();

  return app.exec();
}
