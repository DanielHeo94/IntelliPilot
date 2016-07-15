
#include "sys.h"
sys _sys;

void setup() {
  _sys.config();
  _sys.start();
}

void loop() { /* Not used */ }
