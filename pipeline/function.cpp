#include "function.h"

unsigned int getrd(unsigned int inst[])
{
	unsigned int retrd = 0, b = 16;
	for(int i=0; i<5; i++){
		retrd += b*inst[16+i];
		b /= 2;
	}
	return retrd;
}
unsigned int getrs(unsigned int inst[])
{
	unsigned int retrs = 0, b = 16;
	for(int i=0; i<5; i++){
		retrs += b*inst[6+i];
		b /= 2;
	}
	return retrs;
}
unsigned int getrt(unsigned int inst[])
{
	unsigned int retrt = 0, b = 16;
	for(int i=0; i<5; i++){
		retrt += b*inst[11+i];
		b /= 2;
	}
	return retrt;
}
unsigned int getshamt(unsigned int inst[])
{
	unsigned int retshamt = 0, b = 16;
	for(int i=0; i<5; i++){
		retshamt += b*inst[21+i];
		b /= 2;
	}
	return retshamt;
}
unsigned int getfunct(unsigned int inst[])
{
	unsigned int retfunct = 0, b = 32;
	for(int i=0; i<6; i++){
		retfunct += b*inst[26+i];
		b /= 2;
	}
	return retfunct;
}
unsigned int getimu(unsigned int inst[])
{
	unsigned int retimu = 0, b = 32768;
	for(int i=0; i<16; i++){
		retimu += b*inst[16+i];
		b /= 2;
	}
	return retimu;
}
int getim(unsigned int inst[])
{
	/*
	int ret, b1, b2;
	b1 = instruct[reg[34]+2];
	b2 = (unsigned char)instruct[reg[34]+3];
	b1 = b1 << 8;
	ret = (b1 | b2);
	return ret;
	*/
	int ret = 0, b = 32768;
	for(int i =0; i<16; i++){
		if(i == 0) ret -= b*inst[16+i];
		else ret += b*inst[16+i];
		b /= 2;
	}
	return ret;
}
unsigned int getaddr(unsigned int inst[])
{
	unsigned int retaddr = 0, b = 33554432;
	for(int i=0; i<26; i++){
		retaddr += b*inst[6+i];
		b /= 2;
	}
	return retaddr;
}