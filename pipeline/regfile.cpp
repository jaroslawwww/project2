#include "regfile.h"
using namespace std;

int reg[];
char data[1024];
char instruct[1024];
FILE* file_ptr;
int termi = 0;
bool error3 = false;
bool jump = false;
int cycle = 0;
//pipeline-register
int IF_ID[2];
int ID_EX[14];
int EX_DM[10];
int DM_WB[5];
int PCSrc = 0;
int forward = 0;
int stalled = 0;
int stalled2 = 0;
int doublestall = 0;
int forwardint = 0;
int WB_des = 0;
int WB_write = 0;
int DM_des = 0;
int DM_write = 0;
int EX_write = 0;
int branchaddr = 0;
int IDEX_memRead = 0;
int IDEX_rsrt = 0;
int IDEX_res = 0;
int EXDM_memRead = 0;
int EXDM_rdrt = 0;
char buf[4];

int forwardint2 = 0;
int flushed = 0;
int flushed2 = 0;
int flushed3 = 0;
int flag = 0;
int flag2 = 0;

string tempstring;
string IDstage = "NOP";
string EXstage = "NOP";
string DMstage = "NOP";
string WBstage = "NOP";

string IFout = "NOP";
string IDout = "NOP";
string EXout = "NOP";
string DMout = "NOP";
string WBout = "NOP";

void regset()
{
	reg[0] = 0; //zero
	reg[1] = 0; //at
	reg[2] = 0; //v0
	reg[3] = 0;	//v1
	reg[4] = 0; //a0
	reg[5] = 0; //a1
	reg[6] = 0; //a2
	reg[7] = 0; //a3
	reg[8] = 0; //t0
	reg[9] = 0; //t1
	reg[10] = 0; //t2
	reg[11] = 0; //t3
	reg[12] = 0; //t4
	reg[13] = 0; //t5
	reg[14] = 0; //t6
	reg[15] = 0; //t7
	reg[16] = 0; //s0
	reg[17] = 0; //s1
	reg[18] = 0; //s2
	reg[19] = 0; //s3
	reg[20] = 0; //s4
	reg[21] = 0; //s5
	reg[22] = 0; //s6
	reg[23] = 0; //s7
	reg[24] = 0; //t8
	reg[25] = 0; //t9
	reg[26] = 0; //k0
	reg[27] = 0; //k1
	reg[28] = 0; //gp
	reg[29] = 0; //sp
	reg[30] = 0; //fp
	reg[31] = 0; //ra
	reg[32] = 0; //hi
	reg[33] = 0; //lo
	reg[34] = 0; //pc
	reg[35] = 0; //IF
}