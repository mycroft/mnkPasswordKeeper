#ifndef _MNKENCRYPTIONENGINE_H_
#define _MNKENCRYPTIONENGINE_H_

#include <QByteArray>

#include "mnkCrypt.H"

class mnkEncryptionEngine {
private:
  mnkEncryptionEngine();
  ~mnkEncryptionEngine();

public:
  static mnkEncryptionEngine* getInstance();
  static void killInstance(void);

  void setEncryptionKey(QByteArray);
  QByteArray encryptArray(QByteArray);
  QByteArray decryptArray(QByteArray);

private:
  static mnkEncryptionEngine *_singleton;
  mnkCrypt *interface;
};

#define CryptInterface mnkEncryptionEngine::getInstance()

#endif
