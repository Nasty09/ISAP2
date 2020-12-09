#include <iostream>
#include <tommath.h>
#include "obertka.hpp"
#include "camellia.hpp"

int main()
{
    mp q256 = "0123456789abcdeffedcba987654321000112233445566778899aabbccddeeff",
            q128 = "0123456789abcdeffedcba9876543210",
            qCBC = "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF",
            qCTR = "776BEFF2851DB06F4C8A0542C8696F6C6A81AF1EEC96B4D37FC1D689E6C1C104",
            x = "0123456789abcdeffedcba9876543210",
            xCBC = "5900000005040302A0A1A2A3A4A50019",
            xCTR = "53696E676C6520626C6F636B206D7367",
            y256 = "9acc237dff16d76c20ef7c919e3a7509",
            y128 = "67673138549669730857065648eabe43",
            yCBC = "6F6915DFA6A0DF2484A73788A365F92E",
            yCTR = "3401F9C8247EFFCEBD6994714C1BBB11";
    camellia a128(q128,x), a256(q256,x), aCBC(qCBC, xCBC), aCTR(qCTR,xCTR);

    std::cout <<"128:\n"<<a128<<"\n";
    a128.Cipher();
    std::cout <<"Cipher:\n"<<a128<<"\n";
    a128.Decipher();
    std::cout <<"Decipher:\n"<<a128<<"\n\n\n";

    std::cout <<"256:\n"<<a256<<"\n";
    a256.Cipher();
    std::cout <<"Cipher:\n"<<a256<<"\n";
    a256.Decipher();
    std::cout <<"Decipher:\n"<<a256<<"\n\n\n";

    std::cout <<"CBC:\n"<<aCBC<<"\n";
    aCBC.Cipher();
    std::cout <<"Cipher:\n"<<aCBC<<"\n";
    aCBC.Decipher();
    std::cout <<"Decipher:\n"<<aCBC<<"\n\n\n";

    std::cout <<"CTR:\n"<<aCTR<<"\n";
    aCTR.Cipher();
    std::cout <<"Cipher:\n"<<aCTR<<"\n";
    aCTR.Decipher();
    std::cout <<"Decipher:\n"<<aCTR<<"\n\n\n";

    std::cout<<yCTR<<"\n";

    return 0;
}
