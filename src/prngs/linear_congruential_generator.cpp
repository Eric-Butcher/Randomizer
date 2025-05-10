#include "prng.hpp"

class MersenneTwister : PseudoRandomNumberGenerator {

    /* MT19937-64 coefficients:
    w = 64
    n = 312
    m = 156
    r = 31
    a = 0xb5026f5aa96619e9
    u = 29
    d = 0x5555555555555555
    s = 17
    b = 0x71d67fffeda60000
    t = 37
    c = 0xfff7eee000000000
    l = 43
    f = 6364136223846793005
    */
    MersenneTwister(){

    }
};