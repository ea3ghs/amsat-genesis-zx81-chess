
#make clean chess  run
#make clean simple run

all: clean chess run

run: a.out
	clear
	rm -f run.txt
	./a.out	

a.out: zx81.c rom.h tape.h simz80.c
	gcc zx81.c simz80.c
	readelf -s a.out > size.txt
	fgrep z80_step size.txt

arm:
	clang -mcpu=cortex-m0 -D ARM -Os zx81.c simz80.c
	readelf -s a.out > arm.txt
	arm-none-eabi-strip a.out
	
rom.h: rom.asm
	wine sjasmplus rom.asm --raw=rom --lst=rom.lst
	xxd -i rom rom.h

tape.h: tape.p
	cp tape.p tape
	xxd -i tape tape.h
	rm tape

chess: chess.asm
	wine sjasmplus chess.asm --raw=tape.p --lst=tape.lst

simple: simple.asm
	wine sjasmplus simple.asm --raw=tape.p --lst=tape.lst

clean:
	rm -f a.out *.o *.gch *~ in.txt size.txt rom rom.h *.lst tape.p tape.h tape x run.txt
	ls

#	z80asm -ltape.lst -otape.p simple.asm
#arm-none-eabi-gcc -march=native -Q --help=target
#arm-none-eabi-gcc  -mcpu=native -Q --help=target 

