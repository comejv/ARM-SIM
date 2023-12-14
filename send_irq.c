/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "csapp.h"
#include "arm_constants.h"

int main(int argc, char *argv[]) {
    struct hostent *host;
    struct sockaddr_in addr;
    int sock, option_value = 1;
    unsigned char irq;
    char *irq_name;

    if (argc < 3) {
        fprintf(stderr, "Usage :"
                "%s host port [ IRQ name ]\n\n"
                "Sends an IRQ signal to the ARM simulator executing on the given host using the given port.\n"
                "The IRQ name is one of :\n", argv[0]);
        for (irq = RESET; (irq_name = arm_get_exception_name(irq)); irq++) {
            fprintf(stderr, "%s", irq_name);
            if (irq == INTERRUPT)
                fprintf(stderr, "\t\t(default)");
            fprintf(stderr, "\n");
        }
        exit(1);
    }
    if (argc < 4)
        irq = INTERRUPT;
    else {
        irq = RESET;
        irq_name = arm_get_exception_name(irq);
        while (irq_name && (strcmp(irq_name, argv[3]) != 0)) {
            irq++;
            irq_name = arm_get_exception_name(irq);
        }
        if (!irq_name) {
            fprintf(stderr, "Unknown irq : %s\n", argv[3]);
            exit(2);
        }
    }
    sock = Socket(PF_INET, SOCK_STREAM, 0);
    Setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    bzero(&addr, sizeof(addr));
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_family = AF_INET;
    host = Gethostbyname(argv[1]);
    memcpy(&addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
    Connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    Write(sock, &irq, 1);
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return 0;
}
