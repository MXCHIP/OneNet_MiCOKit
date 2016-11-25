	
#include "nv_dep.h"
static char  file_buf[1024];

#define SDK_NV_LEN 1024
#define SDK_NV_ADDR (0xC0000 - 0x1000-1024)

int NvitemRead(int id,int len,char *buf)
{

	unsigned short addr=id*NVITEM_GetLength(id);
  memcpy(buf,&file_buf[addr],len);
#if 0
	flash_t flash;
	if(!buf)
	  return -1;

	flash_stream_read(&flash, SDK_NV_ADDR, 512, (uint8_t *) buf);
	#endif
	return 0;
}

int NvitemWrite(int id, int len, char *buf)
{

	unsigned short addr=id*NVITEM_GetLength(id);
	memcpy(&file_buf[addr],buf,len);
  #if 0
        flash_t flash;
        flash_erase_sector(&flash, SDK_NV_ADDR);
        flash_stream_write(&flash, SDK_NV_ADDR, 512, (uint8_t *) buf);         
	#endif 
	return 0;
}

void NvitemDelete(int id)
{
;
}