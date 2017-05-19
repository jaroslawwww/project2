#include "rpt.h"

void errordump(int i)
{
	int temp = cycle + 1;
	if(i == 1) fprintf( file_ptr , "In cycle %d: Write $0 Error\n", temp);
	else if(i == 2) fprintf(file_ptr , "In cycle %d: Number Overflow\n", temp); 
	else if(i == 3)	fprintf(file_ptr , "In cycle %d: Overwrite HI-LO registers\n", temp); 
	else if(i == 4)	fprintf(file_ptr , "In cycle %d: Address Overflow\n", temp); 
	else fprintf(file_ptr , "In cycle %d: Misalignment Error\n", temp);
}