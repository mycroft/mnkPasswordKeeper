
#include <sys/types.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "mnkCrypt.H"
#include "Camellia.H"
#include "Whirlpool.H"

#define MNK_CRYPT_SALT "\374\306\263\354\46\76\362\167\117\275\237\36\146\321\212\131\323\152\232\274\167\333\333\246\146\375\274\43\232\133\236\234"

/* realloc for C++ */
template <typename T>
T *resize(T *array, size_t old_size, size_t new_size)
{
  T *temp = new T[new_size];
  std::copy(array, array + old_size, temp);
  delete [] array;
  return temp; 
}

mnkCrypt::mnkCrypt(void)
{
  cryptKey = NULL;
}

mnkCrypt::~mnkCrypt(void)
{
  if(cryptKey != NULL) {
    delete[] cryptKey;
  }
}

uchar *mnkCrypt::mnkEncrypt(uchar *pt, uchar *cr_key, uint inlen, uint *outlen)
{
  uchar *final_ct;
  uchar *ct = NULL;
  uint len = 0, orig_len = inlen; 
  uint reserved_memory = 0;

  uchar pt_tmp[16];
  KEY_TABLE_TYPE kt;

  if(inlen == 0 || pt == NULL || strlen((const char*)pt) == 0) {
    *outlen = 0;
    return(NULL);
  }

  reserved_memory = 16 * sizeof(uchar);
  ct = new uchar[16 * sizeof(uchar) + sizeof(uint)];
  if(ct == NULL) {
    *outlen = 0;
    return(NULL);
  }
  final_ct = ct;

  // In the sizeof(uint)'s first characters, we put the size of clear data.
  // Avoid platform problems ...
  ct[0] = inlen % 256; 
  ct[0] ^= MNK_CRYPT_SALT[16]; 
  ct[1] = (inlen / 256) % 256;
  ct[1] ^= MNK_CRYPT_SALT[17];
  ct[2] = (inlen / (256 * 256)) % 256;
  ct[2] ^= MNK_CRYPT_SALT[18];
  ct[3] = (inlen / (256 * 256 * 256)) % 256;
  ct[3] ^= MNK_CRYPT_SALT[19];

  ct += sizeof(uint);

  // In case of strlen(pt)%16 != 0
  memset((void*)pt_tmp, 0, sizeof(pt_tmp));

  Camellia_Ekeygen(CRYPT_KEYLEN, cr_key, kt);

  while(len < orig_len) {

    if(len + 32 > reserved_memory) {
    // We change the size of ct
      ct = resize(final_ct, 
                         (len) * sizeof(uchar) + sizeof(uint),
                         (len + CRYPT_BUFFSIZE) * sizeof(uchar) + sizeof(uint));

      final_ct = ct;

      reserved_memory += CRYPT_BUFFSIZE;

      ct = final_ct + sizeof(uint);
      memset(ct + len, 0, 16);
    }

    if((len+16) <= orig_len)
      Camellia_EncryptBlock(CRYPT_KEYLEN, (pt + len), kt, (ct + len));
    else {
      // We take the remaining bytes and we pad it ...
      memcpy(pt_tmp, MNK_CRYPT_SALT, 16);
      memcpy(pt_tmp, pt + len, orig_len - len);
      Camellia_EncryptBlock(CRYPT_KEYLEN, pt_tmp, kt, (ct + len));
    }

    len += 16;
  }

  *outlen = len + sizeof(uint);

  return(final_ct);
}

uchar *mnkCrypt::mnkDecrypt(uchar *cr, uchar *cr_key, uint inlen, uint *outlen)
{
  uchar *pt = NULL;
  uchar *pt_save = NULL;

  uint len = 0;
  uint reserved_memory = 0;

  KEY_TABLE_TYPE kt;

  // Avoid platform problems ...
  cr[0] ^= MNK_CRYPT_SALT[16];
  cr[1] ^= MNK_CRYPT_SALT[17];
  cr[2] ^= MNK_CRYPT_SALT[18];
  cr[3] ^= MNK_CRYPT_SALT[19];
  *outlen = 0;
  *outlen += cr[0];
  *outlen += cr[1] * 256;
  *outlen += cr[2] * 256 * 256;
  *outlen += cr[3] * 256 * 256 * 256;

  if(*outlen == 0) {
    return(NULL);
  } 

  cr += sizeof(int);

  // In cas of strlen(pt)%16 != 0
  // memset(pt_tmp, 0, sizeof(pt_tmp));

  Camellia_Ekeygen(CRYPT_KEYLEN, cr_key, kt);
  while((len+4) < inlen) {
    if(pt != NULL) {
      if(len+32 > reserved_memory) {
        pt_save = pt;
        pt = resize(pt,
                    reserved_memory,
                    reserved_memory + CRYPT_BUFFSIZE);
        if(pt == NULL) {
          *outlen = 0;
          free(pt_save);
          return(NULL);
        } else {
          pt_save = pt;
          reserved_memory += CRYPT_BUFFSIZE;
          memset(pt + len, 0, CRYPT_BUFFSIZE);
        }
      }
    } else {
      reserved_memory = CRYPT_BUFFSIZE;

      pt = new uchar[reserved_memory];
      if(pt == NULL) {
        return(NULL);
      }
      memset(pt, 0, reserved_memory);
    }

    // On va lire 16 bytes � partir de cr+len
    Camellia_DecryptBlock(CRYPT_KEYLEN, (cr + len), kt, (pt + len));

    len += 16;
  }

  if(*outlen < (len - 16) || *outlen > len) {
    delete[] pt;
    *outlen = 0;
    return(NULL);
  }

  return(pt);
}

void mnkCrypt::mnkSetKey(QByteArray inputkey)
{
  if(cryptKey == NULL) {
  } else {
    delete[] cryptKey;
    cryptKey = NULL;
  }

  cryptKey = new uchar[CRYPT_KEYLEN / 8];
  mnkInitKey(cryptKey, (uchar*)inputkey.constData());

  return;
}

void mnkCrypt::mnkInitKey(uchar *key_tab, uchar *crypt_key)
{
  unsigned int i;

  QByteArray hashedKey = mnkGetHash((const uchar*)crypt_key, 
                                    strlen((const char*)crypt_key));

// QByteArray mnkCrypt::mnkGetHash(const uchar *data, qint64 maxSize)
  for(i = 0; i < CRYPT_KEYLEN / 8; i++) {
    key_tab[i] = hashedKey[i]; 
  }

/*
  for(i = 0; i < CRYPT_KEYLEN / 8; i++) {
    key_tab[i] = crypt_key[i]
                 ^ MNK_CRYPT_SALT[(CRYPT_KEYLEN / 8) - (i + 1)];
  }
 */

  return;
}

QByteArray mnkCrypt::mnkEncryptArray(QString strtext)
{
  return(this->mnkEncryptArray(strtext.toAscii()));
}

QByteArray mnkCrypt::mnkEncryptArray(QByteArray ba)
{
  uchar *res_string;
  uint   outlen;
  QByteArray qba;

  if(this->cryptKey == NULL) {
    return(NULL);
  }

  res_string = mnkEncrypt((uchar*)ba.constData(),
                         this->cryptKey,
                         ba.size(),
                         &outlen
                         );

  for(uint i = 0; i < outlen; i ++) {
    qba[i] = res_string[i];
  }

  delete[] res_string;

  return(qba);
}

QByteArray mnkCrypt::mnkDecryptArray(QByteArray ba)
{
  uchar *res_string;
  uint   outlen;
  QByteArray qba;

  if(this->cryptKey == NULL) {
    return(NULL);
  }

  res_string = mnkDecrypt((uchar*)ba.constData(),
                         this->cryptKey,
                         ba.size(),
                         &outlen
                         );
  
  for(uint i = 0; i < outlen; i ++) {
    qba[i] = res_string[i];
  }

  delete[] res_string;

  return(qba);
}

QByteArray mnkCrypt::mnkGetHash(const uchar *data, qint64 maxSize)
{
  QByteArray returnedHash;
  struct NESSIEstruct ctx;
  uchar *hashed;

  hashed = new uchar[DIGESTBYTES];

  if(hashed == NULL)
    return(NULL);

  NESSIEinit(&ctx);
  NESSIEadd(data, maxSize * 8, &ctx);
  NESSIEfinalize(&ctx, hashed);

  returnedHash = QByteArray((const char*)hashed, DIGESTBYTES);

/*
  returnedHash = QString();
  for(int i  = 0; i < DIGESTBYTES; i ++) {
    returnedHash.append(
      QString::number(hashed[i], 16).rightJustified(2, '0', true).toUpper()
      );
  }
 */

  delete[](hashed);

  return(returnedHash);
}

void mnkCrypt::mnkPrintHex(const uchar *str, int bytes)
{
  int i;

  printf("(%d) ", bytes);
  for(i = 0; i < bytes; i ++) {
    printf("%.02X", str[i]);
  }
  printf("\n");

  return;
}
