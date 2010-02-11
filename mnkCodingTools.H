#ifndef _MNKCODINGTOOLS_H_
#define _MNKCODINGTOOLS_H_

#include <QByteArray>
#include <QString>

class mnkCodingTools 
{
public:
  static const char Base64EncMap[64];
  static const char Base64DecMap[128];
  static const char UUEncMap[64];
  static const char UUDecMap[128];

public:
  static void base64Encode(const QByteArray &,
                           QByteArray &,
                           bool insertLFs = false,
                           int lineLimit = 76);
  static void base64Decode(const QByteArray &, QByteArray &);
  static void uuEncode(const QByteArray &, QByteArray &);
  static void uuDecode(const QByteArray &, QByteArray &);
  
  static QString base64Encode(const QByteArray &,
                              bool insertLFs = false,
                              int lineLimit = 76);
  static QString base64Decode(const QByteArray &);
  static QString uuEncode(const QByteArray &);
  static QString uuDecode(const QByteArray &);
  static QByteArray uuDecodeToByteArray(const QByteArray &);
  
  static QString base64Encode(const QString &,
                              bool insertLFs = false,
                              int lineLimit = 76);
  static QString base64Decode(const QString &);
  static QString uuEncode(const QString &);
  static QString uuDecode(const QString &);
};

#endif
