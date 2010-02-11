
#include <QClipboard>
#include <QMimeData>
#include <QTime>
#include <QUuid>

#include "mnkPasswordMakerDialog.H"

mnkPasswordMakerDialog::mnkPasswordMakerDialog(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  connect(generatePushButton, SIGNAL(clicked()),
          this, SLOT(generatePassword()));

  connect(copyPushButton, SIGNAL(clicked()),
          this, SLOT(copyPassword()));

  connect(generateUuidPushButton, SIGNAL(clicked()),
          this, SLOT(generateUuid()));

  connect(copyUuidPushButton, SIGNAL(clicked()),
          this, SLOT(copyUuid()));

  this->spinBox->setValue(12);
}

mnkPasswordMakerDialog::~mnkPasswordMakerDialog(void)
{
}

void mnkPasswordMakerDialog::generatePassword(void)
{
  int i;
  QString availableCharacters = QString();
  QString generatedPassword = QString();

  if(Qt::Checked != upcaseCheckBox->checkState()
  && Qt::Checked != lowcaseCheckBox->checkState()
  && Qt::Checked != numberCheckBox->checkState()
  && Qt::Checked != otherCheckBox->checkState()) {
    passwordLineEdit->setText(tr("No valid input."));
    return;
  }

  if(true == upcaseCheckBox->isChecked()) {
    availableCharacters.append("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  }
  if(true == lowcaseCheckBox->isChecked()) {
    availableCharacters.append("abcdefghijklmnopqrstuvwyz");
  }
  if(true == numberCheckBox->isChecked()) {
    availableCharacters.append("0123456789");
  }
  if(true == otherCheckBox->isChecked()) {
    availableCharacters.append("~`!@#$%^&*()-_=+,.<>/?[]{}\\|:;'\"");
  }

  // Init random engine
  qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));

  for(i = 0; i < spinBox->value(); i ++) {
    generatedPassword.append(availableCharacters.at(qrand() % availableCharacters.size()));
    
  }
  passwordLineEdit->setText(generatedPassword);
}

void mnkPasswordMakerDialog::copyPassword(void)
{
  QMimeData *data = new QMimeData();

  if(passwordLineEdit->text().isEmpty() == false) {
    data->setText(passwordLineEdit->text());
    /* Win, OSX */
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QApplication::clipboard()->setMimeData(data);
#endif

    /* X11 */
#ifdef Q_OS_X11
    QApplication::clipboard()->setMimeData(data, QClipboard::Selection);
#endif
  }
}

void mnkPasswordMakerDialog::generateUuid(void)
{
  uuidLineEdit->setText(QUuid::createUuid().toString());
}

void mnkPasswordMakerDialog::copyUuid(void)
{
  QMimeData *data = new QMimeData();

  if(passwordLineEdit->text().isEmpty() == false) {
    data->setText(uuidLineEdit->text());
    /* Win, OSX */
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QApplication::clipboard()->setMimeData(data);
#endif

    /* X11 */
#ifdef Q_OS_X11
    QApplication::clipboard()->setMimeData(data, QClipboard::Selection);
#endif
  }
}

