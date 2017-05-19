#include "write.h"

void writeBack(void)
{
	// DM_WB
	//0:Regwrite 1:MemtoReg 2:read_data 3:address_or_result 4:rd_or_rt
	WBstage = DMstage;
	WB_des = DM_WB[4];
	WB_write = DM_WB[0];
	if(DM_WB[0] == 1){
		if(DM_WB[4] == 0) {
			errordump(1);
			return;
		}
		if(DM_WB[1] == 1){
			forwardint = DM_WB[2];
			reg[DM_WB[4]] = DM_WB[2];
		}else if(DM_WB[1] == 0){
			forwardint = DM_WB[3];
			reg[DM_WB[4]] = DM_WB[3];
		} 
	}
	if(DMstage == "JAL") reg[31] = DM_WB[3];
}