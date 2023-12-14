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
#ifdef arm_fetch
#undef arm_fetch
#endif
#ifdef arm_read_register
#undef arm_read_register
#endif
#ifdef arm_read_usr_register
#undef arm_read_usr_register
#endif
#ifdef arm_read_cpsr
#undef arm_read_cpsr
#endif
#ifdef arm_read_spsr
#undef arm_read_spsr
#endif
#ifdef arm_write_register
#undef arm_write_register
#endif
#ifdef arm_write_usr_register
#undef arm_write_usr_register
#endif
#ifdef arm_write_cpsr
#undef arm_write_cpsr
#endif
#ifdef arm_write_spsr
#undef arm_write_spsr
#endif
#ifdef arm_read_byte
#undef arm_read_byte
#endif
#ifdef arm_read_half
#undef arm_read_half
#endif
#ifdef arm_read_word
#undef arm_read_word
#endif
#ifdef arm_write_byte
#undef arm_write_byte
#endif
#ifdef arm_write_half
#undef arm_write_half
#endif
#ifdef arm_write_word
#undef arm_write_word
#endif
