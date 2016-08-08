// gps.h

#ifndef _GPS_H_
#define _GPS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define PED		1
#define	AUTO	2
#define SEA		3
#define AIR		4

class gps {
public:
	gps();

	void begin(int baudrate = 115200);
	void configure(uint8_t mode = PED,
					  uint8_t datarate = 4,
					  int portrate = 115200,
					  bool gll = true, bool gsa = true, bool gsv = true, bool rmc = true, bool vtg = true);

private:

    /*
	*Settings Array contains the following settings:
    *[0]NavMode,
    *[1]DataRate1,
    *[2]DataRate2,
    *[3]PortRateByte1,
    *[4]PortRateByte2,
    *[5]PortRateByte3,
		*[6]NMEA GLL Sentence,
    *[7]NMEA GSA Sentence,
    *[8]NMEA GSV Sentence,
    *[9]NMEA RMC Sentence,
    *[10]NMEA VTG Sentence
     */

	//NavMode:
	const byte PedestrianMode = 0x03;
	const byte AutomotiveMode = 0x04;
	const byte SeaMode = 0x05;
	const byte AirborneMode = 0x06;

	//DataRate:
	const byte _1Hz[2] = { 0xE8, 0x03 };
	const byte _2Hz[2] = { 0xF4, 0x01 };
	const byte _3Hz3[2] = { 0x2C, 0x01 };
	const byte _4Hz[2] = { 0xFA, 0x00 };

	//PortRate:
	const byte Bps4800[3] = { 0xC0, 0x12, 0x00 };
	const byte Bps9600[3] = { 0x80, 0x25, 0x00 };
	const byte Bps19200[3] = { 0x00, 0x4B, 0x00 }; // **SOFTWARESERIAL LIMIT FOR ARDUINO UNO R3!**
	const byte Bps38400[3] = { 0x00, 0x96, 0x00 }; // **SOFTWARESERIAL LIMIT FOR ARDUINO MEGA 2560!**
	const byte Bps57600[3] = { 0x00, 0xE1, 0x00 };
	const byte Bps115200[3] = { 0x00, 0xC2, 0x01 };
	const byte Bps230400[3] = { 0x00, 0x84, 0x03 };

	//NMEA Messages:
	const byte NmeaOff = 0x00;
	const byte NmeaOn = 0x01;

	void setArray(uint8_t mode, uint8_t datarate, int portrate, bool gll, bool gsa, bool gsv, bool rmc, bool vtg);
	void calcChecksum(byte *checksumPayload, byte payloadSize);
	void sendUBX(byte *UBXmsg, byte msgLength);
	byte getUBX_ACK(byte *msgID);
	void printHex(uint8_t *data, uint8_t length);	// prints 8-bit data in hex
	void setBaud(byte baudSetting);

};

#endif
