#include "webserv.hpp"

//operateur binaire de mon cul
uint16_t ft_htons(uint16_t tmp) { 
    return ((__uint16_t) ((((tmp) >> 8) & 0xff) | (((tmp) & 0xff) << 8))); 
}