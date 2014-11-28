/** Kernel definitions.
 */

#ifndef _kernel_h_
#define _kernel_h_

#include <stdint.h>
#include <stdlib.h>

#include "hal.h"
#include "target.h"
#include "constructor.h"

#ifndef NULL
#define NULL 0
#endif

#define SYSCALL(name) __elk_set_syscall(SYS_ ## name, sys_ ## name)

#define ELK_ASSERT 1
#if ELK_ASSERT
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif

#define ELK_DEBUG 1
#ifdef ELK_DEBUG

#undef DPRINTF

#define debug __elk_debug
extern int debug;

// Thread debug flags.
#define THRDB_CORE      0x00000001

// Memory debug flags.
#define MEMDB_CORE      0x00000010

// Device debug flags.
#define DEVDB_CORE      0x00000100
#define DEVDB_IRQ       0x00000200
#define DEVDB_DRV       0x00000400

// TTY debug flags.
#define TTYDB_CORE      0x00001000

// Virtual file system debug flags.
#define	VFSDB_CORE	0x00010000
#define	VFSDB_SYSCALL	0x00020000
#define	VFSDB_VNODE	0x00040000
#define	VFSDB_BIO	0x00080000
#define	VFSDB_CAP	0x00100000
#define VFSDB_FLAGS	0x00130000

// Additional file system debug flags.
#define AFSDB_CORE      0x01000000

#if 0
  #include <stdio.h>
  #define DPRINTF(_m,X)	if (debug & (_m)) printf X
#else
  #include "hal.h"
  #define DPRINTF(_m,X)	if (debug & (_m)) diag_printf X
#endif

#else
#define	DPRINTF(_m, X)
#endif  // !ELK_DEBUG

extern int puts(const char *s);
#define panic(arg) do { puts(arg); exit(1); } while(1)

/* RICH: Validate a system call address argument.
 */
enum {
  VALID_RD = 0x01,
  VALID_WR = 0x02,
  VALID_RW = 0x03,
  VALID_EX = 0x04
};

#define VALIDATE_ADDRESS(addr, size, access)

/** Set a system call handler.
 * @param nr The system call number.
 * @param fn The system call handling function.
 * @return 0 on success, -1 on  error.
 * This function is defined in crt1.S.
 */
int __elk_set_syscall(int nr, void *fn);

#endif // _kernel_h_
