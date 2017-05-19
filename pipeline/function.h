#ifndef function_H_INCLUDED
#define function_H_INCLUDED
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include "regfile.h"
using namespace std;

unsigned int getrd(unsigned int inst[]);
unsigned int getrs(unsigned int inst[]);
unsigned int getrt(unsigned int inst[]);
unsigned int getshamt(unsigned int inst[]);
unsigned int getfunct(unsigned int inst[]);
unsigned int getimu(unsigned int inst[]);
int getim(unsigned int inst[]);
unsigned int getaddr(unsigned int inst[]);

#endif