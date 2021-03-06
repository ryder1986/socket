/* $OpenBSD: version.h,v 1.76 2016/02/23 09:14:34 djm Exp $ */

#define SSH_VERSION	"OpenSSH_7.2"

#define SSH_PORTABLE	"p2"
#define SSH_RELEASE_MINIMUM	SSH_VERSION SSH_PORTABLE
#ifdef SSH_EXTRAVERSION
#define SSH_RELEASE	SSH_RELEASE_MINIMUM " " SSH_EXTRAVERSION
#else
#define SSH_RELEASE	SSH_RELEASE_MINIMUM
#endif
