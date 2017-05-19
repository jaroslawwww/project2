#include "memory.h"

void memoryAccess(void)
{
	//EX_DM
	//0:Regwrite 1:MemtoReg 2:branch 3:MemRead 4:MemWrite
	//5:branchaddr 6:branch(zero) 7:ALUresult 8:data2
	//9:rd_or_rt
	
	DMstage = EXstage;
	EXDM_memRead = EX_DM[3];
	EXDM_rdrt = EX_DM[9];
	DM_des = EX_DM[9];
	DM_write = EX_DM[0];
	forwardint2 = EX_DM[7];
	DM_WB[0] = EX_DM[0];
	DM_WB[1] = EX_DM[1];
	DM_WB[4] = EX_DM[9];
	//PCSrc = EX_DM[2];
	//branchaddr = EX_DM[5];
	int temp1 = 0;
	unsigned int temp2 = 0;
	
	if(EX_DM[2] == 0){
		if(EX_DM[3] == 1){ //lw lh lhu lb lbu
			if(EXstage == "LW"){
				if(EX_DM[7] >1020 || EX_DM[7]<0){
					errordump(4);
					termi = 4;
				}
				if((EX_DM[7])%4 != 0){
					errordump(5);
					termi = 4;
				}
				if(termi == 4) return;
				temp1 = (unsigned char)data[EX_DM[7]];
				for(int r=1; r<4; r++){	
					temp1 = temp1 << 8;
					temp1 = temp1 | (unsigned char)data[EX_DM[7]+r];
				}
				DM_WB[2] = temp1;
				DM_WB[3] = EX_DM[7];
			}else if(EXstage == "LH"){
				if(EX_DM[7] > 1022 || EX_DM[7] < 0){
					errordump(4);
					termi = 4;
				}
				if(EX_DM[7]%2 != 0){
					errordump(5);
					termi = 4;
				}
				if(termi == 4) return;
				temp1 = data[EX_DM[7]];
				for(int r=1; r<2; r++){
					temp1 = temp1 << 8;
					temp1 = temp1 | (unsigned char)data[EX_DM[7]+r];
				}
				DM_WB[2] = temp1;
				DM_WB[3] = EX_DM[7];
			}else if(EXstage == "LHU"){
				if(EX_DM[7] > 1022 || EX_DM[7] < 0){
					errordump(4);
					termi = 4;
				}
				if(EX_DM[7]%2 != 0){
					errordump(5);
					termi = 4;
				}
				if(termi == 4) return;
				temp2 = (unsigned char)data[EX_DM[7]];
				temp2 = temp2 << 8;
				temp2 = temp2 | (unsigned char)data[EX_DM[7]+1];
				DM_WB[2] = temp2;
				DM_WB[3] = EX_DM[7];
			}else if(EXstage == "LB"){
				if(EX_DM[7] > 1023 || EX_DM < 0){
					errordump(4);
					termi = 4;
				}
				if(termi == 4) return;
				DM_WB[2] = data[EX_DM[7]];
				DM_WB[3] = EX_DM[7];
			}else{
				if(EX_DM[7] > 1023 || EX_DM < 0){
					errordump(4);
					termi = 4;
				}
				if(termi == 4) return;
				temp2 = (unsigned char)data[EX_DM[7]];
				DM_WB[2] = temp2;
				DM_WB[3] = EX_DM[7];
			}
		}else if(EX_DM[4] == 1){ //sw sh sb
			if(EXstage == "SW"){
				if(EX_DM[7] > 1020 || EX_DM[7] < 0){
					errordump(4);
					termi = 4;
				}
				if(EX_DM[7]%4 != 0){
					errordump(5);
					termi = 4;
				}
				if(termi == 4) return;
				DM_WB[2] = 0;
				DM_WB[3] = EX_DM[7];
				data[EX_DM[7]+3] = EX_DM[8];
				for(int r=2; r>=0; r--) data[EX_DM[7]+r] = EX_DM[8] >> 8*(3-r);
			}else if(EXstage == "SH"){
				if(EX_DM[7] > 1022 || EX_DM[7] < 0){
					errordump(4);
					termi = 4;
				}
				if(EX_DM[7]%2 != 0){
					errordump(5);
					termi = 4;
				}
				if(termi == 4) return;
				DM_WB[2] = 0;
				DM_WB[3] = EX_DM[7];
				data[EX_DM[7]+1] = EX_DM[8];
				data[EX_DM[7]] = EX_DM[8] >> 8;
			}else if(EXstage == "SB"){
				if(EX_DM[7] > 1023 || EX_DM[7] < 0){
					errordump(4);
					termi = 4;
				}
				if(termi == 4) return;
				DM_WB[2] = 0;
				DM_WB[3] = EX_DM[7];
				data[EX_DM[7]] = EX_DM[8];
			}
		}else{ //R-type
			DM_WB[2] = 0;
			DM_WB[3] = EX_DM[7];
		}
	}else{ //branch: beq, bne, bgtz, j, jal, jr
		if(EXstage == "JAL"){
			DM_WB[2] = 0;
			DM_WB[3] = EX_DM[7];
		}else{
			DM_WB[2] = 0;
			DM_WB[3] = EX_DM[7];
		} 
	}
	
}