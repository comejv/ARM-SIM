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
%{
#include <stdio.h>
#include <pthread.h>
#include "scanner.h"
#include "gdb_protocol.h"
#include "debug.h"

#define YY_NO_INPUT

#define MAX_ERROR_SIZE 1024

typedef struct parser_data {
    gdb_protocol_data_t gdb;
    char buffer[MAX_ERROR_SIZE];
    int len;
} *parser_data_t;

static void handle_error(parser_data_t data) {
    if (data->len) {
        data->buffer[data->len] = '\0';
        debug("gdb protocol error, invalid data : %s, "
              "requiring retransmission\n", data->buffer);
        gdb_require_retransmission(data->gdb);
        data->len = 0;
    }
}

static void add_character_to_error(parser_data_t data, char c) {
    if (data->len > MAX_ERROR_SIZE-2)
        handle_error(data);
    data->buffer[data->len++] = c;
}
%}
%option interactive always-interactive read noyywrap nounput
%option reentrant extra-type="parser_data_t"

HEX	[0-9a-fA-F]
%%
"+"			handle_error(yyextra);debug("Received ack\n");
"-"			{
                        handle_error(yyextra);
                        debug("Received request for retransmission\n");
                        gdb_transmit_packet(yyextra->gdb);
                        }
"$"[^$#]*"#"{HEX}{HEX}	{
                        handle_error(yyextra);
                        gdb_packet_analysis(yyextra->gdb, yytext, yyleng);
                        }
.			add_character_to_error(yyextra, *yytext);
<<EOF>>			handle_error(yyextra);return 0;
%%
void gdb_scanner(arm_core arm, registers reg, memory mem, int in, int out,
                 pthread_mutex_t *lock) {
    gdb_protocol_data_t gdb;
    struct parser_data data;
    yyscan_t scanner;
    FILE *f;

    gdb = gdb_init_data(arm, reg, mem, out, lock);
    data.gdb = gdb;
    data.len = 0;

    f = fdopen(in, "r");
    if (f == NULL) {
        perror("Cannot open input stream from gdb");
        return;
    }
    yylex_init_extra(&data, &scanner);
    yyset_in(f, scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    gdb_release_data(gdb);
}
