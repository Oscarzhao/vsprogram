#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils/daemon.h"

#define ERR_DAEMON_INIT -10601

typedef void Sigfunc(int);

static Sigfunc *tried_signal( const int signo, Sigfunc *pfunc )
{
    struct sigaction act, oact;

    memset( &act, 0, sizeof(struct sigaction) );
    memset( &oact, 0, sizeof(struct sigaction) );
    act.sa_handler = pfunc;
    if ( sigemptyset( &act.sa_mask ) == -1 )
        return SIG_ERR;
    act.sa_flags = 0;
    if(signo == SIGALRM)
    {
#ifdef  SA_INTERRUPT
            act.sa_flags |= SA_INTERRUPT;  //SunOS 4.x
#endif
        }
    {
#ifdef  SA_RESTART
            act.sa_flags |= SA_RESTART;  //SVR4,44BSD
#endif
        }
    if ( sigaction(signo, &act, &oact) < 0 )
        return( SIG_ERR );

    return( oact.sa_handler );
}

int daemon_init()
{
    pid_t pid;

    if ( (pid=fork()) < 0 )
        return ERR_DAEMON_INIT;
    if ( pid > 0 )
        exit( 0 );

    setsid();

    if ( tried_signal(SIGHUP, SIG_IGN) == SIG_ERR )
        return ERR_DAEMON_INIT;
    if ( tried_signal(SIGINT, SIG_IGN) == SIG_ERR )
        return ERR_DAEMON_INIT;
    if ( tried_signal(SIGWINCH, SIG_IGN) == SIG_ERR )
        return ERR_DAEMON_INIT;
    if ( tried_signal(SIGPIPE, SIG_IGN) == SIG_ERR )
        return ERR_DAEMON_INIT;

    if ( (pid=fork()) < 0 )
        return ERR_DAEMON_INIT;
    if ( pid > 0 )
        exit( 0 );
	
    umask( 0 );
	
	int i;
	for(i=0; i<3; i++)
		close(i);

    return 0;
}

