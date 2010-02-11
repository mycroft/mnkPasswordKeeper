#ifndef _MNKPASSWORDMAKERDIALOG_HPP_
#define _MNKPASSWORDMAKERDIALOG_HPP_

#include <QDialog>
#include "ui_mnkPasswordMaker.h"

class mnkPasswordMakerDialog : public QDialog, public Ui::Dialog
{
  Q_OBJECT

public:
  mnkPasswordMakerDialog(QWidget *parent = 0);
  ~mnkPasswordMakerDialog();

public slots:
  void generatePassword(void);
  void copyPassword(void);
  void generateUuid(void);
  void copyUuid(void);
};

#endif
