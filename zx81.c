
/*
TO_DO

--arreglar keyb
--reducir ram a 1kB y depositar la ROM en const
--pasar un "mensaje", leerlo, interpretar X instrucciones, retornar salida, mientras no ocupo CPU
--??implementar persistencia ram/cpu entre ejecuciones de la aplicacion 
--autoexec? 1kbyte 
--QUITAR AUTOXEC DE CHESS PARA LISTAR CODIGO BASIC
--LIMPIARMAIN
--interceptar lecturas teclado chess y pasar movimientos, calcular tiempo computo (entre dos lecturas teclado)
--interceptar lectura teclado via rom
--arregalar includes y librerias compilado microcontrolador
--quitar struct z80* z80
*/

/*
DONE
--probar varios esquemas compilado z80, armgcc clangarm USAR CLANG
--hexdump .ROM .P a .H
--chess.asm 
--rom.asm

--rutina impresion pantalla, buscar 25 separadores de campo
--aclarar pantalla de ajedrez
--contar bytes en pantalla

--si quito "ghosting de la rom, NO falla la maquina
--si simplifico fetch y write, NO falla la maquina
--minusculas o ^ es shift: modifica unicamente la siguiente pulsacion<
--generar un .P con un minimo programa en ASM
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simz80.h"

const
#include "rom.h"
const
#include "tape.h"

struct z80 z80;
BYTE memory[ 65536 ];
long clock=0;
unsigned char keyb[256];unsigned char shift=0;
char key=0,keyr;

BYTE z80_fetch(WORD a         ){ return memory[ a ]; } 
BYTE z80_read (WORD a         ){ return memory[ a ]; } 
void z80_write(WORD a, BYTE b ){ if(a>=0x4000) { memory[ a ] = b; } } 
void z80_out  (WORD a, BYTE b ){ /*(void)z80; (void)a; (void)b;*/ }

/*Port____Line____Bit__0____1____2____3____4__
  FEFEh  0  (A8)     SHIFT  Z    X    C    V
  FDFEh  1  (A9)       A    S    D    F    G
  FBFEh  2  (A10)      Q    W    E    R    T
  F7FEh  3  (A11)      1    2    3    4    5
  EFFEh  4  (A12)      0    9    8    7    6
  DFFEh  5  (A13)      P    O    I    U    Y
  BFFEh  6  (A14)    ENTER  L    K    J    H
  7FFEh  7  (A15)     SPC   .    M    N    B */

BYTE z80_in( WORD a )
{
const BYTE bb[]={0xfe,0xfd,0xfb,0xf7, 0xef,0xdf,0xbf,0x7f};
int k;
BYTE r=0xff;
BYTE b;
k=keyb[key]; 
if(k)
  {
  b=bb[(k>>5)&7];
  if(a>>8==b)
    {
    r=~(k&0x1f);
    keyr=r;
    }
  }
if(a>>8==0xfe&&shift)r&=0xfe; //mover dentro if(k)
return r;
}



/* setup the emulation state */
static void setup_emulation( void )
{
//  0000-1FFF   BIOS ROM (8KBytes)
//  2000-3FFF   not used
//  4000-43FF   Internal RAM (1 KByte)
//  4000-7FFF   External RAM (16 KBytes)

memset(&z80,0,sizeof(z80));
memcpy(memory+0x0000,rom,0x2000);   
//memcpy(memory+0x2000,rom_bin,0x2000);  /* load rom with ghosting at 0x2000 */
memory[0x02b5+0x0000]=0xc9;   /* patch DISPLAY-5 to a return */
//memory[0x02b5+0x2000]=0xc9;
z80.pc =0;   /* setup the registers */
z80.iff=0;
z80.af_sel=z80.regs_sel = 0;

for(int i=0;i<256;i++)keyb[i]=0;
//keyb[ LSHIFT'] = 0 << 5 |  1;
//keyb['RSHIFT'] = 0 << 5 |  1;
  keyb['Z']      = 0 << 5 |  2;
  keyb['X']      = 0 << 5 |  4;
  keyb['C']      = 0 << 5 |  8;
  keyb['V']      = 0 << 5 | 16;
  keyb['A']      = 1 << 5 |  1;
  keyb['S']      = 1 << 5 |  2;
  keyb['D']      = 1 << 5 |  4;
  keyb['F']      = 1 << 5 |  8;
  keyb['G']      = 1 << 5 | 16;
  keyb['Q']      = 2 << 5 |  1;
  keyb['W']      = 2 << 5 |  2;
  keyb['E']      = 2 << 5 |  4;
  keyb['R']      = 2 << 5 |  8;
  keyb['T']      = 2 << 5 | 16;
  keyb['1']      = 3 << 5 |  1;
  keyb['2']      = 3 << 5 |  2;
  keyb['3']      = 3 << 5 |  4;
  keyb['4']      = 3 << 5 |  8;
  keyb['5']      = 3 << 5 | 16;
  keyb['0']      = 4 << 5 |  1;
  keyb['9']      = 4 << 5 |  2;
  keyb['8']      = 4 << 5 |  4;
  keyb['7']      = 4 << 5 |  8;
  keyb['6']      = 4 << 5 | 16;
  keyb['P']      = 5 << 5 |  1;
  keyb['O']      = 5 << 5 |  2;
  keyb['I']      = 5 << 5 |  4;
  keyb['U']      = 5 << 5 |  8;
  keyb['Y']      = 5 << 5 | 16;
  keyb['L']      = 6 << 5 |  2;
  keyb['K']      = 6 << 5 |  4;
  keyb['J']      = 6 << 5 |  8;
  keyb['H']      = 6 << 5 | 16;
  keyb['M']      = 7 << 5 |  4;
  keyb['N']      = 7 << 5 |  8;
  keyb['B']      = 7 << 5 | 16;
  keyb[' ']      = 7 << 5 |  1;
  keyb['.']      = 7 << 5 |  2;
  keyb['\n']     = 6 << 5 |  1;
}


void carga_p(void)
{
/*
//carga .P
char bas[4096];
FILE *f=0;
f=fopen("chess.p","r");
int n=0;
n=fread(bas,1,4096,f);
fclose(f);
printf("p.p %dB\n",n);
memcpy(&memory[0x4009],bas,n);
//scanf("%d",&n);
*/
memcpy(&memory[0x4009],tape,sizeof(tape));
}




//https://en.wikipedia.org/wiki/ZX81_character_set
////////////0123456789A BCFEF.123456789ABCFEF.123456789ABCFEF.123456789ABCFEF
char lut[]="           \"$$:?()><=+-*/;,.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void update_screen(void)
{
#ifndef ARM
/*
static long l=0;
int x,y,z;
y=memory[D_FILE] | memory[D_FILE+1] << 8;
//if(l==150)system("clear");
//system("tput home");
printf("              \rZX81");
for(int f=0;f!=24;f++)
for(int c=0;c!=33;c++) //separador+32columnas
    {
    int flag=0;
    x=y+f*33+c;
    x=memory[x];
    if(x==0x76){printf("|\n %02d |",f+1);continue;}
    if(x&0x80)flag=1;
    x=x&0x7f;//quitonegrita
    if(x>0x3f)x='@';else x=lut[x];
    if(flag)x=tolower(x); 
    printf("%c",x);
    }
printf("\n");
//printf("\nl=%ld pc=0x%04x shift=%2x key=%02x:%c \n",l++,z80.pc,shift,keyr&0xff,key);
*/
#define D_FILE 0x400c  /* address of the pointer to the beginning of the display file */
int f=0,p,flag,n=0;
char x;
p=memory[D_FILE]|memory[D_FILE+1]<<8;
system("tput home");
printf("              \rZX81");
while(f<25)
 {
 x=memory[p++];n++;
 if(x==0x76){printf("|\n %02d |",++f);continue;}
 if(x&0x80)flag=1;else flag=0;
 x=x&0x7f;//quitonegrita
 if(x>0x3f)x='@';else x=lut[x];
 if(flag)
   {
   //x=tolower(x); 
   printf("\e[7m%c\e[0m",x);
   }
 else
   printf("%c",x); 
 }
//printf("video %d\n",n);
#endif
}


//3 L A $L$ K $B$ $\n$Q $I$ T $O$ $V$ 10\n\
//const char s[]="10EHOLA P$P$\n20EZX81\nV\nK\n*";

//pendulum
const char s1[]="\n\
1 L S ^L 0\n\
2 L T ^L 1\n\
3 L A ^L ^\nQ T\n\
4 L S ^L S ^K A\n\
5 L T ^L T ^K S\n\
6 P T ^. S\n\
7 G 3\n\
R\n\
C\n\
C\n\
C\n\
C\n\
C\n\
C\n\
C\n*";



//load+run list quit
const char s[]="\
J^P^P\n\
K\n\
*";



// CHESS load,run,quit
const char s3[]="J^P^P\n\
               7A6A\n\
               6A5A\n\
               5A4A\n\
               4A3A\n\
               *";


void keyf(void)
{
static int x=0;
if(key){key=0;shift=0;return;} //en la siguiente llamada suelto tecla y shift
key=toupper(s[x]);
if(key=='*')exit(0);
if(key=='^'){shift=1;key=toupper(s[++x]);}
if(x<sizeof(s))x++;
}



#define _AF  ( z80.af[ z80.af_sel ] )
#define _PC  ( z80.pc )
/*
#define BC  ( z80->regs[ z80->regs_sel ].bc )
#define DE  ( z80->regs[ z80->regs_sel ].de )
#define HL  ( z80->regs[ z80->regs_sel ].hl )
#define SP  ( z80->sp )
#define IX  ( z80->ix )
#define IY  ( z80->iy ) 
#define PC  ( z80->pc )
#define IFF ( z80->iff )
#define IR  ( z80->ir )
*/

int main( int argc, char *argv[] )
{
//FILE *f=fopen("run.txt","w");
setup_emulation();  
while(1)
  {
  #define CC 1E7
  for (int c=0;c<CC;c++)
     {
     clock+=CC;
     z80_step();
     if(z80.pc==0x347){carga_p();z80.pc=0x207;}
     //if(_PC>0x4000)fprintf(f,"pc:%04x AF:%04x\n",_PC,_AF);
     }
  update_screen();
  keyf();
  }
return 0;
}


#ifdef ARM
void exit(int x){}
#endif


