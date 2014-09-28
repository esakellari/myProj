#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <errno.h>   
#include <inttypes.h>
#include <string.h>
#include <pthread.h>

#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }
#define		LEN	256
#define     BUFSIZE   512


char 	*song_search_halding(char * , char *);
char 	*song_streaming_halding(char * , char *);
char 	*get_host(char * , char *);
char 	*eidikos_xeirismos(char * , char *);
char 	*strip_from_nl(char * , char *);
char 	*enter_lat_nl(char * , char *);
char    *strip_from_last_keno(char * , char *);
void    diafores(char * , char *);
char    *malakia(char * , char *);
int 	exei_teleia(char *);
char    *get_folder_path (char *,char * );
char    *add_nes (char *,char * ,int );

char music_folder[256];
