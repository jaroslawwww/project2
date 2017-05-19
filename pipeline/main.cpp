#include "regfile.h"
#include "function.h"
#include "rpt.h"
#include "instruction.h"
#include "fetch.h"
#include "execute.h"
#include "memory.h"
#include "write.h"
#include <sstream>
using namespace std;

int main()
{
	char buffer[4];
	int i=-1, index=0, max=0;
	regset();
	FILE *fp, *result;
	fpos_t pos;
	fp = fopen("dimage.bin", "rb");
	result = fopen("snapshot.rpt", "w");
	file_ptr = fopen("error_dump.rpt", "w");
	
	if(!fp) return 1;
	while(i++<257){
		if(i>1+max) break;
		int temp = 0;
		fread(buffer, sizeof(char), 4, fp);
		if(i == 0){
			temp = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)buffer[y]);
			}
			reg[29] = temp;
		}else if(i == 1){
			max = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				max = max << 8;
				max = (max | (unsigned char)buffer[y]);
			}
		}else if(i > 1){
			data[index++] = buffer[0];
			data[index++] = buffer[1];
			data[index++] = buffer[2];
			data[index++] = buffer[3];
		}
	}
	fclose(fp);
	fp = fopen("iimage.bin", "rb");
	if(!fp) return 1;
	i = -1, index = 0;
	while(i++<257){
		if(i>1+max) break;
		int temp = 0;
		fread(buffer, sizeof(char), 4, fp);
		if(i == 0){
			temp = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)buffer[y]);
			}
			reg[34] = temp;
			index = reg[34];
		}else if(i == 1){
			max = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				max = max << 8;
				max = (max | (unsigned char)buffer[y]);
			}
		}else if(i > 1){
			instruct[index++] = buffer[0];
			instruct[index++] = buffer[1];
			instruct[index++] = buffer[2];
			instruct[index++] = buffer[3];
		}
	}
	fprintf(result, "cycle %d\n", cycle);
	for(int j=0; j<36; j++){
		if(j<10) fprintf(result, "$0%d: 0x%08X\n", j, reg[j]);
		else if(j<32) fprintf(result, "$%d: 0x%08X\n", j, reg[j]);
		else if(j==32) fprintf(result, "$HI: 0x%08X\n", reg[j]);
		else if(j==33) fprintf(result, "$LO: 0x%08X\n", reg[j]);
		else if(j==34){
			IF_ID[0] = reg[j] + 4;
			fprintf(result, "PC: 0x%08X\n", reg[j]);
		} 
		else{
			int temp = 0;
			temp = (unsigned char)instruct[reg[34]];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)instruct[reg[34]+y]);
			}
			reg[j] = temp;
			fprintf(result, "IF: 0x%08X\n", reg[j]);
			IF_ID[1] = reg[j];
			buf[3] = reg[35];
			for(int r=2; r>=0; r--) buf[r] = reg[35] >> 8*(3-r);
		} 
	}
	fprintf(result, "ID: %s\n", IDout.c_str());
	fprintf(result, "EX: %s\n", EXout.c_str());
	fprintf(result, "DM: %s\n", DMstage.c_str());
	fprintf(result, "WB: %s\n", WBstage.c_str());
	fprintf(result, "\n\n");
	
	
	int diff[35];
	for(int f=0; f<35; f++) diff[f] = reg[f];
	while(1){
		if(termi == 4) break;
		else{
			int x;
			cycle++;
			fprintf(result, "cycle %d\n", cycle);
			for(x=0; x<34; x++){
				if(diff[x] != reg[x]){
					if(x<10) fprintf(result, "$0%d: 0x%08X\n", x, reg[x]);
					else if(x<32) fprintf(result, "$%d: 0x%08X\n", x, reg[x]);
					else if(x==32) fprintf(result, "$HI: 0x%08X\n", reg[x]);
					else if(x==33) fprintf(result, "$LO: 0x%08X\n", reg[x]);
				}
			}
			for(x=0; x<35; x++) diff[x] = reg[x];
			forward = 0;
			printf("cycle%d\n", cycle);
			writeBack();
			memoryAccess();
			execute();
			instructionDecode(&instruct[reg[34]]);
			instructionFetch();
			fprintf(result, "PC: 0x%08X\n", reg[34]);
			/*
			int temp = 0;
			temp = (unsigned char)instruct[reg[34]];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)instruct[reg[34]+y]);
			}
			reg[35] = temp;
			*/
			stringstream ss;
			string tem;
			char hexstr[255];
			sprintf(hexstr, "%08X", reg[35]);
			ss << hexstr;
			tem = ss.str();
			IFout = tem;
			ss.str("");
			IFout = "0x" + IFout;
			if(stalled == 1) IFout = IFout + " to_be_stalled";
			if(flushed2 == 1) IFout = IFout + " to_be_flushed";

			fprintf(result, "IF: %s\n", IFout.c_str());
			fprintf(result, "ID: %s\n", IDout.c_str());
			fprintf(result, "EX: %s\n", EXout.c_str());
			fprintf(result, "DM: %s\n", DMstage.c_str());
			fprintf(result, "WB: %s\n", WBstage.c_str());
			fprintf(result, "\n\n");	
		}
	}
	fclose(fp);
	fclose(result);
	fclose(file_ptr);
	return 0;
}