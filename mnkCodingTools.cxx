/*
 * Most of this source code have been taken from kmdcodec.cpp from the KDE
 * project. This file is tagged to be a LGPL licensed file.
 *
 */


#include "mnkCodingTools.H"

const char mnkCodingTools::Base64EncMap[64] =
{
  0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
  0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
  0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
  0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
  0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
  0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
};

const char mnkCodingTools::Base64DecMap[128] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
  0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
  0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
  0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
  0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
  0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
  0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00
};

const char mnkCodingTools::UUEncMap[64] =
{
  0x60, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F
};

const char mnkCodingTools::UUDecMap[128] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// linelimit must be a multiple to 4. (linelimit % 4 == 0.)
void mnkCodingTools::base64Encode(const QByteArray &in, QByteArray &out, bool insertLFs, int linelimit)
{
    // clear out the output buffer
    out.resize (0);
    if ( in.isEmpty() )
        return;

    unsigned int sidx = 0;
    unsigned int didx = 0;
    const char* data = in.data();
    const unsigned int len = in.size();
    
//    unsigned int linelimit = 76;

    unsigned int out_len = ((len+2)/3)*4;
    
    // Deal with the 76 characters or less per
    // line limit specified in RFC 2045 on a
    // pre request basis.
    insertLFs = (insertLFs && out_len > linelimit);
    if ( insertLFs )
      out_len += ((out_len-1)/linelimit);

    int count = 0;
    out.resize( out_len );

    // 3-byte to 4-byte conversion + 0-63 to ascii printable conversion
    if ( len > 1 )
    {
        while (sidx < len-2)
        {
            if ( insertLFs )
            {
                if ( count && (count%linelimit) == 0 )
                    out[didx++] = '\n';
                count += 4;
            }
            out[didx++] = Base64EncMap[(data[sidx] >> 2) & 077];
            out[didx++] = Base64EncMap[(data[sidx+1] >> 4) & 017 |
                                       (data[sidx] << 4) & 077];
            out[didx++] = Base64EncMap[(data[sidx+2] >> 6) & 003 |
                                       (data[sidx+1] << 2) & 077];
            out[didx++] = Base64EncMap[data[sidx+2] & 077];
            sidx += 3;
        }
    }

    if (sidx < len)
    {
        if ( insertLFs && (count > 0) && (count%linelimit) == 0 )
           out[didx++] = '\n';

        out[didx++] = Base64EncMap[(data[sidx] >> 2) & 077];
        if (sidx < len-1)
        {
            out[didx++] = Base64EncMap[(data[sidx+1] >> 4) & 017 |
                                       (data[sidx] << 4) & 077];
            out[didx++] = Base64EncMap[(data[sidx+1] << 2) & 077];
        }
        else
        {
            out[didx++] = Base64EncMap[(data[sidx] << 4) & 077];
        }
    }

    // Add padding
    while (didx < out.size())
    {
        out[didx] = '=';
        didx++;
    }


  return;
}

void mnkCodingTools::base64Decode(const QByteArray &in, QByteArray &out)
{
    out.resize(0);
    if ( in.isEmpty() )
        return;

    unsigned int count = 0;
    unsigned int len = in.size(), tail = len;
    const char* data = in.data();

    // Deal with possible *nix "BEGIN" marker!!
    while ( count < len && (data[count] == '\n' || data[count] == '\r' ||
            data[count] == '\t' || data[count] == ' ') )
        count++;

    if ( count == len )
        return;

    if ( strncasecmp(data+count, "begin", 5) == 0 )
    {
        count += 5;
        while ( count < len && data[count] != '\n' && data[count] != '\r' )
            count++;

        while ( count < len && (data[count] == '\n' || data[count] == '\r') )
            count ++;

        data += count;
        tail = (len -= count);
    }

    // Find the tail end of the actual encoded data even if
    // there is/are trailing CR and/or LF.
    while ( tail > 0
            && ( data[tail-1] == '=' || data[tail-1] == '\n' || data[tail-1] == '\r' ) )
        if ( data[--tail] != '=' ) len = tail;

    unsigned int outIdx = 0;
    out.resize( (count=len) );
    for (unsigned int idx = 0; idx < count; idx++)
    {
        // Adhere to RFC 2045 and ignore characters
        // that are not part of the encoding table.
        unsigned char ch = data[idx];
        if ((ch > 47 && ch < 58) || (ch > 64 && ch < 91) ||
            (ch > 96 && ch < 123) || ch == '+' || ch == '/' || ch == '=')
        {
            out[outIdx++] = Base64DecMap[ch];
        }
        else
        {
            len--;
            tail--;
        }
    }

    // kdDebug() << "Tail size = " << tail << ", Length size = " << len << endl;

    // 4-byte to 3-byte conversion
    len = (tail>(len/4)) ? tail-(len/4) : 0;
    unsigned int sidx = 0, didx = 0;
    if ( len > 1 )
    {
      while (didx < len-2)
      {
          out[didx] = (((out[sidx] << 2) & 255) | ((out[sidx+1] >> 4) & 003));
          out[didx+1] = (((out[sidx+1] << 4) & 255) | ((out[sidx+2] >> 2) & 017));
          out[didx+2] = (((out[sidx+2] << 6) & 255) | (out[sidx+3] & 077));
          sidx += 4;
          didx += 3;
      }
    }

    if (didx < len)
        out[didx] = (((out[sidx] << 2) & 255) | ((out[sidx+1] >> 4) & 003));

    if (++didx < len )
        out[didx] = (((out[sidx+1] << 4) & 255) | ((out[sidx+2] >> 2) & 017));

    // Resize the output buffer
    if ( len == 0 || len < out.size() )
      out.resize(len);
}

QString mnkCodingTools::base64Encode(const QByteArray &in, bool insertLFs, int linelimit)
{
  if(in.isEmpty())
    return QString("");

  QByteArray out;
  
  base64Encode(in, out, insertLFs, linelimit);
  
  return QString(out);
}

QString mnkCodingTools::base64Encode(const QString &in, bool insertLFs, int linelimit)
{
  if(in.isEmpty())
    return QString("");

  QByteArray intmp;
  QByteArray out;
  intmp.append(in);
  
  base64Encode(intmp, out, insertLFs, linelimit);
  
  return QString(out);
}

QString mnkCodingTools::base64Decode(const QByteArray &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray out;
  
  base64Decode(in, out);
  
  return QString(out);
}


QString mnkCodingTools::base64Decode(const QString &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray intmp;
  QByteArray out;
  intmp.append(in);
  
  base64Decode(intmp, out);
  return QString(out);
}

QString mnkCodingTools::uuEncode(const QString &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray intmp;
  QByteArray out;
  intmp.append(in);
  
  uuEncode(intmp, out);
  
  return QString(out);
}

QString mnkCodingTools::uuEncode(const QByteArray &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray out;
  
  uuEncode(in, out);
  
  return QString(out);
}

QString mnkCodingTools::uuDecode(const QString &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray intmp;
  QByteArray out;
  intmp.append(in);
  
  uuDecode(intmp, out);
  
  return QString(out);
}

QString mnkCodingTools::uuDecode(const QByteArray &in)
{
  if(in.isEmpty())
    return QString("");

  QByteArray out;
  
  uuDecode(in, out);
  
  return QString(out);
}

QByteArray mnkCodingTools::uuDecodeToByteArray(const QByteArray &in)
{
  if(in.isEmpty())
    return QByteArray();

  QByteArray out;
  uuDecode(in, out);
  return(out);
}

void mnkCodingTools::uuEncode(const QByteArray &in, QByteArray &out)
{
    out.resize( 0 );
    if( in.isEmpty() )
        return;

    unsigned int sidx = 0;
    unsigned int didx = 0;
    unsigned int line_len = 45;

    const char nl[] = "\n";
    const char* data = in.data();
    const unsigned int nl_len = strlen(nl);
    const unsigned int len = in.size();

    out.resize( (len+2)/3*4 + ((len+line_len-1)/line_len)*(nl_len+1) );
    // split into lines, adding line-length and line terminator
    while (sidx+line_len < len)
    {
        // line length
        out[didx++] = UUEncMap[line_len];

        // 3-byte to 4-byte conversion + 0-63 to ascii printable conversion
        for (unsigned int end = sidx+line_len; sidx < end; sidx += 3)
        {
            out[didx++] = UUEncMap[(data[sidx] >> 2) & 077];
            out[didx++] = UUEncMap[(data[sidx+1] >> 4) & 017 |
                                   (data[sidx] << 4) & 077];
            out[didx++] = UUEncMap[(data[sidx+2] >> 6) & 003 |
                                (data[sidx+1] << 2) & 077];
            out[didx++] = UUEncMap[data[sidx+2] & 077];
        }

        // line terminator
        //for (unsigned int idx=0; idx < nl_len; idx++)
        //out[didx++] = nl[idx];
        memcpy(out.data()+didx, nl, nl_len);
        didx += nl_len;
    }

    // line length
    out[didx++] = UUEncMap[len-sidx];
    // 3-byte to 4-byte conversion + 0-63 to ascii printable conversion
    while (sidx+2 < len)
    {
        out[didx++] = UUEncMap[(data[sidx] >> 2) & 077];
        out[didx++] = UUEncMap[(data[sidx+1] >> 4) & 017 |
                               (data[sidx] << 4) & 077];
        out[didx++] = UUEncMap[(data[sidx+2] >> 6) & 003 |
                               (data[sidx+1] << 2) & 077];
        out[didx++] = UUEncMap[data[sidx+2] & 077];
        sidx += 3;
    }

    if (sidx < len-1)
    {
        out[didx++] = UUEncMap[(data[sidx] >> 2) & 077];
        out[didx++] = UUEncMap[(data[sidx+1] >> 4) & 017 |
                               (data[sidx] << 4) & 077];
        out[didx++] = UUEncMap[(data[sidx+1] << 2) & 077];
        out[didx++] = UUEncMap[0];
    }
    else if (sidx < len)
    {
        out[didx++] = UUEncMap[(data[sidx] >> 2) & 077];
        out[didx++] = UUEncMap[(data[sidx] << 4) & 077];
        out[didx++] = UUEncMap[0];
        out[didx++] = UUEncMap[0];
    }

    // line terminator
    memcpy(out.data()+didx, nl, nl_len);
    didx += nl_len;

    // sanity check
    if ( didx != out.size() )
        out.resize( 0 );
}

void mnkCodingTools::uuDecode(const QByteArray &in, QByteArray &out)
{

    out.resize( 0 );
    if( in.isEmpty() )
        return;

    unsigned int sidx = 0;
    unsigned int didx = 0;
    unsigned int len = in.size();
    unsigned int line_len, end;
    const char* data = in.data();

    // Deal with *nix "BEGIN"/"END" separators!!
    unsigned int count = 0;
    while ( count < len && (data[count] == '\n' || data[count] == '\r' ||
            data[count] == '\t' || data[count] == ' ') )
        count ++;

    bool hasLF = false;
    if ( strncasecmp( data+count, "begin", 5) == 0 )
    {
        count += 5;
        while ( count < len && data[count] != '\n' && data[count] != '\r' )
            count ++;

        while ( count < len && (data[count] == '\n' || data[count] == '\r') )
            count ++;

        data += count;
        len -= count;
        hasLF = true;
    }

    out.resize( len/4*3 );
    while ( sidx < len )
    {
        // get line length (in number of encoded octets)
        line_len = UUDecMap[ (unsigned char) data[sidx++]];
        // ascii printable to 0-63 and 4-byte to 3-byte conversion
        end = didx+line_len;
        char A, B, C, D;
        if (end > 2) {
          while (didx < end-2)
          {
             A = UUDecMap[(unsigned char) data[sidx]];
             B = UUDecMap[(unsigned char) data[sidx+1]];
             C = UUDecMap[(unsigned char) data[sidx+2]];
             D = UUDecMap[(unsigned char) data[sidx+3]];
             out[didx++] = ( ((A << 2) & 255) | ((B >> 4) & 003) );
             out[didx++] = ( ((B << 4) & 255) | ((C >> 2) & 017) );
             out[didx++] = ( ((C << 6) & 255) | (D & 077) );
             sidx += 4;
          }
        }

        if (didx < end)
        {
            A = UUDecMap[(unsigned char) data[sidx]];
            B = UUDecMap[(unsigned char) data[sidx+1]];
            out[didx++] = ( ((A << 2) & 255) | ((B >> 4) & 003) );
        }

        if (didx < end)
        {
            B = UUDecMap[(unsigned char) data[sidx+1]];
            C = UUDecMap[(unsigned char) data[sidx+2]];
            out[didx++] = ( ((B << 4) & 255) | ((C >> 2) & 017) );
        }

        // skip padding
        while (sidx < len  && data[sidx] != '\n' && data[sidx] != '\r')
            sidx++;

        // skip end of line
        while (sidx < len  && (data[sidx] == '\n' || data[sidx] == '\r'))
            sidx++;

        // skip the "END" separator when present.
        if ( hasLF && strncasecmp( data+sidx, "end", 3) == 0 )
            break;
    }

    if ( didx < out.size()  )
        out.resize( didx );
}

