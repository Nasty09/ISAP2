#ifndef CAMELLIA_HPP
#define CAMELLIA_HPP
#include "obertka.hpp"

class camellia
{
    mp _key, _arg, kw1, kw2, kw3, kw4, ke1, ke2, ke3, ke4, ke5, ke6,
    k1, k2, k3, k4, k5, k6, k7, k8, k9, k10, k11, k12, k13, k14, k15,
    k16, k17, k18, k19, k20, k21, k22, k23, k24;
public:
    camellia(mp key, mp argin);
    friend std::ostream& operator<< (std::ostream& out, const camellia A)
    {
        out << "Key:  " << A._key << "\nArg:  " << A._arg << "\n";
        return out;
    }
    void Cipher();
    void Decipher();
private:
    void key128 (mp KL, mp KR);
    void key256 (mp KL, mp KR);
    void Cipher128();
    void Cipher256();
    void Decipher128();
    void Decipher256();
};
#endif
