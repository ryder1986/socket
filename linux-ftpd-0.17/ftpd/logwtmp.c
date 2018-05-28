/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/*
 * From: @(#)logwtmp.c	8.1 (Berkeley) 6/4/93
 * From: NetBSD: logwtmp.c,v 1.4 1995/04/11 02:44:58 cgd Exp
 * From: OpenBSD: ? 
 */
char logwtmp_rcsid[] = 
  "$Id: logwtmp.c,v 1.5 1999/07/16 00:34:29 dholland Exp $";

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include "extern.h"

static int fd = -1;

/*
 * Modified version of logwtmp that holds wtmp file open
 * after first call, for use with ftp (which may chroot
 * after login, but before logout).
 */
void
ftpdlogwtmp(const char *line, const char *name,
	const char *host, const struct sockaddr_storage *ss)
{
	struct utmp ut;
	struct stat buf;

	if (fd < 0 && (fd = open(_PATH_WTMP, O_WRONLY|O_APPEND, 0)) < 0)
		return;
	if (fstat(fd, &buf) == 0) {
		memset(&ut, 0, sizeof(ut));
#ifdef USER_PROCESS
		ut.ut_type = *name ? USER_PROCESS : DEAD_PROCESS;
		ut.ut_pid = getpid();
#endif

#if __WORDSIZE == 64 && __WORDSIZE_COMPAT32
		/* Length of 'time_t' is 64 bits, but 'ut.ut_time' has 32 bits.*/
		time_t now;

		(void)time(&now);
		ut.ut_time = now & 0xffffffff; /* Discard the upper 32 bits. */
#else
		/* Equal size time representations. */
		(void)time(&ut.ut_time);
#endif /* Mixed 64 and 32 bits time */

		(void)strncpy(ut.ut_line, line, sizeof(ut.ut_line));
		(void)strncpy(ut.ut_name, name, sizeof(ut.ut_name));
		(void)strncpy(ut.ut_host, host, sizeof(ut.ut_host));
		if (ss) {
		    switch (ss->ss_family) {
			case AF_INET6:
			    memcpy(&ut.ut_addr,
				    &((const struct sockaddr_in6 *) ss)->sin6_addr,
				    sizeof(struct in6_addr));
			    break;
			case AF_INET:
			    memcpy(&ut.ut_addr,
				    &((const struct sockaddr_in *) ss)->sin_addr,
				    sizeof(struct in_addr));
			default:
			    break;
		    }
		}
		if (write(fd, (char *)&ut, sizeof(struct utmp)) !=
		    sizeof(struct utmp))
			(void)ftruncate(fd, buf.st_size);
	}
}
