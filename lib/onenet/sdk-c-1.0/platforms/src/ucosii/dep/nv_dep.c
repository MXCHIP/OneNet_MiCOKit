#include "24cxx.h" 
#include "delay.h"			
#include "nv_dep.h"
int NvitemRead(int id,int len,char *buf)
{
	unsigned short addr=id*NVITEM_GetLength(id);
	AT24CXX_Read(addr,buf,len);
	return 0;
}

int NvitemWrite(int id, int len, char *buf)
{
	unsigned short addr=id*NVITEM_GetLength(id);
	AT24CXX_Write(addr,buf,len);
	return 0;
}

void NvitemDelete(int id)
{
	unsigned short addr=id*NVITEM_GetLength(id);
	char data=0x0;
	int i=0;
	return;
	for(i=0;i<NVITEM_GetLength(id);i++)
	{
		AT24CXX_Write(addr+i,&data,1);
	}
}