#include <QApplication>

#include <QCloseEvent>

#include <QDomDocument>
#include <QDomElement>

#include <QFileDialog>

#include <QMessageBox>

#include <QActionGroup>
#include <QStyleFactory>

#include "mnkPasswordKeeperMainWindow.H"
#include "mnkPasswordMakerDialog.H"

#include "mnkEncryptionEngine.H"

// #define DEBUG

mnkPasswordKeeperMainWindow::mnkPasswordKeeperMainWindow(QMainWindow *parent)
  : QMainWindow(parent)
{
  setupUi(this);
  addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

  fileName = QString(DEFAULTFILE);
  currentPassword = NULL;
  currentItem = NULL;
  generatorDialog = NULL;
  fileFactory = NULL;

  connect(removeButton, SIGNAL(clicked(void)),
          this, SLOT(removeButtonPushed(void)));
  connect(newButton, SIGNAL(clicked(void)),
          this, SLOT(newButtonPushed(void)));

  connect(filterEdit, SIGNAL(textChanged(const QString&)),
          this, SLOT(filterEdited(const QString&)));

  connect(actionOpen, SIGNAL(triggered(void)),
          this, SLOT(fileOpen(void)));
  connect(actionSave, SIGNAL(triggered(void)),
          this, SLOT(fileSave(void)));
  connect(actionSave_As, SIGNAL(triggered(void)),
          this, SLOT(fileSaveAs(void)));
  connect(actionClose, SIGNAL(triggered(void)),
          this, SLOT(fileClose(void)));
  connect(actionExit, SIGNAL(triggered(void)),
          this, SLOT(fileExit(void)));

  connect(actionDock, SIGNAL(triggered(void)),
          this, SLOT(dockShow(void)));
  connect(actionAbout, SIGNAL(triggered(void)),
          this, SLOT(aboutShow(void)));
  connect(actionMaster_Password, SIGNAL(triggered(void)),
          this, SLOT(showMasterDialogSlot(void)));
  connect(actionGenerator, SIGNAL(triggered(void)),
          this, SLOT(showGenerator(void)));

  connect(siteEdit, SIGNAL(textEdited(const QString&)),
          this, SLOT(applyTextChanged(const QString&)));
  connect(usernameEdit, SIGNAL(textEdited(const QString&)),
          this, SLOT(applyTextChanged(const QString&)));
  connect(passwordEdit, SIGNAL(textEdited(const QString&)),
          this, SLOT(applyTextChanged(const QString&)));
  connect(commentTextEdit, SIGNAL(textChanged()),
          this, SLOT(applyButtonPushed()));

  connect(actionLock, SIGNAL(triggered(void)),
          this, SLOT(lock(void)));

  connect(actionText_Export, SIGNAL(triggered(void)),
          this, SLOT(exportTextFile(void)));

//  siteEdit / usernameEdit / passwordEdit / commentTextEdit

  connect(listWidget,
          SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
          this,
          SLOT(itemChanged(QListWidgetItem*, QListWidgetItem*)));

  actionFilterFocus = new QAction(this);
  actionFilterFocus->setShortcut(tr("Ctrl+F"));
  connect(actionFilterFocus, SIGNAL(triggered(void)),
          this, SLOT(filterFocus(void)));

  this->addAction(actionFilterFocus);

  /* Add styles */
  QActionGroup *ag = new QActionGroup(this);
  ag->setExclusive(true);
  for(int i = 0; i < QStyleFactory::keys().count(); i ++) {
    QAction *tmp = new QAction(QStyleFactory::keys().at(i), this);
    tmp->setCheckable(true);
    ag->addAction(tmp);
    menuWindow->addAction(tmp);

#ifndef Q_WS_MAC
    if(tmp->text() == QString(tr("Cleanlooks"))) {
#else
    if(tmp->text() == QString("Macintosh (Aqua)")) {
#endif
      tmp->setChecked(true);
      changeStyle(tmp);
    } else {
      tmp->setChecked(false);
    }
  }

  connect(ag, SIGNAL(triggered(QAction *)),
          this, SLOT(changeStyle(QAction *)));

  fileName = QString();

  CryptInterface->setEncryptionKey(QString("this is my key").toAscii());

  isModified = false;
  
  // If there is no default file: ask for opening a file or not.
  int ret = QMessageBox::Yes;
  if(QFile::exists(DEFAULTFILE) == false) {
    ret = QMessageBox::warning(this, tr("Initialization"),
                               tr("No default file found.\n"
                                  "Do you want to open another .xmls database ?\n"
                                  "Press 'No' to create a new password database."),
                               QMessageBox::Yes | QMessageBox::No);
    if(ret == QMessageBox::Yes) {
      fileOpen(QString());
    } else {
      // We don't want to choose another file. Using passworddb.xmls as default file.
      this->fileName = DEFAULTFILE;
    }
  } else {
    this->fileName = DEFAULTFILE;
  }

  int i;
  for(i = 0; i < 3; i ++)
  {
    ret = askForMasterKey(this->fileName, false);
    if(ret == false)
      break;
    ret = openReadFile(this->fileName);
    if(ret == true)
      break;
    else {
      if(i == 0)
      {
        ret = QMessageBox::warning(this, tr("Initialization"),
                                   tr("Unable to open database.\nDo you want to try another file ?"),
                                   QMessageBox::Yes | QMessageBox::No);
        if(ret == QMessageBox::Yes) {
          fileOpen(QString());
        }
      }
    }
  }

  if(ret == false)
  {
    QMessageBox::warning(this, tr("Initialization"),
                         tr("Unable to open database.\nOpening an empty editor."));
  }

  reloadList();

  // If 0 element in list, disabling editor.
  if(listWidget->currentItem() == NULL)
    disableEditor();
}

mnkPasswordKeeperMainWindow::~mnkPasswordKeeperMainWindow(void)
{
  return ;
}

void mnkPasswordKeeperMainWindow::removeButtonPushed(void)
{
  mnkPassword *myPassword;
  QListWidgetItem *myItem = listWidget->currentItem();

  if(myItem == NULL)
    return;

  myPassword = qVariantValue<mnkPassword*>(myItem->data(Qt::UserRole));

  int ret = QMessageBox::warning(this,
                                 tr("Password deletion"),
                                 tr("Do you want to delete '%1' ?")
                                   .arg(myPassword->site),
                                 QMessageBox::Yes | QMessageBox::No);

  if(ret == QMessageBox::Yes) {
    currentPassword = NULL;
    statusbar->showMessage(tr("Record removed: %1").arg(myPassword->site), STM);

    passwordList.removeAll(myPassword);
    isModified = true;
    reloadList();
  }
}

void mnkPasswordKeeperMainWindow::newButtonPushed(void)
{
  QListWidgetItem *myItem;
  mnkPassword *myPassword;

  currentPassword = NULL;
  currentItem = NULL;

  /* Search for already existing '<< New record >>' */
  for(int i = 0; i < passwordList.count(); i ++) {
    if(passwordList.at(i)->site == QString(tr("<< New record >>"))) {
      QMessageBox::warning(this, tr("Already existing"), 
                           tr("A new record already exist."));
      return;
    }
  }

  myPassword = new mnkPassword();

  listWidget->clearSelection();

  siteEdit->setText(QString(tr("<< New record >>")));
  usernameEdit->setText(QString());
  passwordEdit->setText(QString());
  commentTextEdit->setText(QString());

  myPassword->site = siteEdit->text();

  myItem = new QListWidgetItem();
  myItem->setData(Qt::UserRole, qVariantFromValue(myPassword));
  myItem->setText(QString("%1").arg(myPassword->site));

  listWidget->addItem(myItem);
  passwordList.append(myPassword);
  currentPassword = myPassword;

  listWidget->setCurrentItem(myItem);

  isModified = true;

  siteEdit->setFocus();
  siteEdit->selectAll();
}

void mnkPasswordKeeperMainWindow::applyButtonPushed(void)
{
#ifdef DEBUG
  printf("[CALL: applyButtonPushed]\n");
#endif

  mnkPassword *myPassword;

  if(currentPassword == NULL
  || currentItem == NULL)
    return;

  myPassword = currentPassword;

  if(currentItem != NULL) {
    if(usernameEdit->text() == QString()) {
      currentItem->setText(QString("%1").arg(siteEdit->text()));
	} else {
      currentItem->setText(QString("%1 (%2)")
					  .arg(siteEdit->text(), usernameEdit->text()));
    }
  }

  myPassword->site = siteEdit->text();
  myPassword->username = usernameEdit->text();
  myPassword->password = passwordEdit->text();
  myPassword->comment = commentTextEdit->toPlainText();

  statusbar->showMessage(tr("Record modified: %1").arg(myPassword->site), STM);

  isModified = true;
}

void mnkPasswordKeeperMainWindow::applyTextChanged(const QString &edt)
{
  applyButtonPushed();
}

void mnkPasswordKeeperMainWindow::fileOpen(const QString &path)
{
  int i;
  bool ret;
  QString fileName = path;

  if(isModified == true) {
    ret = fileClose();
    if(ret == false)
      return;
  } else {
    fileClose();
  }

  if(true == fileName.isNull()) {
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open File"),
                                            "",
                                            tr("Password Files (*.xmls)"));
    if(askForMasterKey(fileName, false) == false)
      return;
  }

  if(false == fileName.isEmpty()) {
    for(i = 0; i < 3; i ++)
    {
      if((ret = this->openReadFile(fileName)) == true)
        break;
      if(i != 2 && askForMasterKey(fileName, false) == false)
        return;
    }
    if(ret == false)
    {
      QMessageBox::warning(this, tr("Initialization"),
                                 tr("Unable to open database.\nOpening an empty editor."));
      return;
    }
    reloadList();
    this->fileName = fileName;
    isModified = false;
  }
}

void mnkPasswordKeeperMainWindow::fileSave(void)
{
  this->fileSaveAs(this->fileName);
  statusbar->showMessage(tr("File saved."), STM);

  return;
}

void mnkPasswordKeeperMainWindow::fileSaveAs(const QString &path)
{
  bool ret;

  if(fileFactory == NULL)
    fileFactory = new mnkFileFactory();

  fileFactory->setFileName(path);

  ret = fileFactory->saveListToFile(passwordList);

  if(ret == false) {
    statusbar->showMessage(fileFactory->getLastErrorString(), STM);
  } else {
    isModified = false;
  }

  return;
}

void mnkPasswordKeeperMainWindow::closeEvent(QCloseEvent * event)
{
  bool ret = fileClose();
  if(ret == false) {
    event->ignore();
  }
}

bool mnkPasswordKeeperMainWindow::fileClose(void)
{
  bool isClosed = true;

  if(isModified == true) {
    int ret = QMessageBox::warning(this,
                tr("Changed not saved"),
                tr("Do you want to save changes ?"),
                QMessageBox::Yes 
                | QMessageBox::No
                | QMessageBox::Cancel);
    if(ret == QMessageBox::Yes) {
      fileSave();
      isClosed = true;
    } else if(ret == QMessageBox::No) {
      isClosed = true;
    } else
      isClosed = false;
  }

  if(isClosed == true) {
    /* Close */
    listWidget->clearSelection();

    siteEdit->setText(QString());
    usernameEdit->setText(QString());
    passwordEdit->setText(QString());
    commentTextEdit->blockSignals(true);
    commentTextEdit->setText(QString());

    listWidget->clear();
    commentTextEdit->blockSignals(false);

    /* clear pwd list */
    while(passwordList.count() > 0)
      delete(passwordList.takeAt(0));

    this->fileName = QString();
  } else
    return(false);

  return(true);
}

void mnkPasswordKeeperMainWindow::fileExit(void)
{
  if(true == fileClose())
    QApplication::exit(0);
}

void mnkPasswordKeeperMainWindow::itemChanged(QListWidgetItem*current, 
                                              QListWidgetItem*previous)
{
  mnkPassword *myPassword;
  
  // remove warning:
  previous = NULL;

#ifdef DEBUG
  printf("[CALL: itemChanged]\n");
#endif

  if(current == NULL) {
    disableEditor();
  } else {
    enableEditor();
  }

  if(current == NULL) {
    currentPassword = NULL;
    currentItem = NULL;

    listWidget->clearSelection();

    siteEdit->setText(QString());
    usernameEdit->setText(QString());
    passwordEdit->setText(QString());
    commentTextEdit->setText(QString());

    return;
  }

  myPassword = qVariantValue<mnkPassword*>(current->data(Qt::UserRole));

  currentPassword = myPassword;
  currentItem = current;

  siteEdit->setText(myPassword->site);
  usernameEdit->setText(myPassword->username);
  passwordEdit->setText(myPassword->password);
  commentTextEdit->blockSignals(true);
  commentTextEdit->setPlainText(myPassword->comment);
  commentTextEdit->blockSignals(false);

  /* Change status */

  return;
}

void mnkPasswordKeeperMainWindow::reloadList(void)
{
  QListWidgetItem *myItem;
  QListWidgetItem *selectedItem = NULL;
  mnkPassword *myPassword;
  mnkPassword *selectedPassword = NULL;

  /* Get actual selected element, if any */
  if(listWidget->selectedItems().count() == 1) {
    selectedPassword = qVariantValue<mnkPassword*>
                       (listWidget->selectedItems().at(0)->data(Qt::UserRole));
  }

  listWidget->clear();

  for(int i = 0; i < passwordList.count(); i ++) {
    myPassword = passwordList.at(i);
    if(filterEdit->text().isEmpty() == false) {
      QString txt = filterEdit->text();
      if(myPassword->site.contains(txt, Qt::CaseInsensitive) == false
      && myPassword->username.contains(txt, Qt::CaseInsensitive) == false
      && myPassword->password.contains(txt, Qt::CaseInsensitive) == false
      && myPassword->comment.contains(txt, Qt::CaseInsensitive) == false)
        continue;
    }

    myItem = new QListWidgetItem();

    if(myPassword == selectedPassword) {
     selectedItem = myItem; 
    }

    myItem->setData(Qt::UserRole, qVariantFromValue(myPassword));
	if(myPassword->username == QString()) {
      myItem->setText(QString("%1")
                      .arg(myPassword->site));
	} else {
      myItem->setText(QString("%1 (%2)")
                      .arg(myPassword->site, myPassword->username));
    }
    listWidget->addItem(myItem);
  }
  
  listWidget->sortItems();

  if(selectedItem != NULL) {
    listWidget->setCurrentItem(selectedItem);
  } else {
    if(listWidget->count() > 0) {
      listWidget->setCurrentItem(listWidget->item(0));
    }
  }

  if(listWidget->count() <= 1)
    elementLabel->setText(QString(tr("%1 element"))
                             .arg(QString::number(listWidget->count())));
  else
    elementLabel->setText(QString(tr("%1 elements"))
                             .arg(QString::number(listWidget->count())));

  return;
}

bool mnkPasswordKeeperMainWindow::openReadFile(QString fileName)
{
  QList<mnkPassword*> passList;

  if(fileFactory == NULL)
    fileFactory = new mnkFileFactory();

  passList = fileFactory->getListFromFile(fileName);

  if(passList.count() == 0 && fileFactory->isError() == true)
    return(false);
  else {
    passwordList += passList;
  } 
  
  return(true);
}

void mnkPasswordKeeperMainWindow::filterEdited(const QString &txt)
{
  reloadList();
}

void mnkPasswordKeeperMainWindow::dockShow(void)
{
  if(dockWidget->isHidden() == true)
    dockWidget->show();
  else
    dockWidget->hide();
}

void mnkPasswordKeeperMainWindow::aboutShow(void)
{
  QString info = QString(tr("Designed in Toulouse, France\n"));
  info += QString(tr("By Patrick MARIE <mycroft@monkeyz.eu>\n"));
  info += QString(tr("Check for updates on http://www.monkeyz.eu/ !\n"));
  info += QString(tr("Version: %1\n").arg(QString(VERSION)));
  info += QString(tr("Compiled with Qt %1").arg(QString(qVersion())));
  QMessageBox::information(this,
                    tr("About this software"),
                    info);
}

void mnkPasswordKeeperMainWindow::filterFocus(void)
{
  filterEdit->setFocus();
}

void mnkPasswordKeeperMainWindow::showMasterDialogSlot(void)
{
  QString masterPassword = showMasterDialog();
  if(masterPassword == QString()) {
    return;
  }

  CryptInterface->setEncryptionKey(masterPassword.toAscii());
}

QString mnkPasswordKeeperMainWindow::showMasterDialog(QString message)
{
  QDialog *myDialog;
  QWidget *buttons;
  QPushButton *okb, *notokb;
  QLabel *lab;
  QLineEdit *le;
  QHBoxLayout *hl;
  QVBoxLayout *vl;

  myDialog = new QDialog(this);
  myDialog->setWindowTitle(tr("Master password"));

  okb = new QPushButton(myDialog);
  okb->setText(tr("Validate"));

  notokb = new QPushButton(myDialog);
  notokb->setText(tr("Cancel"));

  hl = new QHBoxLayout;
  hl->addWidget(okb);
  hl->addWidget(notokb);

  buttons = new QWidget(myDialog);
  buttons->setLayout(hl);

  lab = new QLabel(myDialog);
  if(message == QString()) {
    lab->setText(tr("Enter new Master password"));
  } else {
    lab->setText(message);
  }
  
  le = new QLineEdit(myDialog);
  le->setEchoMode(QLineEdit::Password);

  vl = new QVBoxLayout;
  vl->addWidget(lab);
  vl->addWidget(le);
  vl->addWidget(buttons);
  
  myDialog->setLayout(vl);

  connect(okb, SIGNAL(clicked()),
          myDialog, SLOT(accept()));
  connect(notokb, SIGNAL(clicked()),
          myDialog, SLOT(reject()));
  
  le->setFocus();
  myDialog->exec();

  if(myDialog->result() == QDialog::Accepted) {
    return(le->text());
  }
  return(QString());
}

void mnkPasswordKeeperMainWindow::showGenerator(void)
{
  if(generatorDialog == NULL)
    generatorDialog = new mnkPasswordMakerDialog(this);

  if(generatorDialog->isHidden())
    generatorDialog->show();
  else
    generatorDialog->hide();
}

void mnkPasswordKeeperMainWindow::changeStyle(QAction *act)
{
  QString style = act->text();
  QApplication::setStyle(QStyleFactory::create(style));
  QApplication::setPalette(QApplication::style()->standardPalette());
}

void mnkPasswordKeeperMainWindow::enableEditor(void)
{
  siteEdit->setEnabled(true);
  usernameEdit->setEnabled(true);
  passwordEdit->setEnabled(true);
  commentTextEdit->setEnabled(true);
}

void mnkPasswordKeeperMainWindow::disableEditor(void)
{
  siteEdit->setEnabled(false);
  usernameEdit->setEnabled(false);
  passwordEdit->setEnabled(false);
  commentTextEdit->setEnabled(false);
}

void mnkPasswordKeeperMainWindow::lock(void)
{
  QString password;

  filterEdit->setText(QString());
  listWidget->clear();
  itemChanged(NULL, NULL);

  siteEdit->setEnabled(false);
  usernameEdit->setEnabled(false);
  passwordEdit->setEnabled(false);
  commentTextEdit->setEnabled(false);
  removeButton->setEnabled(false);
  newButton->setEnabled(false);
  listWidget->setEnabled(false);
  filterEdit->setEnabled(false);

  // actionLock
  do {
    password = showMasterDialog();
    if(password == QString()) {
      QMessageBox::critical(this,
                            tr("Error"),
                            tr("Wrong password."));
      exit(-1);
    }
  } while(password != masterPassword);

  siteEdit->setEnabled(true);
  usernameEdit->setEnabled(true);
  passwordEdit->setEnabled(true);
  commentTextEdit->setEnabled(true);
  removeButton->setEnabled(true);
  newButton->setEnabled(true);
  listWidget->setEnabled(true);
  filterEdit->setEnabled(true);

  reloadList();
}

void mnkPasswordKeeperMainWindow::exportTextFile(void)
{
  mnkPassword *myPassword = NULL;

  if(passwordList.count() == 0) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Can't export: No password."));
    return;
  }

  QFile exportFile(QString("passwords.txt"));

  exportFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
  if(exportFile.isOpen() == false) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Can't export: Error while opening file."));
    return;
  }

  for(int i = 0; i < passwordList.count(); i ++) {
    myPassword = passwordList.at(i);

    exportFile.write(QString("Sitename: %1\n").arg(myPassword->site).toAscii());
    exportFile.write(QString("Username: %1\n").arg(myPassword->username).toAscii());
    exportFile.write(QString("Password: %1\n").arg(myPassword->password).toAscii());
    exportFile.write(QString("Comments:\n").toAscii());
    QStringList myList = myPassword->comment.split(QRegExp("\n"));
    for(int j = 0; j < myList.count(); j ++) {
      exportFile.write(QString("# %1\n").arg(myList.at(j)).toAscii());
    }
    exportFile.write(QString("--\n\n").toAscii());
  }

  exportFile.close();

  QMessageBox::information(this,
                           tr("Export"),
                           tr("Export is finished."));
}

bool mnkPasswordKeeperMainWindow::askForMasterKey(QString fileName,
                                                  bool exitIfEmpty)
{
  masterPassword = showMasterDialog(tr("Please enter your master password\n"
                    "This password will be used to encrypt your database\n\n"
                    "Your database will be stored in the file \"%1\"")
                                  .arg(fileName));

  if(masterPassword == QString()) {
    QMessageBox::critical(this, tr("Error"), tr("No master password defined."));
    if(exitIfEmpty == true)
      exit(-1);
    return(false);
  } else {
    CryptInterface->setEncryptionKey(masterPassword.toAscii());
  }
  return(true);
}
