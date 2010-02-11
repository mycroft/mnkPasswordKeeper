#ifndef _MNKFILEFACTORY_H_
#define _MNKFILEFACTORY_H_

#include <QObject>

#include "mnkPassword.H"

class mnkFileFactory : public QObject
{
  Q_OBJECT

public:
  mnkFileFactory();
  ~mnkFileFactory();

  QList<mnkPassword*> getListFromFile(QString); 
  bool saveListToFile(QList<mnkPassword*>);
  bool isError(void) { return lastActionFinishedInError; };

  void setFileName(QString fileName) { this->fileName = fileName; };
  QString getFileName(void) { return(this->fileName); };

  QString getLastErrorString(void) { return(lastErrorString); };

private:
  QString fileName;
  bool lastActionFinishedInError;
  QString lastErrorString;
};


#endif
