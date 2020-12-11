#include <iostream>
#include <tommath.h>
#include "obertka.hpp"
#include "camellia.hpp"

int main()
{
    mp q256 = "0123456789abcdeffedcba987654321000112233445566778899aabbccddeeff",
            q128 = "0123456789abcdeffedcba9876543210",
            qCBC = "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF",
            x = "0123456789abcdeffedcba9876543210",
            xCBC = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E",
            y256 = "9acc237dff16d76c20ef7c919e3a7509",
            y128 = "67673138549669730857065648eabe43",
            IV1 = "5900000005040302A0A1A2A3A4A50019",
            IV2 = "D4DBCD92A89641561D0DBBD0D57F7E1D";
    camellia a128(q128,x), a256(q256,x), aCBC(qCBC, xCBC, IV1), aCBC2(q256, xCBC, IV2);

    std::cout <<"128:\n"<<a128<<"Out:  "<<y128<<"\n\n";
    a128.Cipher();
    std::cout <<"Cipher:\n"<<a128<<"\n";
    a128.Decipher();
    std::cout <<"Decipher:\n"<<a128<<"\n\n\n";

    std::cout <<"256:\n"<<a256<<"Out:  "<<y256<<"\n\n";
    a256.Cipher();
    std::cout <<"Cipher:\n"<<a256<<"\n";
    a256.Decipher();
    std::cout <<"Decipher:\n"<<a256<<"\n\n\n";

    std::cout <<"CBC 128:\n"<<aCBC<<"IV:   "<<IV1<<"\n\n";
    aCBC.CipherCBC();
    std::cout <<"Cipher:\n"<<aCBC<<"\n";
    aCBC.DecipherCBC();
    std::cout <<"Decipher:\n"<<aCBC<<"\n\n\n";

    std::cout <<"CBC 256:\n"<<aCBC2<<"IV:   "<<IV2<<"\n\n";
    aCBC2.CipherCBC();
    std::cout <<"Cipher:\n"<<aCBC2<<"\n";
    aCBC2.DecipherCBC();
    std::cout <<"Decipher:\n"<<aCBC2<<"\n\n\n";

    return 0;
}
