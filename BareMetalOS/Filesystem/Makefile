# Made by Jacob Jagiella with Pavani Ravella and Liao Zeyu
all: floppya.img

kernel: kernel_asm.o kernel.o
	ld86 -o kernel -d kernel.o kernel_asm.o

kernel_asm.o: kernel.asm
	as86 kernel.asm -o kernel_asm.o

kernel.o: kernel.c
	bcc -ansi -c -o kernel.o kernel.c

bootload: bootload.asm
	nasm bootload.asm
bootload.o: bootload.asm	
	as86 bootload.asm -o bootload.o

lib_asm.o: lib.asm
	as86 lib.asm -o lib_asm.o

floppya.img: bootload kernel
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3
	dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
	./loadFile message.txt

cleanimage:
	rm -f floppya.img
	rm -f bootload kernel *.o *~
