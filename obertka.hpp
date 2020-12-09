#ifndef OBERT_HPP
#define OBERT_HPP
#include <iostream>
#include <tommath.h>

class mp
{
    mp_int a;
public:
    mp(const char *_a)
    {
        mp_err err;
        err = mp_init(&a);
        err = mp_read_radix(&a, _a, 16);
    }
    mp(int _a)
    {
        mp_err err;
        err = mp_init(&a);
        mp_set_l(&a, _a);
    }
//    mp(mp_int _a)
//    {
//        mp_err err;
//        err = mp_init_copy(&a,&_a);
//    }
    mp()
    {
        mp_err err;
        err = mp_init(&a);
    }
    mp(const mp& other)
    {
        mp_err err;
        err = mp_init_copy(&a, &other.a);
    }
    ~mp()
    {
        mp_clear(&a);
    }

    mp& operator=(const mp& other)
    {
        mp_clear(&a);
        mp_err err;
        err = mp_init_copy(&a, &other.a);
        return *this;
    }
    bool operator== (mp b)
    {
        if (mp_cmp(&a, &b.a) == 0)
            return 1;
        return 0;
    }
    mp operator>> (int b)
    {
        mp c, d;
        mp_err err;
        err = mp_div_2d(&a, b, &c.a, &d.a);
        return c;
    }
    mp operator<< (int b)
    {
        mp c, d;
        mp_err err;
        err = mp_mul_2d(&a, b, &c.a);
        return c;
    }
    mp operator^ (mp b)
    {
        mp c;
        mp_err err;
        err = mp_xor(&a, &b.a, &c.a);
        return c;
    }
    mp operator| (mp b)
    {
        mp c;
        mp_err err;
        err = mp_or(&a, &b.a, &c.a);
        return c;
    }
    mp operator& (mp b)
    {
        mp c;
        mp_err err;
        err = mp_and(&a, &b.a, &c.a);
        return c;
    }
    mp operator~ ()
    {
        mp b;
        mp_err err;
        err = mp_complement(&a, &b.a);
        return b;
    }
    friend std::ostream& operator<< (std::ostream& out, const mp A)
    {
        mp_err err;
        err = mp_fwrite(&A.a, 10, stdout);
        out << "";
        return out;
    }

    int size()
    {
        return mp_count_bits(&a);
    }
    int to_int() { return mp_get_i32(&a); }
    mp MASK(int b)
    {
        mp c, d;
        mp_err err;
        err = mp_div_2d(&a, b, &c.a, &d.a);
        return d;
    }
    mp rotl(int b, int c)
    {
        if (this->size() < c && c - this->size() >= b)
        {
            return (*this << b);
        }
        if (this->size() < c && c - this->size() < b)
        {
            mp p = *this << (c - this->size());
            b = b - c + this->size();
            return p.MASK(c - b) << b | (p >> (c - b));
        }
        return (this->MASK(c - b) << b) | (*this >> (c - b));
    }
//    mp to_bin(int b)
//    {
//        mp buf = 0, zero(0);
//        while (b > 0)
//        {
//            buf = (buf << 1 | (*this >> b).MASK(1));
//            b--;
//        }

//        return buf;
//    }
//    mp rotr(int b) { return (*this >> b) | (*this << (this->size() - b)); }
};
#endif


//    mp exp(int b)
//    {
//        mp c = 1;
//        int n = 0;
//        while (n < b)
//        {
//            c = *this * c;
//            n++;
//        }
//        return c;
//    }
