#include "remote.h"

remote mRemote;

float ch[4];

void setup() {
  Serial.begin(115200);
  Serial.flush();

  mRemote.begin();
}

void loop() {
  mRemote.getCommands(ch);

  Serial.print("ch1 : ");
  Serial.println(ch[0]);
}
