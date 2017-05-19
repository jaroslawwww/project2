#include "execute.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

void execute(void)
{
	// ID_EX
	// 0:Regwrite 1:MemtoReg 2:branch 3:MemRead 4:MemWrite
	// 5:RegDst 6:ALUOp 7:ALUSrc 8:PC+4 9:read_data1 10:read_data2
	// 11:instruct[15:0](im or shamt) 12:rt 13:rd
	
	if(stalled == 1){
		stalled2 = 1;
		EXstage = "NOP";EXout = "NOP";
		EX_DM[0]=0;EX_DM[1]=0;EX_DM[2]=0;EX_DM[3]=0;
		EX_DM[4]=0;EX_DM[5]=0;EX_DM[6]=0;EX_DM[7]=0;
		EX_DM[8]=0;EX_DM[9]=0;
		
		IDEX_memRead = 0;IDEX_rsrt = 0;
		EX_write = 0;
		stalled = 0;
		IDEX_res = 0;
		//doublestall = 0;
		return;
	}
	EXstage = IDstage;
	EXout = IDstage;
	int alu1=0, alu2=0, aluim=0;
	unsigned int alutemp=0;
	string tem;
	stringstream ss;
	alu1 = reg[ID_EX[9]];
	if(ID_EX[7]==0){
		alu2 = reg[ID_EX[10]];
		
	} 
	else{
		aluim = ID_EX[11];
		alutemp = (unsigned int)ID_EX[11];
	} 
	EX_DM[8] = reg[ID_EX[10]];
	//fwd
	if(EXstage == "BEQ" || EXstage == "BNE" || EXstage == "BGTZ" || EXstage == "JR" || EXstage == "JAL" || EXstage == "J"){
		
	}else{
		if(DM_write && (DM_des!=0) && (DM_des == ID_EX[9])){
			alu1 = forwardint2;
			forward = 1;
			ss << ID_EX[9];
			tem = ss.str();
			EXout = EXout + " fwd_EX-DM_rs_$" + tem;
			ss.str("");
		}else if(WB_write && (WB_des!=0) && ~(DM_write&&(DM_des!=0)&& (DM_des!=ID_EX[9])) && (WB_des==ID_EX[9])){
			//alu1 = forwardint;
			forward = 2;
			ss << ID_EX[9];
			tem = ss.str();
			EXout = EXout + " fwd_DM-WB_rs_$" + tem;
			ss.str("");
		}
		if(ID_EX[5] != 0){
			if(DM_write && (DM_des!=0) && (DM_des == ID_EX[10])){
				alu2 = forwardint2;
				EX_DM[8] = forwardint2;
				forward = 1;
				ss << ID_EX[10];
				tem = ss.str();
				EXout = EXout + " fwd_EX-DM_rt_$" + tem;
				ss.str("");
			}else if(WB_write && (WB_des!=0) && ~(DM_write&&(DM_des!=0)&&(DM_des!=ID_EX[10])) && (WB_des==ID_EX[10])){
				//alu2 = forwardint;
				//EX_DM[8] = forwardint;
				forward = 2;
				ss << ID_EX[10];
				tem = ss.str();
				EXout = EXout + " fwd_DM-WB_rt_$" + tem;
				ss.str("");
			}
		}
	}
	if(ID_EX[5] == 1){
		EX_DM[9] = ID_EX[13];
		IDEX_rsrt = ID_EX[13];
	} 
	else if(ID_EX[5] == 0){
		EX_DM[9] = ID_EX[12];
		IDEX_rsrt = ID_EX[12];
	}else{
		EX_DM[9] = ID_EX[12];
		IDEX_rsrt = ID_EX[12];
	}
	
	IDEX_memRead = ID_EX[3];
	EX_write = ID_EX[0];
	IDEX_res = EX_DM[7];
	EX_DM[0]=ID_EX[0];EX_DM[1]=ID_EX[1];EX_DM[2]=ID_EX[2];
	EX_DM[3]=ID_EX[3];EX_DM[4]=ID_EX[4];
		
	EX_DM[5]=ID_EX[8]+4*ID_EX[11];
	EX_DM[6]=0;
	
	long long int mul;
	unsigned long long int mulu;
	int forlo;
	//7
	if(ID_EX[6] == 0){ //lw lh lhu lb lbu sw sh sb
		if(IDstage == "LW"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "LH"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "LHU"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "LB"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "LBU"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "SW"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "SH"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "SB"){
			if(alu1>=0 && alu1+aluim<0 && aluim>=0) errordump(2);
			if(alu1<0 && aluim<0 && alu1+aluim>=0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}
	}else if(ID_EX[6] == 1){ //beq bne bgtz j jal jr
		if(IDstage == "BEQ"){
			//EX_DM[5] = ID_EX[8]+4*ID_EX[11];
		}else if(IDstage == "BNE"){
			//EX_DM[5] = ID_EX[8]+4*ID_EX[11];
		}else if(IDstage == "BGTZ"){
			//EX_DM[5] = ID_EX[8]+4*ID_EX[11];
		}else if(IDstage == "J"){
			//EX_DM[5] = ((ID_EX[8]>>28)<<28) | ID_EX[11]<<2;
		}else if(IDstage == "JAL"){
			//EX_DM[5] = ((ID_EX[8]>>28)<<28) | ID_EX[11]<<2;
			EX_DM[7] = ID_EX[8];
		}else if(IDstage == "JR"){
			//EX_DM[5] = alu1;
		}
	}else{ //R-type
		if(IDstage == "ADD"){
			if(alu1 >= 0 && alu2 >= 0 && alu1+alu2 < 0) errordump(2);
			if(alu1 < 0 && alu2 < 0 && alu1+alu2 >= 0) errordump(2);
			EX_DM[7] = alu1 + alu2;
		}else if(IDstage == "ADDU"){
			EX_DM[7] = alu1 + alu2;
		}else if(IDstage == "SUB"){
			if(alu1 >= 0 && (-alu2) >= 0 && alu1 + (-alu2) < 0) errordump(2);
			if(alu1 < 0 && (-alu2) < 0 && alu1 + (-alu2) >= 0) errordump(2);
			EX_DM[7] = alu1 + (-alu2);
		}else if(IDstage == "AND"){
			EX_DM[7] = alu1 & alu2;
		}else if(IDstage == "OR"){
			EX_DM[7] = alu1 | alu2;
		}else if(IDstage == "XOR"){
			EX_DM[7] = alu1 ^ alu2;
		}else if(IDstage == "NOR"){
			EX_DM[7] = ~(alu1 | alu2);
		}else if(IDstage == "NAND"){
			EX_DM[7] = ~(alu1 & alu2);
		}else if(IDstage == "SLT"){
			EX_DM[7] = (alu1 < alu2);
		}else if(IDstage == "SLL"){
			EX_DM[7] = alu2 << ID_EX[11];
		}else if(IDstage == "SRL"){
			EX_DM[7] = (unsigned int)alu2 >> ID_EX[11];
		}else if(IDstage == "SRA"){
			EX_DM[7] = alu2 >> ID_EX[11];
		}else if(IDstage == "MULT"){
			if(error3 == 1) errordump(3);
			else error3 = 1;

			//if(alu1 >= 0 && alu2 >= 0 && alu1 * alu2 < 0) errordump(2);
			long long int ma = alu1, mb = alu2;
			mul = ma * mb;
			forlo = mul;
			reg[32] = mul >> 32;
			reg[33] = forlo;
		}else if(IDstage == "MULTU"){
			if(error3 == 1) errordump(3);
			else error3 = 1;
			unsigned long long int a = (unsigned int)alu1, b = (unsigned int)alu2;
			mulu = a * b;
			forlo = mulu;
			reg[32] = mulu >> 32;
			reg[33] = forlo;
		}else if(IDstage == "MFHI"){
			if(error3 == 1) error3 = 0;
			EX_DM[7] = reg[32];
		}else if(IDstage == "MFLO"){
			if(error3 == 1) error3 = 0;
			EX_DM[7] = reg[33];
		}else if(IDstage == "ADDI"){
			if(alu1 >= 0 && aluim >= 0 && alu1+aluim < 0) errordump(2);
			if(alu1 < 0 && aluim < 0 && alu1+aluim >= 0) errordump(2);
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "ADDIU"){
			EX_DM[7] = alu1 + aluim;
		}else if(IDstage == "LUI"){
			EX_DM[7] = aluim << 16;
		}else if(IDstage == "ANDI"){
			EX_DM[7] = alu1 & alutemp;
		}else if(IDstage == "ORI"){
			EX_DM[7] = alu1 | alutemp;
		}else if(IDstage == "NORI"){
			EX_DM[7] = ~(alu1 | alutemp);
		}else if(IDstage == "SLTI"){
			EX_DM[7] = (alu1 < aluim);
		}
	}	

}
