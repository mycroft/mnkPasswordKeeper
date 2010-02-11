
#include "mnkEncryptionEngine.H"

mnkEncryptionEngine *mnkEncryptionEngine::_singleton = NULL;

mnkEncryptionEngine::mnkEncryptionEngine()
{
  interface = new mnkCrypt();
}

mnkEncryptionEngine::~mnkEncryptionEngine()
{
  delete(interface);
}

mnkEncryptionEngine* mnkEncryptionEngine::getInstance()
{
  if(_singleton == NULL) {
    _singleton = new mnkEncryptionEngine();
  }
  return(_singleton);
}

void mnkEncryptionEngine::killInstance()
{
  if(_singleton != NULL) {
    delete(_singleton);
    _singleton = NULL;
  }
}

void mnkEncryptionEngine::setEncryptionKey(QByteArray enckey)
{
  interface->mnkSetKey(enckey);
}

QByteArray mnkEncryptionEngine::encryptArray(QByteArray encstr)
{
  return(interface->mnkEncryptArray(encstr));
}

QByteArray mnkEncryptionEngine::decryptArray(QByteArray decstr)
{
  return(interface->mnkDecryptArray(decstr));
}

