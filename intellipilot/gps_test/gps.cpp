// 
// 
// 

#include "gps.h"

byte settingsArray[11] = { 0x00, };

boolean gpsStatus[] = { false, false, false, false, false, false, false };
unsigned long start;

gps::gps() {}

void gps::begin(int baudrate) {
	Serial2.begin(baudrate);
}

void gps::configure(uint8_t mode, uint8_t datarate, int portrate, bool gll, bool gsa, bool gsv, bool rmc, bool vtg) {
	byte gpsSetSuccess = 0;
	Serial.println("Configuring u-Blox GPS initial state...");

	gps::setArray(mode, datarate, portrate, gll, gsa, gsv, rmc, vtg);

	//Generate the configuration string for Navigation Mode
	byte setNav[] = { 0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, settingsArray[0], 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	gps::calcChecksum(&setNav[2], sizeof(setNav) - 4);

	//Generate the configuration string for Data Rate
	byte setDataRate[] = { 0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, settingsArray[1], settingsArray[2], 0x01, 0x00, 0x01, 0x00, 0x00, 0x00 };
	gps::calcChecksum(&setDataRate[2], sizeof(setDataRate) - 4);

	//Generate the configuration string for Baud Rate
	byte setPortRate[] = { 0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, settingsArray[3], settingsArray[4], settingsArray[5], 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	gps::calcChecksum(&setPortRate[2], sizeof(setPortRate) - 4);

	byte setGLL[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B };
	byte setGSA[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32 };
	byte setGSV[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39 };
	byte setRMC[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40 };
	byte setVTG[] = { 0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46 };

	delay(2500);

	while (gpsSetSuccess < 3)
	{
		Serial.print("Setting Navigation Mode... ");
		gps::sendUBX(&setNav[0], sizeof(setNav));  //Send UBX Packet
		gpsSetSuccess += gps::getUBX_ACK(&setNav[2]); //Passes Class ID and Message ID to the ACK Receive function
		if (gpsSetSuccess == 5) {
			gpsSetSuccess -= 4;
			gps::setBaud(settingsArray[4]);
			delay(1500);
			byte lowerPortRate[] = { 0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA2, 0xB5 };
			gps::sendUBX(lowerPortRate, sizeof(lowerPortRate));
			Serial2.begin(9600);
			delay(2000);
		}
		if (gpsSetSuccess == 6) gpsSetSuccess -= 4;
		if (gpsSetSuccess == 10) gpsStatus[0] = true;
	}
	if (gpsSetSuccess == 3) Serial.println("Navigation mode configuration failed.");
	gpsSetSuccess = 0;
	while (gpsSetSuccess < 3) {
		Serial.print("Setting Data Update Rate... ");
		gps::sendUBX(&setDataRate[0], sizeof(setDataRate));  //Send UBX Packet
		gpsSetSuccess += gps::getUBX_ACK(&setDataRate[2]); //Passes Class ID and Message ID to the ACK Receive function
		if (gpsSetSuccess == 10) gpsStatus[1] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("Data update mode configuration failed.");
	gpsSetSuccess = 0;


	while (gpsSetSuccess < 3 && settingsArray[6] == 0x00) {
		Serial.print("Deactivating NMEA GLL Messages ");
		gps::sendUBX(setGLL, sizeof(setGLL));
		gpsSetSuccess += gps::getUBX_ACK(&setGLL[2]);
		if (gpsSetSuccess == 10) gpsStatus[2] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("NMEA GLL Message Deactivation Failed!");
	gpsSetSuccess = 0;

	while (gpsSetSuccess < 3 && settingsArray[7] == 0x00) {
		Serial.print("Deactivating NMEA GSA Messages ");
		gps::sendUBX(setGSA, sizeof(setGSA));
		gpsSetSuccess += gps::getUBX_ACK(&setGSA[2]);
		if (gpsSetSuccess == 10) gpsStatus[3] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("NMEA GSA Message Deactivation Failed!");
	gpsSetSuccess = 0;

	while (gpsSetSuccess < 3 && settingsArray[8] == 0x00) {
		Serial.print("Deactivating NMEA GSV Messages ");
		gps::sendUBX(setGSV, sizeof(setGSV));
		gpsSetSuccess += gps::getUBX_ACK(&setGSV[2]);
		if (gpsSetSuccess == 10) gpsStatus[4] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("NMEA GSV Message Deactivation Failed!");
	gpsSetSuccess = 0;

	while (gpsSetSuccess < 3 && settingsArray[9] == 0x00) {
		Serial.print("Deactivating NMEA RMC Messages ");
		gps::sendUBX(setRMC, sizeof(setRMC));
		gpsSetSuccess += gps::getUBX_ACK(&setRMC[2]);
		if (gpsSetSuccess == 10) gpsStatus[5] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("NMEA RMC Message Deactivation Failed!");
	gpsSetSuccess = 0;

	while (gpsSetSuccess < 3 && settingsArray[10] == 0x00) {
		Serial.print("Deactivating NMEA VTG Messages ");
		gps::sendUBX(setVTG, sizeof(setVTG));
		gpsSetSuccess += gps::getUBX_ACK(&setVTG[2]);
		if (gpsSetSuccess == 10) gpsStatus[6] = true;
		if (gpsSetSuccess == 5 | gpsSetSuccess == 6) gpsSetSuccess -= 4;
	}
	if (gpsSetSuccess == 3) Serial.println("NMEA VTG Message Deactivation Failed!");

	gpsSetSuccess = 0;
	if (settingsArray[4] != 0x25) {
		Serial.print("Setting Port Baud Rate... ");
		gps::sendUBX(&setPortRate[0], sizeof(setPortRate));
		gps::setBaud(settingsArray[4]);
		Serial.println("Success!");
		delay(500);
	}
}

void gps::calcChecksum(byte *checksumPayload, byte payloadSize) {
	byte CK_A = 0, CK_B = 0;
	for (int i = 0; i < payloadSize; i++) {
		CK_A = CK_A + *checksumPayload;
		CK_B = CK_B + CK_A;
		checksumPayload++;
	}
	*checksumPayload = CK_A;
	checksumPayload++;
	*checksumPayload = CK_B;
}

void gps::setArray(uint8_t mode, uint8_t datarate, int portrate, bool gll, bool gsa, bool gsv, bool rmc, bool vtg) {
	switch (mode) {
	case PED:
		settingsArray[0] = PedestrianMode;
	case AUTO:
		settingsArray[0] = AutomotiveMode;
	case SEA:
		settingsArray[0] = SeaMode;
	case AIR:
		settingsArray[0] = AirborneMode;
	default:
		settingsArray[0] = PedestrianMode;
		break;
	}

	switch (datarate)
	{
	case 1:
		settingsArray[1] = _1Hz[0];
		settingsArray[2] = _1Hz[1];
	case 2:
		settingsArray[1] = _2Hz[0];
		settingsArray[2] = _2Hz[1];
	case 3:
		settingsArray[1] = _3Hz3[0];
		settingsArray[2] = _3Hz3[1];
	case 4:
		settingsArray[1] = _4Hz[0];
		settingsArray[2] = _4Hz[1];
	default:
		settingsArray[1] = _4Hz[0];
		settingsArray[2] = _4Hz[1];
		break;
	}

	switch (portrate)
	{
	case 4800:
		settingsArray[3] = Bps4800[0];
		settingsArray[4] = Bps4800[1];
		settingsArray[5] = Bps4800[2];
	case 9600:
		settingsArray[3] = Bps9600[0];
		settingsArray[4] = Bps9600[1];
		settingsArray[5] = Bps9600[2];
	case 19200:
		settingsArray[3] = Bps19200[0];
		settingsArray[4] = Bps19200[1];
		settingsArray[5] = Bps19200[2];
	case 38400:
		settingsArray[3] = Bps38400[0];
		settingsArray[4] = Bps38400[1];
		settingsArray[5] = Bps38400[2];
	case 57600:
		settingsArray[3] = Bps57600[0];
		settingsArray[4] = Bps57600[1];
		settingsArray[5] = Bps57600[2];
	case 115200:
		settingsArray[3] = Bps115200[0];
		settingsArray[4] = Bps115200[1];
		settingsArray[5] = Bps115200[2];
	case 230400:
		settingsArray[3] = Bps230400[0];
		settingsArray[4] = Bps230400[1];
		settingsArray[5] = Bps230400[2];
	default:
		settingsArray[3] = Bps115200[0];
		settingsArray[4] = Bps115200[1];
		settingsArray[5] = Bps115200[2];
		break;
	}

	if (gll) settingsArray[6] = NmeaOn; else settingsArray[6] = NmeaOff;
	if (gsa) settingsArray[7] = NmeaOn; else settingsArray[7] = NmeaOff;
	if (gsv) settingsArray[8] = NmeaOn; else settingsArray[8] = NmeaOff;
	if (rmc) settingsArray[9] = NmeaOn; else settingsArray[9] = NmeaOff;
	if (vtg) settingsArray[10] = NmeaOn; else settingsArray[10] = NmeaOff;


}

void gps::sendUBX(byte *UBXmsg, byte msgLength) {
	for (int i = 0; i < msgLength; i++) {
		Serial2.write(UBXmsg[i]);
		Serial2.flush();
	}
	Serial2.println();
	Serial2.flush();
}

byte gps::getUBX_ACK(byte *msgID) {
	byte CK_A = 0, CK_B = 0;
	byte incoming_char;
	boolean headerReceived = false;
	unsigned long ackWait = millis();
	byte ackPacket[10] = { 0xB5, 0x62, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	int i = 0;
	while (1) {
		if (Serial2.available()) {
			incoming_char = Serial2.read();
			if (incoming_char == ackPacket[i]) {
				i++;
			}
			else if (i > 2) {
				ackPacket[i] = incoming_char;
				i++;
			}
		}
		if (i > 9) break;
		if ((millis() - ackWait) > 1500) {
			Serial.println("ACK Timeout");
			return 5;
		}
		if (i == 4 && ackPacket[3] == 0x00) {
			Serial.println("NAK Received");
			return 1;
		}
	}

	for (i = 2; i < 8; i++) {
		CK_A = CK_A + ackPacket[i];
		CK_B = CK_B + CK_A;
	}
	if (msgID[0] == ackPacket[6] && msgID[1] == ackPacket[7] && CK_A == ackPacket[8] && CK_B == ackPacket[9]) {
		Serial.println("Success!");
		Serial.print("ACK Received! ");
		gps::printHex(ackPacket, sizeof(ackPacket));
		return 10;
	}
	else {
		Serial.print("ACK Checksum Failure: ");
		gps::printHex(ackPacket, sizeof(ackPacket));
		delay(1000);
		return 1;
	}
}

void gps::printHex(uint8_t *data, uint8_t length) {
	char tmp[length * 2 + 1];
	byte first;
	int j = 0;
	for (byte i = 0; i < length; i++)
	{
		first = (data[i] >> 4) | 48;
		if (first > 57) tmp[j] = first + (byte)7;
		else tmp[j] = first;
		j++;

		first = (data[i] & 0x0F) | 48;
		if (first > 57) tmp[j] = first + (byte)7;
		else tmp[j] = first;
		j++;
	}
	tmp[length * 2] = 0;
	for (byte i = 0, j = 0; i < sizeof(tmp); i++) {
		Serial.print(tmp[i]);
		if (j == 1) {
			Serial.print(" ");
			j = 0;
		}
		else j++;
	}
	Serial.println();
}

void gps::setBaud(byte baudSetting) {
	if (baudSetting == 0x12) Serial2.begin(4800);
	if (baudSetting == 0x4B) Serial2.begin(19200);
	if (baudSetting == 0x96) Serial2.begin(38400);
	if (baudSetting == 0xE1) Serial2.begin(57600);
	if (baudSetting == 0xC2) Serial2.begin(115200);
	if (baudSetting == 0x84) Serial2.begin(230400);
}