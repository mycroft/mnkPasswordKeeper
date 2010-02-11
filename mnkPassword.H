#ifndef _MNKPASSWORD_H_
#define _MNKPASSWORD_H_

#include <QString>
#include <QMetaType>

#include "mnkCodingTools.H"

class mnkPassword
{
public:
  mnkPassword(void) {};
  ~mnkPassword(void) {};

public:
  QString site;
  QString username;
  QString password;
  QString comment;

public:
  bool operator==(const mnkPassword&);

  static QString encode(QString);
  static QString decode(QString);
};

Q_DECLARE_METATYPE(mnkPassword);
Q_DECLARE_METATYPE(mnkPassword*);

#endif
