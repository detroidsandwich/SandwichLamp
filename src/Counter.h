#ifndef COUNTER_H
#define COUNTER_H

#include <stdint.h>

struct Counter
{
  uint16_t all;
  uint16_t  enabled;
  uint16_t  disabled;
  uint16_t  alarm;
  uint16_t  maintenance;
};


#endif