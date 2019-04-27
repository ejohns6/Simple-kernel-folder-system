#ifndef __MBOX
#define __MBOX

#include <linux/unistd.h>
#include <linux/linkage.h>


_syscall2(long, create_mbox_421, unsigned long, arg1, int, enable_crypt);

_syscall1(long, remove_mbox_421, unsigned long, id);

_syscall0(long, count_mbox_421, void);

_syscall2(long, list_mbox_421, unsigned __user *, mbxes, long, k);

_syscall4(long, send_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key);

_syscall4(long, recv_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key);

_syscall4(long, peek_msg_421, unsigned long, id, unsigned char __user *, msg, long, n, unsigned long, key);

_syscall1(long, count_msg_421, unsigned long, id);

_syscall1(long, len_msg_421, unsigned long, id);

#endif
