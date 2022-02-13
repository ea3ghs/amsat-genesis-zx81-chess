/* GPL2 1995 Frank D. Cringle. Modifications 2000/2003 by Andreas Gerlich */

#ifndef _SIMZ80_H

#include <limits.h>

#if UCHAR_MAX == 255
typedef unsigned char	BYTE;
#else
#error Need to find an 8-bit type for BYTE
#endif

#if USHRT_MAX == 65535
typedef unsigned short	WORD;
#else
#error Need to find an 16-bit type for WORD
#endif

/* FASTREG needs to be at least 16 bits wide and efficient for the
   host architecture */
#if UINT_MAX >= 65535
typedef unsigned int	FASTREG;
#else
typedef unsigned long	FASTREG;
#endif

/* FASTWORK needs to be wider than 16 bits and efficient for the host
   architecture */
#if UINT_MAX > 65535
typedef unsigned int	FASTWORK;
#else
typedef unsigned long	FASTWORK;
#endif

struct ddregs
{
	WORD bc;
	WORD de;
	WORD hl;
};

struct z80
{
  WORD af[ 2 ];
  int  af_sel;

  struct ddregs regs[ 2 ];
  int    regs_sel;

  WORD ir;
  WORD ix;
  WORD iy;
  WORD sp;
  WORD pc;
  WORD iff;
};

extern void z80_step(  );
extern BYTE z80_fetch( WORD a );
extern BYTE z80_read(  WORD a );
extern void z80_write( WORD a, BYTE b );
extern BYTE z80_in(    WORD a );
extern void z80_out(   WORD a, BYTE b );

#endif /* _SIMZ80_H */
