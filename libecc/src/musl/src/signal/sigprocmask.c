#include <signal.h>
#include <errno.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *old)
{
	int r = pthread_sigmask(how, set, old);
	if (!r) return r;
	errno = r;
	return -1;
}
