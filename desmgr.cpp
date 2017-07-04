#include "desmgr.h"

DesMgr::DesMgr()
{
    myDES = new yxyDES2;
}

QString DesMgr::decrypt( QString strCode )
{
    QByteArray ba = strCode.toLatin1();
    char *cstr = ba.data();

    memset(szSourceKey1, 0, 8);
    char szSourceKey1[8] = { '5', '5', '5', '5', '5', '7', '8', '7' };
    myDES->InitializeKey(szSourceKey1, 0);

    //char code[8192] = "8F57B45937C2E4AB";
    myDES->DecryptAnyLength(szCiphertextData, ConvertOtherFormat2Ciphertext(cstr), 0);

    char* resule = myDES->GetPlaintextAnyLength();

    QString strRtn(resule);

    return strRtn;
}

void DesMgr::ConvertCiphertext2OtherFormat(int iBitsLen, char *szCipherInBytes)
{
    memset(hexCiphertextAnyLength, 0, 16384);
    memset(bitsCiphertextAnyLength, 0, 32768);
    myDES->Bytes2Bits(szCipherInBytes, bitsCiphertextAnyLength, iBitsLen);
    myDES->Bits2Hex(hexCiphertextAnyLength, bitsCiphertextAnyLength, iBitsLen);
    for (int i = 0; i < iBitsLen; i++)
    {
        bitsCiphertextAnyLength[i] += 48;
    }
}

int DesMgr::ConvertOtherFormat2Ciphertext(char *szCipher)
{
    int iLen = 0;
    memset(szCiphertextData, 0, 8192);

    {
        iLen = ((strlen(szCipher) >> 2) + (strlen(szCipher) % 4 == 0 ? 0 : 1)) << 4;
        memset(hexCiphertextAnyLength, 0, 16384);
        memcpy(hexCiphertextAnyLength, szCipher, strlen(szCipher));
        myDES->Hex2Bits(hexCiphertextAnyLength, bitsCiphertextAnyLength, iLen);
        myDES->Bits2Bytes(szCiphertextData, bitsCiphertextAnyLength, iLen);
    }

    return iLen >> 3;
}
