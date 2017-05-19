#include "instruction.h"
#include <sstream>
void instructionDecode(char buffer[])
{
	unsigned int ins[32] = {0};
	unsigned int temp, a, index=0;
	int opcode = 0, b = 32;
	if(flushed2 == 1){
		flushed3 = 1;
		flushed2 = 0;
	}else{
		for(int i=0; i<4; i++){
				temp = buf[i];
				for(int j=7; j>=0; j--){
					a = temp % 2;
					temp = temp/2;
					ins[index+j] = a;
				}
				index += 8;
			}
			for(int i=0; i<6; i++){
				opcode += b*ins[i];
				b /= 2;
			}
	}
	
	unsigned int funct = getfunct(ins);
	unsigned int rd = getrd(ins), rs = getrs(ins), rt = getrt(ins), shamt = getshamt(ins), imu = getimu(ins), addr = getaddr(ins);
	int im = getim(ins);
	long long int mul;
	unsigned long long int mulu;
	int forlo;
	int A, B;
	string tem;
	stringstream ss;
	A = reg[rs];
	B = reg[rt];
	
	tempstring = IDstage;
	if(opcode == 0){
		if(funct == 32){//add
			IDstage = "ADD";
		}else if(funct == 33){//addu
			IDstage = "ADDU";
		}else if(funct == 34){//sub
			IDstage = "SUB";
		}else if(funct == 36){//and
			IDstage = "AND";
		}else if(funct == 37){//or
			IDstage = "OR";
		}else if(funct == 38){//xor
			IDstage = "XOR";
		}else if(funct == 39){//nor
			IDstage = "NOR";
		}else if(funct == 40){//nand
			IDstage = "NAND";
		}else if(funct == 42){//slt
			IDstage = "SLT";
		}else if(funct == 0){//sll
			IDstage = "SLL";
			rs = 0;
			if(rt == 0 && rd == 0 && shamt ==0){
				IDstage = "NOP";
			}
		}else if(funct == 2){//srl
			IDstage = "SRL";
			rs = 0;
		}else if(funct == 3){//sra
			IDstage = "SRA";
			rs = 0;
		}else if(funct == 8){//jr
			IDstage = "JR";
			rt = 0;
		}else if(funct == 24){//mult
			IDstage = "MULT";
		}else if(funct == 25){//multu
			IDstage = "MULTU";
		}else if(funct == 16){//mfhi
			IDstage = "MFHI";
		}else if(funct == 18){//mflo
			IDstage = "MFLO";
		}
	}else if(opcode == 8){//addi
		IDstage = "ADDI";
	}else if(opcode == 9){//addiu
		IDstage = "ADDIU";
	}else if(opcode == 35){//lw
		IDstage = "LW";
	}else if(opcode == 33){//lh
		IDstage = "LH";
	}else if(opcode == 37){//lhu
		IDstage = "LHU";
	}else if(opcode == 32){//lb
		IDstage = "LB";
	}else if(opcode == 36){//lbu
		IDstage = "LBU";
	}else if(opcode == 43){//sw
		IDstage = "SW";
	}else if(opcode == 41){//sh
		IDstage = "SH";
	}else if(opcode == 40){//sb
		IDstage = "SB";
	}else if(opcode == 15){//lui
		IDstage = "LUI";
		rs = 0;
	}else if(opcode == 12){//andi
		IDstage = "ANDI";
	}else if(opcode == 13){//ori
		IDstage = "ORI";
	}else if(opcode == 14){//nori
		IDstage = "NORI";
	}else if(opcode == 10){//slti
		IDstage = "SLTI";
	}else if(opcode == 4){//beq
		IDstage = "BEQ";
	}else if(opcode == 5){//bne
		IDstage = "BNE";
	}else if(opcode == 7){//bgtz
		IDstage = "BGTZ";
		rt = 0;
	}else if(opcode == 2){//j
		IDstage = "J";
	}else if(opcode == 3){//jal
		IDstage = "JAL";
	}else if(opcode == 63){//halt
		IDstage = "HALT";
	}
	
	if(doublestall != 1) IDout = IDstage;
	else{
		IDout = tempstring;
	} 
	//stall-1
	
	if(opcode != 0){
		if(IDstage == "SW" || IDstage == "SH" || IDstage == "SB"){
			if(IDEX_memRead && (IDEX_rsrt == rt) && rt != 0 && IDEX_rsrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
			if(IDEX_memRead && (IDEX_rsrt == rs) && rs != 0 && IDEX_rsrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}else if(IDstage != "HALT"){
			if(IDEX_memRead && (IDEX_rsrt == rs)  && rs != 0 && IDEX_rsrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}
	}else{
		if(IDstage != "MFHI" && IDstage != "MFLO"){
			if(IDEX_memRead && (IDEX_rsrt == rs) && IDEX_rsrt != 0 && rs !=0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
			if(IDEX_memRead && (IDEX_rsrt == rt) && IDEX_rsrt != 0 && rt !=0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}
	}
	
	//stall-2
	if(IDstage == "BEQ" || IDstage == "BNE" || IDstage == "BGTZ" || IDstage == "JR" || IDstage == "JAL" || IDstage == "J"){
		//do something R-type beq
		if(IDstage == "BGTZ"){
			if(EX_write && IDEX_rsrt == rs && IDEX_rsrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}else{
			if(EX_write && ((IDEX_rsrt == rs)||(IDEX_rsrt == rt)) && IDEX_rsrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}
		if(stalled2 == 1){
			if(EXDM_memRead && ((EXDM_rdrt == rs) || (EXDM_rdrt == rt)) && EXDM_rdrt != 0){
				stalled = 1;
				doublestall = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}else{
			if(EXDM_memRead && ((EXDM_rdrt == rs) || (EXDM_rdrt == rt)) && EXDM_rdrt != 0){
				stalled = 1;
				IDout = IDout + " to_be_stalled";
				return;
			}
		}

	}
	
	//fwd
	if(IDstage == "BEQ" || IDstage == "BNE" || IDstage == "BGTZ" || IDstage == "JR" || IDstage == "JAL" || IDstage == "J"){
		if(!(forward != 0 && EX_write == 1 && ((IDEX_rsrt == rs)||(IDEX_rsrt == rt)))){
			if(IDstage == "JR"){
				if(DM_write && (DM_des!=0) && (DM_des == rs)){
					A = forwardint2;
					ss << rs;
					tem = ss.str();
					IDout = IDout + " fwd_EX-DM_rs_$" + tem;
					ss.str("");
				}
			}else{
				if(DM_write && (DM_des!=0) && (DM_des == rs)){
					A = forwardint2;
					ss << rs;
					tem = ss.str();
					IDout = IDout + " fwd_EX-DM_rs_$" + tem;
					ss.str("");
				} 
				if(DM_write && (DM_des!=0) && (DM_des == rt)){
					B = forwardint2;
					ss << rt;
					tem = ss.str();
					IDout = IDout + " fwd_EX-DM_rt_$" + tem;
					ss.str("");
				}
			}			 
		}
	}
	ID_EX[8]=IF_ID[0];
	PCSrc = 0;
	if(opcode == 0){
		ID_EX[0]=1;
		ID_EX[1]=0;
		ID_EX[2]=0;
		ID_EX[3]=0;
		ID_EX[4]=0;
		ID_EX[5]=1;
		ID_EX[6]=2;
		ID_EX[7]=0;
		ID_EX[9]=rs;
		ID_EX[10]=rt;
		ID_EX[11]=0;
		ID_EX[12]=rt;
		ID_EX[13]=rd;
		if(funct == 32){//add
		}else if(funct == 33){//addu
		}else if(funct == 34){//sub
		}else if(funct == 36){//and
		}else if(funct == 37){//or
		}else if(funct == 38){//xor
		}else if(funct == 39){//nor
		}else if(funct == 40){//nand
		}else if(funct == 42){//slt
		}else if(funct == 0){//sll
			if(IDstage == "NOP"){
				ID_EX[0]=0;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
				ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=0;
				ID_EX[8]=0;ID_EX[9]=0;ID_EX[10]=0;ID_EX[11]=0;
				ID_EX[12]=0;ID_EX[13]=0;
			}else{
				ID_EX[9]=0;ID_EX[10]=rt;ID_EX[11]=shamt;
			}
		}else if(funct == 2){//srl
			ID_EX[9]=0;ID_EX[10]=rt;ID_EX[11]=shamt;
		}else if(funct == 3){//sra
			ID_EX[9]=0;ID_EX[10]=rt;ID_EX[11]=shamt;
		}else if(funct == 8){//jr
			ID_EX[0]=0;ID_EX[5]=0;ID_EX[6]=1;ID_EX[7]=1;
			ID_EX[9]=rs;ID_EX[12]=0;ID_EX[13]=0;ID_EX[10]=0;
			flushed = 1;
			PCSrc = 1;
			branchaddr = A;
			flag = 0;
			jump = true;
		}else if(funct == 24){//mult
			ID_EX[0]=0;ID_EX[12]=0;ID_EX[13]=0;
		}else if(funct == 25){//multu
			ID_EX[0]=0;ID_EX[12]=0;ID_EX[13]=0;
		}else if(funct == 16){//mfhi
			ID_EX[9]=0;ID_EX[10]=0;ID_EX[12]=0;ID_EX[13]=rd;
		}else if(funct == 18){//mflo
			ID_EX[9]=0;ID_EX[10]=0;ID_EX[12]=0;ID_EX[13]=rd;
		}
	}else if(opcode == 8){//addi
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 9){//addiu
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 35){//lw
		ID_EX[0]=1;ID_EX[1]=1;ID_EX[2]=0;ID_EX[3]=1;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 33){//lh
		ID_EX[0]=1;ID_EX[1]=1;ID_EX[2]=0;ID_EX[3]=1;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 37){//lhu
		ID_EX[0]=1;ID_EX[1]=1;ID_EX[2]=0;ID_EX[3]=1;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 32){//lb
		ID_EX[0]=1;ID_EX[1]=1;ID_EX[2]=0;ID_EX[3]=1;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 36){//lbu
		ID_EX[0]=1;ID_EX[1]=1;ID_EX[2]=0;ID_EX[3]=1;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 43){//sw
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=1;ID_EX[5]=2;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 41){//sh
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=1;ID_EX[5]=2;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 40){//sb
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=1;ID_EX[5]=2;ID_EX[6]=0;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 15){//lui
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=0;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 12){//andi
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=imu;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 13){//ori
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=imu;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 14){//nori
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=imu;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 10){//slti
		ID_EX[0]=1;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=2;ID_EX[7]=1;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=rt;ID_EX[13]=0;
	}else if(opcode == 4){//beq
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[3]=0;//ID_EX[1]=>dont care
		ID_EX[4]=0;ID_EX[5]=2;ID_EX[6]=1;ID_EX[7]=0;//ID_EX[5]=>dont care
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=0;ID_EX[13]=0;

		if(A == B){
			ID_EX[2] = 1;
			PCSrc = 1;
			branchaddr = IF_ID[0]+4*im;
			flag = 1;
			flushed = 1;
		} 
		else ID_EX[2] = 0;
	}else if(opcode == 5){//bne
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=2;ID_EX[6]=1;ID_EX[7]=0;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=0;ID_EX[13]=0;
		
		if(A != B){
			ID_EX[2] = 1;
			PCSrc = 1;
			branchaddr = IF_ID[0]+4*im;
			flag = 1;
			flushed = 1;
		} 
		else ID_EX[2] = 0;
	}else if(opcode == 7){//bgtz
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[2]=1;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=2;ID_EX[6]=1;ID_EX[7]=0;
		ID_EX[9]=rs;ID_EX[10]=rt;
		ID_EX[11]=im;ID_EX[12]=0;ID_EX[13]=0;
		
		if(A > 0){
			ID_EX[2] = 1;
			PCSrc = 1;
			branchaddr = IF_ID[0]+4*im;
			flag = 1;
			flushed = 1;
		} 
		else ID_EX[2] = 0;
	}else if(opcode == 2){//j
		ID_EX[0]=0;ID_EX[1]=2;ID_EX[2]=1;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=2;ID_EX[6]=1;ID_EX[7]=0;
		ID_EX[9]=0;ID_EX[10]=0;
		ID_EX[11]=addr;ID_EX[12]=0;ID_EX[13]=0;
		jump = true;
		PCSrc = 1;
		branchaddr = ((IF_ID[0]>>28)<<28) | addr<<2;
		flag = 0;
		flushed = 1;
	}else if(opcode == 3){//jal
		ID_EX[0]=1;ID_EX[1]=2;ID_EX[2]=1;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=2;ID_EX[6]=1;ID_EX[7]=0;
		ID_EX[9]=0;ID_EX[10]=0;
		ID_EX[11]=addr;ID_EX[12]=31;ID_EX[13]=0;
		jump = true;
		PCSrc = 1;
		branchaddr = ((IF_ID[0]>>28)<<28) | addr<<2;
		flag = 0;
		flushed = 1;
		ID_EX[8]=IF_ID[0]; /////////problem
	}else if(opcode == 63){//halt
		ID_EX[0]=0;ID_EX[1]=0;ID_EX[2]=0;ID_EX[3]=0;
		ID_EX[4]=0;ID_EX[5]=0;ID_EX[6]=0;ID_EX[7]=0;
		ID_EX[9]=0;ID_EX[10]=0;
		ID_EX[11]=0;ID_EX[12]=0;ID_EX[13]=0;
		if(termi != 4)termi++;
	}
}