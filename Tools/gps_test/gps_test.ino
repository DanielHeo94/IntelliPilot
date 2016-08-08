#include "gps.h"
#include "nmea.h"

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

gps mGps;
nmea parser;

void setup()
{
  Serial.begin(115200);
  mGps.begin(115200);
  mGps.configure();
  
  Serial.print("Testing TinyGPS library v. "); Serial.println(nmea::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
}

void loop()
{
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  print_int(parser.satellites(), nmea::GPS_INVALID_SATELLITES, 5);
  print_int(parser.hdop(), nmea::GPS_INVALID_HDOP, 5);
  parser.f_get_position(&flat, &flon, &age);
  print_float(flat, nmea::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, nmea::GPS_INVALID_F_ANGLE, 11, 6);
  print_int(age, nmea::GPS_INVALID_AGE, 5);
  print_date(parser);
  print_float(parser.f_altitude(), nmea::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(parser.f_course(), nmea::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(parser.f_speed_kmph(), nmea::GPS_INVALID_F_SPEED, 6, 2);
  print_str(parser.f_course() == nmea::GPS_INVALID_F_ANGLE ? "*** " : nmea::cardinal(parser.f_course()), 6);
  print_int(flat == nmea::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)nmea::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == nmea::GPS_INVALID_F_ANGLE ? nmea::GPS_INVALID_F_ANGLE : nmea::course_to(flat, flon, LONDON_LAT, LONDON_LON), nmea::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == nmea::GPS_INVALID_F_ANGLE ? "*** " : nmea::cardinal(nmea::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  parser.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(200);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial2.available())
      parser.encode(Serial2.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(nmea &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  parser.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == nmea::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, nmea::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
