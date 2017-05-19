#include "fetch.h"


void instructionFetch(void)
{
	//PCSrc
	
	if(stalled == 1){
		if(doublestall != 1){
			reg[34] += 4;
			int temp = 0;
			temp = (unsigned char)instruct[reg[34]];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)instruct[reg[34]+y]);
			}
			reg[35] = temp;
			return;
		}else{
			/*for(int r=0; r<4; r++){
				buf[r] = (unsigned char)instruct[reg[34]+r];
			}*/
			return;
		}
	}else{
		if(stalled2 == 1){
			if(doublestall == 1){
				for(int r=0; r<4; r++){
					buf[r] = (unsigned char)instruct[reg[34]+r];
				}
				stalled2 = 0;
				doublestall = 0;
				if(flushed != 1){
					IF_ID[0] = reg[34] + 4;
					return;
				} 
				else flag2 = 1;
			}else{
				for(int r=0; r<4; r++){
					buf[r] = (unsigned char)instruct[reg[34]+r];
				}
				stalled2 = 0;
				if(flushed != 1){
					IF_ID[0] = reg[34] + 4;
					return;
				} 
				else flag2 = 1;
			}
		}
		
		if(flushed == 1){
			if(flag2 != 1) reg[34] += 4;
			else flag2 = 0;
			flushed = 0;
			flushed2 = 1;
		}else{
			flag2 = 0;
			reg[34] += 4;
		}
	}
	
	if(flushed3 == 1){
		if(flag == 1) reg[34] = branchaddr;
		else reg[34] = branchaddr;
		
		flushed3 = 0;
	}
	IF_ID[0] = reg[34] + 4;
	int temp = 0;
	temp = (unsigned char)instruct[reg[34]];
	for(int y=1; y<4; y++){
		temp = temp << 8;
		temp = (temp | (unsigned char)instruct[reg[34]+y]);
	}
	reg[35] = temp;
	IF_ID[1] = reg[35];
	for(int r=0; r<4; r++){
		buf[r] = (unsigned char)instruct[reg[34]+r];
	}

}