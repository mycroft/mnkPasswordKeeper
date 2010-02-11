
#include <QDomDocument>
#include <QDomElement>

#include <QFile>
#include <QObject>

#include <QMessageBox>
#include <QFileDialog>

#include "mnkEncryptionEngine.H"
#include "mnkFileFactory.H"

#define ENCODE(x) mnkPassword::encode(mnkCodingTools::uuEncode(x))
#define DECODE(x) \
    mnkCodingTools::uuDecodeToByteArray(mnkPassword::decode(x).toAscii())

mnkFileFactory::mnkFileFactory()
{
  fileName = QString();
  lastActionFinishedInError = false;
  lastErrorString = QString();
}

mnkFileFactory::~mnkFileFactory()
{

}

QList<mnkPassword*> mnkFileFactory::getListFromFile(QString fileName)
{
  QDomElement root, child;
  QDomDocument domDocument;
  QString errorTitle, errorStr, tmpStr;
  int errorLine, errorColumn;
  QString site, password, username, comment;
  QList<mnkPassword*> passList;
  
  QFile file(fileName);

  lastActionFinishedInError = false;

  if(file.open(QIODevice::ReadWrite) == false) {
    errorTitle = QString(tr("Error")); 
    errorStr = QString(tr("Can't open file: %1").arg(fileName));

    QMessageBox::critical(NULL, errorTitle, errorStr);

    lastActionFinishedInError = true;
    return(QList<mnkPassword*>());
  }

  setFileName(fileName);
  QByteArray contents = file.readAll();

  QByteArray decodedContents = mnkCodingTools::uuDecodeToByteArray(contents);
  tmpStr = CryptInterface->decryptArray(decodedContents);

  if(domDocument.setContent(tmpStr,
                            true,
                            &errorStr,
                            &errorLine,
                            &errorColumn) == false) {
    /* error */
    QString errTitle = QString(tr("Database reading error"));
    QString errMessage = tr("Problem while reading password database.\n"
                            "Are you sure the password is correct ?");

    QMessageBox::critical(NULL, errTitle, errMessage);

    lastActionFinishedInError = true;
    return(QList<mnkPassword*>());
  }

  root = domDocument.documentElement();

  child = root.firstChildElement("password");
  while(child.isNull() == false) {

    site = DECODE(child.firstChildElement("site").text());
    username = DECODE(child.firstChildElement("username").text());
    password = DECODE(child.firstChildElement("password").text());
    comment = DECODE(child.firstChildElement("comment").text());

    mnkPassword *newPass = new mnkPassword;
    newPass->site = site;
    newPass->username = username;
    newPass->password = password;
    newPass->comment = comment;

    passList.append(newPass);

    child = child.nextSiblingElement("password");
  }

  return(passList);
}


bool mnkFileFactory::saveListToFile(QList<mnkPassword*> passList)
{
  lastActionFinishedInError = false;
  lastErrorString = QString();

  QString tmpStr = QString("<pass>\n");

  for(int i = 0; i < passList.count(); i ++) {
    mnkPassword *m1 = passList.at(i);
    tmpStr+= QString("  <password>\n");
    tmpStr+= QString("    <site>%1</site>\n").arg(ENCODE(m1->site));
    tmpStr+= QString("    <username>%1</username>\n").arg(ENCODE(m1->username));
    tmpStr+= QString("    <password>%1</password>\n").arg(ENCODE(m1->password));
    tmpStr+= QString("    <comment>%1</comment>\n").arg(ENCODE(m1->comment));
    tmpStr+= QString("  </password>\n");
  }

  tmpStr += QString("</pass>\n\n");
  tmpStr = mnkCodingTools::uuEncode(CryptInterface->encryptArray(tmpStr.toAscii()));

  if(this->fileName.isEmpty() == true) {
    this->fileName = QFileDialog::getSaveFileName(NULL,
                                                  tr("Save File"),
                                                  "",
                                                  tr("Password Files (*.xmls)"));
  }

  if(this->fileName.isEmpty() == true) {
    lastErrorString = tr("Error while saving file: No file name.");
    return(false);
  }

  if(this->fileName.contains(".xmls") == false)
    this->fileName += QString(".xmls");

  QFile db(this->fileName);
  bool ret = db.open(QIODevice::WriteOnly);
  if(ret == false) {
    QMessageBox::warning(NULL, tr("Can't save file"),
                         db.errorString());
  } else {
    int ret = db.write(tmpStr.toAscii());
    if(ret == tmpStr.toAscii().size()) {
      lastErrorString = tr("File %1 saved.").arg(this->fileName);
    } else {
      lastErrorString = tr("Error while saving file.");
    }
    db.close();
  }

  return(true);
}

