#ifndef regfile_H_INCLUDED
#define regfile_H_INCLUDED

#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

extern int reg[36];
extern char data[1024];
extern char instruct[1024];
extern FILE* file_ptr;
extern int termi;
extern int cycle;
extern bool error3;
extern bool jump;
extern int IF_ID[2];
extern int ID_EX[14];
extern int EX_DM[10];
extern int DM_WB[5];
extern int PCSrc;
extern int forward;
extern int stalled;
extern int stalled2;
extern int doublestall;

extern int forwardint;
extern int WB_des;
extern int WB_write;
extern int DM_des;
extern int DM_write;
extern int EX_write;

extern int branchaddr;
extern int IDEX_memRead;
extern int IDEX_rsrt;
extern int IDEX_res;
extern int EXDM_memRead;
extern int EXDM_rdrt;
extern int forwardint2;

extern char buf[4];
extern int flushed;
extern int flushed2;
extern int flushed3;
extern int flag;
extern int flag2;

extern string tempstring;
extern string IDstage;
extern string EXstage;
extern string DMstage;
extern string WBstage; 

extern string IFout;
extern string IDout;
extern string EXout;
extern string DMout;
extern string WBout;

void regset(void);

#endif