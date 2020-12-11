#include "camellia.hpp"
#include <iostream>
#include <array>

std::array<std::string, 256> Const = {"70", "82", "2c", "ec", "b3", "27", "c0", "e5", "e4", "85", "57", "35", "ea", "0c", "ae", "41",
                                  "23", "ef", "6b", "93", "45", "19", "a5", "21", "ed", "0e", "4f", "4e", "1d", "65", "92", "bd",
                                  "86", "b8", "af", "8f", "7c", "eb", "1f", "ce", "3e", "30", "dc", "5f", "5e", "c5", "0b", "1a",
                                  "a6", "e1", "39", "ca", "d5", "47", "5d", "3d", "d9", "01", "5a", "d6", "51", "56", "6c", "4d",
                                  "8b", "0d", "9a", "66", "fb", "cc", "b0", "2d", "74", "12", "2b", "20", "f0", "b1", "84", "99",
                                  "df", "4c", "cb", "c2", "34", "7e", "76", "05", "6d", "b7", "a9", "31", "d1", "17", "04", "d7",
                                  "14", "58", "3a", "61", "de", "1b", "11", "1c", "32", "0f", "9c", "16", "53", "18", "f2", "22",
                                  "fe", "44", "cf", "b2", "c3", "b5", "7a", "91", "24", "08", "e8", "a8", "60", "fc", "69", "50",
                                  "aa", "d0", "a0", "7d", "a1", "89", "62", "97", "54", "5b", "1e", "95", "e0", "ff", "64", "d2",
                                  "10", "c4", "00", "48", "a3", "f7", "75", "db", "8a", "03", "e6", "da", "09", "3f", "dd", "94",
                                  "87", "5c", "83", "02", "cd", "4a", "90", "33", "73", "67", "f6", "f3", "9d", "7f", "bf", "e2",
                                  "52", "9b", "d8", "26", "c8", "37", "c6", "3b", "81", "96", "6f", "4b", "13", "be", "63", "2e",
                                  "e9", "79", "a7", "8c", "9f", "6e", "bc", "8e", "29", "f5", "f9", "b6", "2f", "fd", "b4", "59",
                                  "78", "98", "06", "6a", "e7", "46", "71", "ba", "d4", "25", "ab", "42", "88", "a2", "8d", "fa",
                                  "72", "07", "b9", "55", "f8", "ee", "ac", "0a", "36", "49", "2a", "68", "3c", "38", "f1", "a4",
                                  "40", "28", "d3", "7b", "bb", "c9", "43", "c1", "15", "e3", "ad", "f4", "77", "c7", "80", "9e"};

mp Sigma1 = "a09e667f3bcc908b",
   Sigma2 = "b67ae8584caa73b2",
   Sigma3 = "c6ef372fe94f82be",
   Sigma4 = "54ff53a5f1d36f1c",
   Sigma5 = "10e527fade682d1d",
   Sigma6 = "b05688c2b3e6c1fd";

mp S1(mp x) { return mp(Const[x.to_int()].c_str()); }
mp F(mp arg, mp local_key)
{
    arg = arg ^ local_key;
    mp y1, y2, y3, y4, y5, y6, y7, y8,
            x1 = (arg >> 56), x2 = (arg >> 48).MASK(8),
            x3 = (arg >> 40).MASK(8), x4 = (arg >> 32).MASK(8),
            x5 = (arg >> 24).MASK(8), x6 = (arg >> 16).MASK(8),
            x7 = (arg >> 8).MASK(8) , x8 = arg.MASK(8);
    x1 = S1(x1);
    x2 = S1(x2).rotl(1,8);
    x3 = S1(x3).rotl(7,8);
    x4 = S1(x4.rotl(1,8));
    x5 = S1(x5).rotl(1,8);
    x6 = S1(x6).rotl(7,8);
    x7 = S1(x7.rotl(1,8));
    x8 = S1(x8);
    y1 = x1 ^ x3 ^ x4 ^ x6 ^ x7 ^ x8;
    y2 = x1 ^ x2 ^ x4 ^ x5 ^ x7 ^ x8;
    y3 = x1 ^ x2 ^ x3 ^ x5 ^ x6 ^ x8;
    y4 = x2 ^ x3 ^ x4 ^ x5 ^ x6 ^ x7;
    y5 = x1 ^ x2 ^ x6 ^ x7 ^ x8;
    y6 = x2 ^ x3 ^ x5 ^ x7 ^ x8;
    y7 = x3 ^ x4 ^ x5 ^ x6 ^ x8;
    y8 = x1 ^ x4 ^ x5 ^ x6 ^ x7;
    return (y1 << 56) | (y2 << 48) | (y3 << 40) | (y4 << 32) | (y5 << 24) |
                  (y6 << 16) | (y7 << 8) | y8;
}
mp FL(mp arg, mp local_key)
{
    mp r = arg.MASK(32), l = arg >> 32,
            k1 = local_key >> 32, k2 = local_key.MASK(32);
    r = r ^ ((l & k1).rotl(1,32));
    l = l ^ (r | k2);
    return (l << 32) | r;
}
mp FLI(mp arg, mp local_key)
{
    mp r = arg.MASK(32), l = arg >> 32,
            k1 = local_key >> 32, k2 = local_key.MASK(32);
    l = l ^ (r | k2);
    r = r ^ ((l & k1).rotl(1,32));
    return (l << 32) | r;
}


camellia::camellia(mp key, mp argin) : _key(key), _arg(argin)
{
    mp KL, KR;
    if (_key.size() <= 128)
    {
        KL = _key;
        KR = 0;
        this->key128(KL, KR);
    }
    else if (_key.size() <= 256)
    {
        KL = _key >> 128;
        KR = _key.MASK(128);
        this->key256(KL, KR);
    }
    else std::cout <<"ERROR\n\n";
}
camellia::camellia(mp key, mp data, mp inIV) : _key(key), _data(data), _inIV(inIV)
{
    mp KL, KR;
    _arg = _data;
    _datasize= _data.size();
    if (_key.size() <= 128)
    {
        KL = _key;
        KR = 0;
        this->key128(KL, KR);
    }
    else if (_key.size() <= 256)
    {
        KL = _key >> 128;
        KR = _key.MASK(128);
        this->key256(KL, KR);
    }
    else std::cout <<"ERROR\n\n";
}
void camellia::Cipher()
{
    if (_key.size() <= 128) this->Cipher128();
    else this->Cipher256();
}
void camellia::Decipher()
{
    if (_key.size() <= 128) this->Decipher128();
    else this->Decipher256();
}
void camellia::CipherCBC()
{
    mp outdata;
    _arg = _inIV;
    while (_data.size() > 0)
    {
        if (_data.size() >= 128)
        {
            _arg = _arg ^ (_data >> (_data.size()-128));
            _data = _data.MASK(_data.size()-128);
        }
        else
        {
            _arg = _arg ^ _data;
            _data = 0;
        }
        this->Cipher();
        outdata = outdata << 128 | _arg;
    }
    _arg = outdata;
    _data = outdata;
}
void camellia::DecipherCBC()
{
    mp outdata, IV;
    while (_data.size() > 0)
    {
        if (_data.size() % 128 != 0)
        {
            _arg = _data >> (_data.size()-_data.size() % 128);
            _data = _data.MASK(_data.size()-_data.size() % 128);
        }
        else
        {
            _arg = _data >> (_data.size()-128);
            _data = _data.MASK(_data.size()-128);
        }
        IV = _arg;
        this->Decipher();
        if (_datasize < 128) outdata = (outdata << _datasize) | (_arg ^ _inIV);
        else
        {
            _datasize -= 128;
            outdata = (outdata << 128) | (_arg ^ _inIV);
        }
        _inIV = IV;
    }
    _arg = outdata;
    _data = outdata;
}
void camellia::key128 (mp KL, mp KR)
{
    mp KA, D1, D2;
    D1 = (KL ^ KR) >> 64;
    D2 = (KL ^ KR).MASK(64);
    D2 = D2 ^ F(D1, Sigma1);
    D1 = D1 ^ F(D2, Sigma2);
    D1 = D1 ^ (KL >> 64);
    D2 = D2 ^ (KL.MASK(64));
    D2 = D2 ^ F(D1, Sigma3);
    D1 = D1 ^ F(D2, Sigma4);
    KA = (D1 << 64) | D2;

    kw1 = KL >> 64;
    kw2 = KL.MASK(64);
    k1 = KA >> 64;
    k2 = KA.MASK(64);
    k3 = KL.rotl(15,128) >> 64;
    k4 = KL.rotl(15,128).MASK(64);
    k5 = KA.rotl(15,128) >> 64;
    k6 = KA.rotl(15,128).MASK(64);
    ke1 = KA.rotl(30,128) >> 64;
    ke2 = KA.rotl(30,128).MASK(64);
    k7 = KL.rotl(45,128) >> 64;
    k8 = KL.rotl(45,128).MASK(64);
    k9 = KA.rotl(45,128) >> 64;
    k10 = KL.rotl(60,128).MASK(64);
    k11 = KA.rotl(60,128) >> 64;
    k12 = KA.rotl(60,128).MASK(64);
    ke3 = KL.rotl(77,128) >> 64;
    ke4 = KL.rotl(77,128).MASK(64);
    k13 = KL.rotl(94,128) >> 64;
    k14 = KL.rotl(94,128).MASK(64);
    k15 = KA.rotl(94,128) >> 64;
    k16 = KA.rotl(94,128).MASK(64);
    k17 = KL.rotl(111,128) >> 64;
    k18 = KL.rotl(111,128).MASK(64);
    kw3 = KA.rotl(111,128) >> 64;
    kw4 = KA.rotl(111,128).MASK(64);
}
void camellia::key256 (mp KL, mp KR)
{
    mp KA, KB, D1, D2;
    D1 = (KL ^ KR) >> 64;
    D2 = (KL ^ KR).MASK(64);
    D2 = D2 ^ F(D1, Sigma1);
    D1 = D1 ^ F(D2, Sigma2);
    D1 = D1 ^ (KL >> 64);
    D2 = D2 ^ (KL.MASK(64));
    D2 = D2 ^ F(D1, Sigma3);
    D1 = D1 ^ F(D2, Sigma4);
    KA = (D1 << 64) | D2;
    D1 = (KA ^ KR) >> 64;
    D2 = (KA ^ KR).MASK(64);
    D2 = D2 ^ F(D1, Sigma5);
    D1 = D1 ^ F(D2, Sigma6);
    KB = (D1 << 64) | D2;

    kw1 = KL >> 64;
    kw2 = KL.MASK(64);
    k1 = KB >> 64;
    k2 = KB.MASK(64);
    k3 = KR.rotl(15,128) >> 64;
    k4 = KR.rotl(15,128).MASK(64);
    k5 = KA.rotl(15,128) >> 64;
    k6 = KA.rotl(15,128).MASK(64);
    ke1 = KR.rotl(30,128) >> 64;
    ke2 = KR.rotl(30,128).MASK(64);
    k7 = KB.rotl(30,128) >> 64;
    k8 = KB.rotl(30,128).MASK(64);
    k9 = KL.rotl(45,128) >> 64;
    k10 = KL.rotl(45,128).MASK(64);
    k11 = KA.rotl(45,128) >> 64;
    k12 = KA.rotl(45,128).MASK(64);
    ke3 = KL.rotl(60,128) >> 64;
    ke4 = KL.rotl(60,128).MASK(64);
    k13 = KR.rotl(60,128) >> 64;
    k14 = KR.rotl(60,128).MASK(64);
    k15 = KB.rotl(60,128) >> 64;
    k16 = KB.rotl(60,128).MASK(64);
    k17 = KL.rotl(77,128) >> 64;
    k18 = KL.rotl(77,128).MASK(64);
    ke5 = KA.rotl(77,128) >> 64;
    ke6 = KA.rotl(77,128).MASK(64);
    k19 = KR.rotl(94,128) >> 64;
    k20 = KR.rotl(94,128).MASK(64);
    k21 = KA.rotl(94,128) >> 64;
    k22 = KA.rotl(94,128).MASK(64);
    k23 = KL.rotl(111,128) >> 64;
    k24 = KL.rotl(111,128).MASK(64);
    kw3 = KB.rotl(111,128) >> 64;
    kw4 = KB.rotl(111,128).MASK(64);
}
void camellia::Cipher128()
{
    mp D1 = _arg >> 64, D2 = _arg.MASK(64);
    D1 = D1 ^ kw1;
    D2 = D2 ^ kw2;
    D2 = D2 ^ F(D1, k1);
    D1 = D1 ^ F(D2, k2);
    D2 = D2 ^ F(D1, k3);
    D1 = D1 ^ F(D2, k4);
    D2 = D2 ^ F(D1, k5);
    D1 = D1 ^ F(D2, k6);
    D1 = FL(D1, ke1);
    D2 = FLI(D2, ke2);
    D2 = D2 ^ F(D1, k7);
    D1 = D1 ^ F(D2, k8);
    D2 = D2 ^ F(D1, k9);
    D1 = D1 ^ F(D2, k10);
    D2 = D2 ^ F(D1, k11);
    D1 = D1 ^ F(D2, k12);
    D1 = FL(D1, ke3);
    D2 = FLI(D2, ke4);
    D2 = D2 ^ F(D1, k13);
    D1 = D1 ^ F(D2, k14);
    D2 = D2 ^ F(D1, k15);
    D1 = D1 ^ F(D2, k16);
    D2 = D2 ^ F(D1, k17);
    D1 = D1 ^ F(D2, k18);
    D2 = D2 ^ kw3;
    D1 = D1 ^ kw4;
    _arg = (D2 << 64) | D1;
}
void camellia::Cipher256()
{
    mp D1 = _arg >> 64, D2 = _arg.MASK(64);
    D1 = D1 ^ kw1;
    D2 = D2 ^ kw2;
    D2 = D2 ^ F(D1, k1);
    D1 = D1 ^ F(D2, k2);
    D2 = D2 ^ F(D1, k3);
    D1 = D1 ^ F(D2, k4);
    D2 = D2 ^ F(D1, k5);
    D1 = D1 ^ F(D2, k6);
    D1 = FL(D1, ke1);
    D2 = FLI(D2, ke2);
    D2 = D2 ^ F(D1, k7);
    D1 = D1 ^ F(D2, k8);
    D2 = D2 ^ F(D1, k9);
    D1 = D1 ^ F(D2, k10);
    D2 = D2 ^ F(D1, k11);
    D1 = D1 ^ F(D2, k12);
    D1 = FL(D1, ke3);
    D2 = FLI(D2, ke4);
    D2 = D2 ^ F(D1, k13);
    D1 = D1 ^ F(D2, k14);
    D2 = D2 ^ F(D1, k15);
    D1 = D1 ^ F(D2, k16);
    D2 = D2 ^ F(D1, k17);
    D1 = D1 ^ F(D2, k18);
    D1 = FL(D1, ke5);
    D2 = FLI(D2, ke6);
    D2 = D2 ^ F(D1, k19);
    D1 = D1 ^ F(D2, k20);
    D2 = D2 ^ F(D1, k21);
    D1 = D1 ^ F(D2, k22);
    D2 = D2 ^ F(D1, k23);
    D1 = D1 ^ F(D2, k24);
    D2 = D2 ^ kw3;
    D1 = D1 ^ kw4;
    _arg = D2 << 64 | D1;
}
void camellia::Decipher128()
{
    mp D1 = _arg >> 64, D2 = _arg.MASK(64);
    D1 = D1 ^ kw3;
    D2 = D2 ^ kw4;
    D2 = D2 ^ F(D1, k18);
    D1 = D1 ^ F(D2, k17);
    D2 = D2 ^ F(D1, k16);
    D1 = D1 ^ F(D2, k15);
    D2 = D2 ^ F(D1, k14);
    D1 = D1 ^ F(D2, k13);
    D1 = FL(D1, ke4);
    D2 = FLI(D2, ke3);
    D2 = D2 ^ F(D1, k12);
    D1 = D1 ^ F(D2, k11);
    D2 = D2 ^ F(D1, k10);
    D1 = D1 ^ F(D2, k9);
    D2 = D2 ^ F(D1, k8);
    D1 = D1 ^ F(D2, k7);
    D1 = FL(D1, ke2);
    D2 = FLI(D2, ke1);
    D2 = D2 ^ F(D1, k6);
    D1 = D1 ^ F(D2, k5);
    D2 = D2 ^ F(D1, k4);
    D1 = D1 ^ F(D2, k3);
    D2 = D2 ^ F(D1, k2);
    D1 = D1 ^ F(D2, k1);
    D2 = D2 ^ kw1;
    D1 = D1 ^ kw2;
    _arg = D2 << 64 | D1;
}
void camellia::Decipher256()
{
    mp D1 = _arg >> 64, D2 = _arg.MASK(64);
    D1 = D1 ^ kw3;
    D2 = D2 ^ kw4;
    D2 = D2 ^ F(D1, k24);
    D1 = D1 ^ F(D2, k23);
    D2 = D2 ^ F(D1, k22);
    D1 = D1 ^ F(D2, k21);
    D2 = D2 ^ F(D1, k20);
    D1 = D1 ^ F(D2, k19);
    D1 = FL(D1, ke6);
    D2 = FLI(D2, ke5);
    D2 = D2 ^ F(D1, k18);
    D1 = D1 ^ F(D2, k17);
    D2 = D2 ^ F(D1, k16);
    D1 = D1 ^ F(D2, k15);
    D2 = D2 ^ F(D1, k14);
    D1 = D1 ^ F(D2, k13);
    D1 = FL(D1, ke4);
    D2 = FLI(D2, ke3);
    D2 = D2 ^ F(D1, k12);
    D1 = D1 ^ F(D2, k11);
    D2 = D2 ^ F(D1, k10);
    D1 = D1 ^ F(D2, k9);
    D2 = D2 ^ F(D1, k8);
    D1 = D1 ^ F(D2, k7);
    D1 = FL(D1, ke2);
    D2 = FLI(D2, ke1);
    D2 = D2 ^ F(D1, k6);
    D1 = D1 ^ F(D2, k5);
    D2 = D2 ^ F(D1, k4);
    D1 = D1 ^ F(D2, k3);
    D2 = D2 ^ F(D1, k2);
    D1 = D1 ^ F(D2, k1);
    D2 = D2 ^ kw1;
    D1 = D1 ^ kw2;
    _arg = D2 << 64 | D1;
}
