/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#ifndef __UTIL_H__
#define __UTIL_H__
#include <stdint.h>

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))

#define get_bit(x, i) (((x)>>(i))&1)
#define set_bit(x, i) ((x)|(1<<(i)))
#define clr_bit(x, i) ((x)&~(1<<(i)))

#define get_bits(x, h, l) (((x)>>(l))&~((((uint32_t) ~0)>>((h)-(l)+1))<<((h)-(l)+1)))
#define set_bits(x, h, l, bits) \
	(((x)&~(((~0)>>(l))<<(l)))|((x)&((((uint32_t) ~0)>>((h)+1))<<((h)+1)))|((bits)<<(l)))

#define reverse_2(x) ((((x)&0xFF)<<8)|(((x)>>8)&0xFF))
#define reverse_4(x) ((((x)&0xFF)<<24)|((((x)>>8)&0xFF)<<16)|\
                      ((((x)>>16)&0xFF)<<8)|(((x)>>24)&0xFF))

uint32_t asr(uint32_t value, uint8_t shift);
uint32_t ror(uint32_t value, uint8_t rotation);

int is_big_endian();

// A function that switch the endian of a value 
void switch_endian(uint32_t *x);

#endif

