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
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

void add_debug_to(char *name);
int __is_debugged(char *name);
int __debug_raw_binary(char *data, int len);

#ifdef DEBUG
#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__ ((unused))
#else
#define VARIABLE_IS_NOT_USED
#endif

#ifdef CACHE_DEBUG_FLAG
/*
  Warning, in this alternative, performance should be improved, but issuing calls
  to debug functions during options parsing might result in debug flag incorrectly
  set to 0 for some files
*/
static int VARIABLE_IS_NOT_USED is_debugged_first = 1;
static int VARIABLE_IS_NOT_USED is_debugged_result = 0;
#define is_debugged(x) ((is_debugged_first && \
                         (is_debugged_first = 0, \
						  is_debugged_result = __is_debugged(x) \
						 ) \
						) || \
                        (is_debugged_result))
#else
#define is_debugged(x) __is_debugged(x)
#endif

#define debug_raw(format, ...) ((void) (is_debugged(__FILE__) && \
                                fprintf(stderr, format, ##__VA_ARGS__)))
#define debug(format, ...) ((void) (is_debugged(__FILE__) && \
                            fprintf(stderr, "[DEBUG] %s,%d: "format, \
                                    __FILE__, __LINE__, ##__VA_ARGS__)))
#define debug_raw_binary(data, len) ((void) (is_debugged(__FILE__) && \
                                     __debug_raw_binary(data, len)))
#else
#define debug_raw(format, ...)
#define debug(format, ...)
#define debug_raw_binary(data, len)
#endif

#ifdef WARNING
#define warning(format, ...) fprintf(stderr, "[WARNING] %s, %d: "format, \
                                     __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#endif
