#include "adv2hr_main.h"

int main()
{
  adv2hr_init();
  while(1)
  {
    adv2hr_thread();
  }
  return 0;
}


