#ifndef _INTELLIPILOT_INO_
#include "sys.h"
#define _INTELLIPILOT_INO_

sys _sys;

void setup() {
  _sys.config();
  _sys.start();
}

void loop() {}

#endif
