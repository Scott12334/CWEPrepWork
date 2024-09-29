#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char ** argv){
	  int i;
  int j;
  long local_98;
  long local_90;
  long local_88;
 long local_80;
  long local_78;
  long local_70;
  long local_68;
  long local_60;
  long local_58;
  long local_50;
  long local_48;
  long local_40;
  long local_38;
  long local_30;
  long local_28;
  long local_20;
  char flag[300];
  memset(flag,0,200);
  local_98 = 0x304e3264684e3359;
  local_90 = 0x7a59334d4f746e5a;
  local_88 = 0x33416a6377396c63;
  local_80 = 0x7a4d3358334d324d;
  local_78 = 0x32467a4e784d6e62;
  local_70 = 0x77596d627838314d;
  local_68 = 0x77457a4e30306d63;
  local_60 = 0x75467a6331396c62;
  local_58 = 0x334d58647139315a;
  local_50 = 0x6b427a59754e7a58;
  local_48 = 0x6a3856496e35574d;
  local_40 = 0x7851474d6a35324d;
  local_38 = 0x664e584d66646d62;
  local_30 = 0x754e7a5830426a62;
  local_28 = 0x78634463354a3359;
  local_20 = 0x3d3d51666834474d;
  for (i = 0; i < 0x10; i = i + 1) {
    for (j = 0; j < 8; j = j + 1) {
      flag[(long)j + (long)i * 8] = *(long *)((long)&local_98 + (long)j + (long)i * 8);
    }
  }
  printf("Tesst");
  printf("Flag: %s", flag);
  printf("\nLength %d",strlen(flag));
	return 0;
}
