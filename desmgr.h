#ifndef DESMGR_H
#define DESMGR_H

#include "yxyDES2.h"
#include <QString>

class DesMgr
{
public:
    DesMgr();

    QString decrypt( QString strCode );

private:
    void ConvertCiphertext2OtherFormat(int iBitsLen, char *szCipherInBytes);
    int ConvertOtherFormat2Ciphertext(char *szCipher);

    char bitsCiphertextAnyLength[32768];
    char hexCiphertextAnyLength[16384];
    char szSourceKey1[8], szSourceKey2[8], szPlaintextData[8192], szCiphertextData[8192];

    yxyDES2* myDES;
};

#endif // DESMGR_H
