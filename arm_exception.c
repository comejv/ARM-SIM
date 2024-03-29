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
#include "debug.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "util.h"

// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0
#define Exception_bit_9 (CP15_reg1_EEbit << 9)
#define IRQ_vector_ADR 0x18

void print_string(arm_core p)
{

    uint32_t address = arm_read_register(p, 0);
    uint8_t value;
    uint32_t i = 0;
    char c[100];
    do
    {
        arm_read_byte(p, address, &value);
        c[i] = (char)value;
        i++;
        address += 1;
    } while (value != 0);
    printf("%s", c);
    fflush(stdout);
}
int scanint(arm_core p)
{
    int x;
    scanf("%d", &x);
    return x;
}

int arm_exception(arm_core p, uint8_t exception)
{
    uint32_t reset_cpsr = 0x153 | Exception_bit_9;
    /* As there is no operating system in our simulator, we handle
     * software interrupts here :
     * - 0x123456 is the end of the simulation
     * - other opcodes can be used for any custom behavior,
     *   such as my_putchar given as an example
     */
    if (exception == SOFTWARE_INTERRUPT)
    {
        uint32_t value;
        uint32_t address = arm_read_register(p, 15);
        address -= 8;
        uint32_t instruction;
        arm_read_word(p, address, &instruction);
        instruction &= 0xFFFFFF;

        switch (instruction)
        {
        case 0x123456:
            return END_SIMULATION;
        case 0x000001:
            value = arm_read_register(p, 0);
            putchar((char)value);
            fflush(stdout);
            return 0;
        case 0x000002:
            value = arm_read_register(p, 0);
            printf("%d", value);
            fflush(stdout);
            return 0;
        case 0x000003:
            print_string(p);
            return 0;
        case 0x000004:
            arm_write_register(p, 0, scanint(p));
            return 0;
        default:
            printf("Unknown software interrupt %x\n", instruction);
            return exception;
        }
    }
    if (exception == INTERRUPT)
    {
        uint32_t cpsr = arm_read_cpsr(p);
        if (get_bit(cpsr, 7) == 0) // TO DO
        {
            debug("DEBUT IRQ !\n");
            uint32_t sp_adr = arm_read_register(p, SP);
            uint32_t R14_irq = arm_read_register(p, PC);
            uint32_t SPSR_irq = cpsr;
            cpsr = set_bits(cpsr, 4, 0, IRQ);
            cpsr = clr_bit(cpsr, 5);
            cpsr = set_bit(cpsr, 7); // Disable IRQ
            arm_write_cpsr(p, cpsr);
            arm_write_spsr(p, SPSR_irq);
            arm_write_register(p, PC, IRQ_vector_ADR - 4);
            arm_write_register(p, LR, R14_irq);
            arm_write_register(p, SP, sp_adr);
            debug("LR value : %x\n", R14_irq);
            return INTERRUPT;
        }
        return 1;
    }
    /* Aside from SWI, we only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */
    if (exception == RESET)
    {
        arm_write_cpsr(p, reset_cpsr);
        arm_write_register(p, 15, 0);
    }
    return exception;
}
