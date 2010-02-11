#ifndef _MNKCRYPT_H_
#define _MNKCRYPT_H_

#include <QString>
#include <QByteArray>

#define CRYPT_KEYLEN 256
#define CRYPT_BUFFSIZE 256

typedef unsigned int uint;

class mnkCrypt
{
public:
  mnkCrypt(void);
  ~mnkCrypt(void);

  static uchar *mnkEncrypt(uchar *pt, uchar *cr_key, uint inlen, uint *outlen);
  static uchar *mnkDecrypt(uchar *cr, uchar *cr_key, uint inlen, uint *outlen);

  void scSetKey(QByteArray inputkey);
  void mnkInitKey(uchar *key_tab, uchar *crypt_key);
  void mnkSetKey(QByteArray inputkey);


  QByteArray mnkEncryptArray(QByteArray);
  QByteArray mnkEncryptArray(QString);
  QByteArray mnkDecryptArray(QByteArray);

  static QByteArray mnkGetHash(const uchar *data, qint64 maxSize);
  static void mnkPrintHex(const uchar *str, int bytes);

private:
  uchar *cryptKey;
};

#endif
