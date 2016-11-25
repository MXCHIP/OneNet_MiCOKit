#include "nv_dep.h"
#include "onenet_mico_platform.h"

int NvitemRead(int id,int len,char *buf)
{

  unsigned short addr=id*NVITEM_GetLength(id);
  onenet_mico_platform_para_read(buf, addr, len);
  return 0;
}

int NvitemWrite(int id, int len, char *buf)
{
  unsigned short addr=id*NVITEM_GetLength(id);
  onenet_mico_platform_para_write(buf, addr, len);
  return 0;
}

void NvitemDelete(int id)
{
  ;
}
