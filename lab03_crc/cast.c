#include <stdint.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
  uint32_t data = 0xFFFFAAAA;
  printf("Початкова послідовність: 0x%X\n", data);

  uint16_t cast = data;
  printf("Приведена послідовність: 0x%X\n", cast);

  return 0;
}
