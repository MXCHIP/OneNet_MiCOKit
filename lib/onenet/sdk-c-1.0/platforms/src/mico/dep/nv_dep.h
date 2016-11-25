#ifndef __NV_DEP__
#define __NV_DEP__
int NvitemRead(int id,int len,char *buf);

int NvitemWrite(int id, int len, char *buf);
extern int NVITEM_GetLength(int id);
void NvitemDelete(int id);
#endif
