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
#include <sys/socket.h>
#include <pthread.h>
#include <getopt.h>
#include "csapp.h"
#include "scanner.h"
#include "arm.h"
#include "memory.h"
#include "gdb_protocol.h"
#include "trace.h"
#include "debug.h"

struct shared_data {
    registers reg;
    memory mem;
    arm_core arm;
    pthread_mutex_t lock;
    in_port_t gdb_port, irq_port;
};

struct server_data {
    int socket;
    unsigned short port;
};

static struct server_data create_server(in_port_t port) {
    struct sockaddr_in addr;
    socklen_t addr_length;
    struct server_data result;
    int option_value = 1;

    result.socket = Socket(PF_INET, SOCK_STREAM, 0);
    Setsockopt(result.socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    bzero(&addr, sizeof(addr));
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(result.socket, (struct sockaddr *) &addr, sizeof(addr));
    addr_length = sizeof(addr);
    getsockname(result.socket, (struct sockaddr *) &addr, &addr_length);
    result.port = ntohs(addr.sin_port);

    Listen(result.socket, 1);
    return result;
}

static void *gdb_listener(void *arg) {
    struct shared_data *shared = (struct shared_data *) arg;
    struct sockaddr_in peer;
    int connection;
    socklen_t peer_length;
    struct server_data server;

    server = create_server(shared->gdb_port);
    fprintf(stderr, "Listening to gdb connection on port %d\n", server.port);
    peer_length = sizeof(peer);
    connection = Accept(server.socket, (struct sockaddr *) &peer, &peer_length);
    gdb_scanner(shared->arm, shared->reg, shared->mem, connection, connection, &shared->lock);
    shutdown(connection, SHUT_RDWR);
    close(server.socket);

    pthread_exit(NULL);
}

static void *irq_listener(void *arg) {
    struct shared_data *shared = (struct shared_data *) arg;
    struct sockaddr_in peer;
    int connection;
    socklen_t peer_length;
    struct server_data server;
    unsigned char irq;

    server = create_server(shared->irq_port);
    fprintf(stderr, "Listening to irq connections on port %d\n", server.port);
    while (1) {
        peer_length = sizeof(peer);
        connection = Accept(server.socket, (struct sockaddr *) &peer, &peer_length);
        while (Read(connection, &irq, 1) > 0) {
            pthread_mutex_lock(&shared->lock);
            arm_exception(shared->arm, irq);
            pthread_mutex_unlock(&shared->lock);
        }
        shutdown(connection, SHUT_RDWR);
    }
    close(server.socket);

    pthread_exit(NULL);
}

void usage(char *name) {
    fprintf(stderr, "Usage:\n"
            "%s [ --help ] [ --gdb-port port ] [ --irq-port port ] "
            "[ --trace-file file ] [ --trace-registers ] [ --trace-memory ] "
            "[ --trace-state ] [ --trace-position ] [ --debug filename ]\n\n"
            "Start an ARMv5 instruction set simulator that acts as a gdb server "
            "and can receive interrupts. It is possible to specify on which ports "
            "the simulator listen to gdb client or irq sending program "
            "connections. Trace options have the following behavior:\n"
            "- trace file: file into which trace information is stored (default is"
            " stdout)\n"
            "- trace registers: outputs informations about each access to"
            " registers\n"
            "- trace memory: outputs informations about each access to memory\n"
            "- trace state: outputs the processor state after each instruction\n"
            "- trace position: for each traced access, outputs the file and line"
            " at which the access has been performed\n"
            "The debug switch enable selective reporting of debug messages on a "
            "per source file basis\n", name);
}

int main(int argc, char *argv[]) {
    struct shared_data shared;
    pthread_t gdb_thread;
    pthread_t irq_thread;
    void *result;
    int opt;
    FILE *trace_file;

    struct option longopts[] = {
        { "gdb-port", required_argument, NULL, 'g' },
        { "irq-port", required_argument, NULL, 'i' },
        { "trace-file", required_argument, NULL, 't' },
        { "trace-registers", no_argument, NULL, 'r' },
        { "trace-memory", no_argument, NULL, 'm' },
        { "trace-state", required_argument, NULL, 's' },
        { "trace-position", no_argument, NULL, 'p' },
        { "help", no_argument, NULL, 'h' },
        { "debug", required_argument, NULL, 'd' },
        { NULL, 0, NULL, 0 }
    };

    shared.gdb_port = 0;
    shared.irq_port = 0;
    trace_file = stdout;
    while ((opt = getopt_long(argc, argv, "g:i:ht:rmspd:", longopts, NULL))
           != -1) {
        switch (opt) {
        case 'g':
            shared.gdb_port = atoi(optarg);
            break;
        case 'i':
            shared.irq_port = atoi(optarg);
            break;
        case 'h':
            usage(argv[0]);
            exit(0);
        case 't':
            trace_file = fopen(optarg, "w");
            if (trace_file == NULL) {
                perror("Trace file");
                exit(1);
            }
            break;
        case 'r':
            trace_add(REGISTERS);
            break;
        case 'm':
            trace_add(MEMORY);
            break;
        case 's':
            trace_add(arm_get_mode_number(optarg));
            break;
        case 'p':
            trace_add(POSITION);
            break;
        case 'd':
            add_debug_to(optarg);
            break;
        default:
            fprintf(stderr, "Unrecognized option %c\n", opt);
            usage(argv[0]);
            exit(1);
        }
    }
    gdb_init();
    arm_init();
    set_trace_file(trace_file);

    // We allocate 32K for the memory, and we have to make sure that this is coherent with
    // the examples, see in particular the linker_script.
    shared.mem = memory_create(0x8000);
    shared.reg = registers_create();
    shared.arm = arm_create(shared.reg, shared.mem);

    pthread_mutex_init(&shared.lock, NULL);
    pthread_create(&gdb_thread, NULL, gdb_listener, &shared);
    pthread_create(&irq_thread, NULL, irq_listener, &shared);
    pthread_join(gdb_thread, &result);
    arm_destroy(shared.arm);
    registers_destroy(shared.reg);
    memory_destroy(shared.mem);
    return 0;
}
