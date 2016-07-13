//
//  scheduler.cpp
//  intellipilot
//
//  Created by 허진수 on 2016. 7. 12..
//  Copyright © 2016년 dronix.kr. All rights reserved.
//

#include "scheduler.h"
#include "tasks.h"

#include <FreeRTOS_ARM.h>

tasks _tasks;

scheduler::scheduler() {}

void scheduler::taskCreate() {
    
    Serial.println("Creating tasks");
    
    portBASE_TYPE sa1, sa2, sa3, sa4, sa5;
    portBASE_TYPE c1, c2;
    portBASE_TYPE cp1;
    portBASE_TYPE fc1, fc2, fc3, fc4, fc5;
    portBASE_TYPE l1;
    
    Serial.print("\t\tgetEulerAnglesGyroThread..."); sa1 = xTaskCreate(_tasks.getEulerAnglesGyroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL); Serial.println("\t\tSuccess.");
    Serial.print("\t\tgetAccAltThread...");          sa2 = xTaskCreate(_tasks.getAccAltThread, NULL, configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);          Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tgetBaroThread...");            sa3 = xTaskCreate(_tasks.getBaroThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);            Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tgetCompThread...");            sa4 = xTaskCreate(_tasks.getCompThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);            Serial.println("\t\t\tSuccess.");
    Serial.print("\t\tgetGpsThread...");             sa5 = xTaskCreate(_tasks.getGpsThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);             Serial.println("\t\t\t\tSuccess.");
    
    Serial.print("\t\tgetCommandsThread...");        c1 = xTaskCreate(_tasks.getCommandsThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);         Serial.println("\t\tSuccess.");
    
    if (sa1 != pdPASS
        || sa2 != pdPASS
        || sa3 != pdPASS
        || sa4 != pdPASS
        || sa5 != pdPASS
        || c1  != pdPASS
        ) {
        Serial.println(F("Creation problem"));
        while(1);
    }
}

void scheduler::taskStart() {
    
    // Start scheduler
    Serial.println("Start scheduler.");
    Serial.println(":;;i;;;;;::::;;::;;;;;;;;;;;ii;i;;;;;;:::;;;::::::::::::,,,,,,.");
    Serial.println(";;iii1;iiiiiii;i;;;;;;;;;;;;;;;;;;;;;;;;;::::::::;::::::::,,,,,");
    Serial.println(";ii111i111111iiiiiiiiii;;;;ii;ii;;;;;;;;::::::::::::::::;;::,,,");
    Serial.println(";ii11111111111iiiiii;;::,,::::,,,,,,,,,,,,,,,,,,,,,,,,..,:::,,,");
    Serial.println(";ii1i1tttt::,::,,:::,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:,,,");
    Serial.println(";iii1t1:,,...;:,,,,,,,,,,,,,,,,,,,,,,,,,,,,, :,,,,,,,,,,;::::,,");
    Serial.println(";iii1tt;,.::,,, ;i1  .,,,,,,,,,,,,,,,,,,,,,, 1,,,,,,,,,,,::::,,");
    Serial.println(";iii1tt:,.:,:  t,,.   i,,,,,,,,,,,,,,,,,,,,, ;,,,,,,,,,,,::::,,");
    Serial.println("iiiii1t::i:,   :,,,;;;,,,....,,,. ;i..:,. 1;  ,,,,,,,,,,,:;::::");
    Serial.println(";iiii11::,,,   ,,,,.  ., ::,. ,. .,,, ;,  ,,  ;,,,,,,,,,,;;;:::");
    Serial.println(";iiii11;:,,,   :,:;   ;. :,,, ., .,. ,i,,   ..,,, .. ;,,,;ii;::");
    Serial.println("iiii11t1:,,:.   ,,,  t,,, . .t,,,;ii,,,,,,,,,,,,,  .i::,,;ii;::");
    Serial.println("iiii11t1::,,,:.  ..1;,,,,,,,,,,,,,,,,,,, ;,,,,,,,.  t:,,:iii;;:");
    Serial.println(";ii111f1,:,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, 1,,.,,,,.  ::,,:;ii;;:");
    Serial.println(";iii11t1,,,,,,, i,,,,,,, .,,,,. :. i;.., . t:,,,,. ;:,,,::;;;::");
    Serial.println(";iiii1t1,,,,,,,  ,,,,,,,..:,,,  i  ;,,,, ..::  i,. ;,,,,:::;;::");
    Serial.println("iiiii1t1,,,,,,,  ,,,,,,,  :,,,  1. :, .: .,i,,i:,,::,,,,:::;:::");
    Serial.println("iiiii1t1,,,,,,,  ,,,,,,,.... ,; 1,:1;;:,:,:,,,,,,..t::::.,:::::");
    Serial.println("iiiiii11,:,::::  .   :t,,,,,,::::::::::,,.   ..,t,::::::.;:::::");
    Serial.println("iiiii11::,,::,:11;,,,,,:,,,..  ..:i11;::...,  .,,:i1111:.i::;;:");
    Serial.println("iiii;i1:::...,;,,,,,,,,:::::,::::,,,,,:,,,,,,,,,,,,.....:;::;;:");
    Serial.println("i;i;;ii111ii::iiiiiii1111i;;::::::::::::::,,,,,,,,:::::,:::;;;;");
    Serial.println("iii;;;iiiiiiiiiiiiii11111iiiiiiiii;iiiiiiiii11ii:;;;;::,:;;;;i;");
    Serial.println(";ii;;i111ii;;;;iiiiii111111iiiiiii;;iiii;;;i11iii;;;;::,,;;iii;");
    Serial.println(";;;::i11ii;;;;;iiiiiiiiiiiiiiiiiiii;iiiiiiiiiiiiiiiii;:,,:;iiii");
    Serial.println(":;;::;iiiiii;iiiii;;iii1iiiitiiiiiiiiiiiiiiii11iiiiiii;::;iiii;");
    Serial.println(":;;::;;;iii;;;i;i;iiiiiiiiiititiiLfiiiiiiiiiii11i1i11i;::;;iiii");
    Serial.println("::;::;:;iiii;ii;iiiiifiit,,1::,,,,,,,;i1fiiiiii11111i;;::;;ii1i");
    Serial.println(":;:::::;;;iiiiiiiiii;,::,,,::,::::itft;:,tiiiii11iiii;:::;ii11i");
    Serial.println("::,,,::;iiii;iiii1,::1fLGCi::::ifCtiiitLCL,i;iiiift;;;:::ii111i");
    Serial.println("::,.,:::iiiiiiit1:ttf1;::i1LG1Lf:;::,::;;LG,.i;;8@8C:::::;it11i");
    Serial.println("::,,,:::iiiiitf;:Cf;;:,,,:;iiCL;:.  . .,:iLC@1;;;88@@@f::i1111i");
    Serial.println("::,,,:;;;;;iii,:1f1;,. ;1 .:;LCi:.8808L.::LL8@@tG888@@1::i11i11");
    Serial.println("::,,:;;i;;;;ii,@Ctt:,.@@88:,;fLf;:.G8G,::fLtit88;@@@@:::;ii1111");
    Serial.println("::,,:iiii;;;i@@@8Ct;:,.LG,::iLLCf1,:,,:tCLi;;;;8;@@8@;;;:ii11i1");
    Serial.println("::,,:;ii;iiii@8@i;LLfi:,,;1LCfii1Cffftt01i;:,:;,i;i11;;;;1i1ii1");
    Serial.println("::,,:;i1iiii1f@::::;1GLtfLL1;;::;;;;iii;;::,,:;:1i:;iii:;ii1ii1");
    Serial.println("::,,:;i11ii1iii:::::::;;;;:::,,::::::::,,:,,,,;;1i:;iii;;111ii1");
    Serial.println(",::,:ii111iiiii:::,,,,,,,,,:::,::::::,:,:,::::;iii:;iiii;111ii1");
    Serial.println(",:::i1111iiiiii;::,,:,,:,,::,,:::::,,,,:,::,::;i;;:;iiiii111i11");
    Serial.println(",,:,;i11iiii;;;;::,:,,,,:,:,,,,,,:,,,,:::,,,::;i:i:;iiiii111i11");
    Serial.println(":::,:i11iii;;;;;i::,:,:,t1i11i;:i1:::1,,,8,::::;f;:i;iii1111i11");
    Serial.println(":::,:iiiii;;::::;:;:::,,,tfCCCCCCCCCCLLLf,,::::0G;:iiiiii111i11");
    Serial.println(",,:.:ii1i;;;;::GGG00f;;1;::::;:,:;i:;::::::::1001::;i1ii1111i11");
    Serial.println(",,:,:;iii;;;::,ii1C0GGC@8GG00000000000000000800;::iiiiii111iiii");
    Serial.println(",,:,;;;;;;;;;;::::;;;;L0CGCCCCGGGGGGGGGGCGCG8G:,:;;iiiii111i;ii");
    Serial.println(",,:,;,:;;;;;::::::,:::100LLCCG0CCCCCCLtLC0GGGG:,:;i;;;;i11ti;1i");
    Serial.println(",:::i::;i;i1i:,;::,,::i80GGGG00CCCCCCCLfG0GGG0,,:;;i;;;iiii::;;");
    Serial.println("::::;:::;1;:,i,C:,,,,:10G0GG000CCCCCCLCLGCGGG8,:;;ii;iii1ti;;;,");
    Serial.println("::;:i;;;;;iii@@@LCLLLfGGGCCC0000CCCCCCC0GGGGG0:::;ii;;ii1tti;1:");
    Serial.println(":;;i1;iiii;:i@08@GGCfCCCCCLCG00008888800GCCCCCGG08;i;iii11iiii;");
    Serial.println("111if11i1ii;@888@0GC0GGGCCG000000000GCCGGG00080000;;;;ii11i;:i:");
    Serial.println("11itL111t11t1@@f08G000GCGGGGGGGCCCLCGG0G0888880G8L:;;i11t1;;;1i");
    Serial.println("ii1tt;i1111i;;;ii;f80GGGGGGGCCCCG00G000000880008C;;;;;::ii:::,,");
    Serial.println("ti1ftiiiiiiiiiiii;;::t00GGGGCGG00GCG00000000801;;iii;ii:;;:;i;:");
    Serial.println("1itfiii11i1111tt11i;::,00GG0000LL008088880;;;i1t1i1t1ttftfffftt");
    Serial.println("1itti;;1i1111111111i;;,0CffLC0i.,00GCLLGG,::;;;i;i111tttfttff1t");
    Serial.println(";,t1:;:::;;;:;:;:::,:0@@@@@@@@.,,,@@@@@@@@8:,:::::::;11ffttttii");
    Serial.println("..i:,:;:::;;iitfffft1@@@@@@CCCfLLC8@8@@@@@@Cft11tf:;i1fLLfftii;");
    Serial.println(",,,;;;;iiii1;;i;ttffftfttLCfftfLffCLCCCCCGGCCLCCCGG0tttfLLfffti");
    Serial.println(",,;;1;i1111ii11tttttfCCCLLCLCCLfCCCCCGCCLLLLLftt1LLCCGCLLffti;;");
    Serial.println("::;1i1itt111ttt11ttt11t1fttttti1111itt;t1fttttfffttLf1tti1fti11");
    Serial.println(";;;;iittttttf1fftttt11iiii;:,:;::,:i::::iiii1t1i;ii:;;;;::::::;");

    vTaskStartScheduler();
    Serial.println("Insufficient RAM");
    while(1);
}

