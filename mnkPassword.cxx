
#include "mnkPassword.H"

bool mnkPassword::operator==(const mnkPassword& a)
{
  if(a.password == this->password
  && a.site == this->site
  && a.username == this->username
  && a.comment == this->comment)
    return(true);
  else
    return(false);
}

QString mnkPassword::encode(QString str)
{
  str.replace(QString("&"), QString("&amp;"));
  str.replace(QString("<"), QString("&lt;"));
  str.replace(QString(">"), QString("&gt;"));

  return(str);
}

QString mnkPassword::decode(QString str)
{
  str.replace(QString("&gt;"), QString(">"));
  str.replace(QString("&lt;"), QString("<"));
  str.replace(QString("&amp;"), QString("&"));

  return(str);
}
