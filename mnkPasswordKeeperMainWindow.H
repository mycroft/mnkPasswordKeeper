#ifndef _MNKPASSWORDKEEPERMAINWINDOW_H_
#define _MNKPASSWORDKEEPERMAINWINDOW_H_

#include <QString>

#define VERSION "1.1.0"

#include "mnkPassword.H"
#include "mnkPasswordMakerDialog.H"
#include "mnkFileFactory.H"

#include "ui_mnkPasswordKeeper.h"

class mnkPasswordKeeperMainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT

public:
  mnkPasswordKeeperMainWindow(QMainWindow *parent = NULL);
  ~mnkPasswordKeeperMainWindow();
  bool openReadFile(QString fileName);

  void enableEditor(void);
  void disableEditor(void);
  QString showMasterDialog(QString message = QString());

public slots:
  void removeButtonPushed(void);
  void newButtonPushed(void);

//  void addButtonPushed(void);
//  void deleteButtonPushed(void);
  void applyButtonPushed(void);
  void applyTextChanged(const QString &);
//  void clearButtonPushed(void);

  void fileOpen(const QString &path = QString());
  void fileSave(void);
  void fileSaveAs(const QString &path = QString());
  bool fileClose(void);
  void fileExit(void);
  void dockShow(void);
  void aboutShow(void);
  void filterFocus(void);

  void itemChanged(QListWidgetItem*, QListWidgetItem*);

  void reloadList(void);
  void filterEdited(const QString &);
  void showMasterDialogSlot(void);

  void showGenerator(void);

  void closeEvent(QCloseEvent *);
  void changeStyle(QAction *);
  void lock(void);
  void exportTextFile(void);

  bool askForMasterKey(QString, bool);

private:
  QString fileName;
  QList<mnkPassword*> passwordList;
  mnkFileFactory* fileFactory;

  mnkPassword* currentPassword;
  QListWidgetItem* currentItem;

  bool isModified;

  mnkPasswordMakerDialog *generatorDialog;

  QAction *actionFilterFocus;

  QString masterPassword;
};

#define DEFAULTFILE QString("passworddb.xmls")

/* Default status timeout */
#define STM 60000

#endif
