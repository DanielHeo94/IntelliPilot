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
  Serial.print(ch[0]);
  Serial.print("\tch2 : ");
  Serial.print(ch[1]);
  Serial.print("\tch3 : ");
  Serial.print(ch[2]);
  Serial.print("\tch4 : ");
  Serial.println(ch[3]);
  
  
}
