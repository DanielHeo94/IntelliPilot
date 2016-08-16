#ifndef _PARSER_H_
#define _PARSER_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <limits.h>

#define _GPS_VERSION "0.92" // software version of this library
#define _GPS_MPH_PER_KNOT 1.15077945
#define _GPS_MPS_PER_KNOT 0.51444444
#define _GPS_KMPH_PER_KNOT 1.852
#define _GPS_MILES_PER_METER 0.00062137112
#define _GPS_KM_PER_METER 0.001
#define _GPS_FEET_PER_METER 3.2808399
#define _GPS_MAX_FIELD_SIZE 15

struct RawDegrees
{
        uint16_t deg;
        uint32_t billionths;
        bool negative;
public:
        RawDegrees() : deg(0), billionths(0), negative(false)
        {
        }
};

struct gpsLocation
{
        friend class gps;
public:
        bool isValid() const {
                return valid;
        }
        bool isUpdated() const {
                return updated;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }
        const RawDegrees &rawLat()     {
                updated = false; return rawLatData;
        }
        const RawDegrees &rawLng()     {
                updated = false; return rawLngData;
        }
        double lat();
        double lng();

        gpsLocation() : valid(false), updated(false)
        {
        }

private:
        bool valid, updated;
        RawDegrees rawLatData, rawLngData, rawNewLatData, rawNewLngData;
        uint32_t lastCommitTime;
        void commit();
        void setLatitude(const char *term);
        void setLongitude(const char *term);
};

struct gpsDate
{
        friend class gps;
public:
        bool isValid() const {
                return valid;
        }
        bool isUpdated() const {
                return updated;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }

        uint32_t value()           {
                updated = false; return date;
        }
        uint16_t year();
        uint8_t month();
        uint8_t day();

        gpsDate() : valid(false), updated(false), date(0)
        {
        }

private:
        bool valid, updated;
        uint32_t date, newDate;
        uint32_t lastCommitTime;
        void commit();
        void setDate(const char *term);
};

struct gpsTime
{
        friend class gps;
public:
        bool isValid() const {
                return valid;
        }
        bool isUpdated() const {
                return updated;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }

        uint32_t value()           {
                updated = false; return time;
        }
        uint8_t hour();
        uint8_t minute();
        uint8_t second();
        uint8_t centisecond();

        gpsTime() : valid(false), updated(false), time(0)
        {
        }

private:
        bool valid, updated;
        uint32_t time, newTime;
        uint32_t lastCommitTime;
        void commit();
        void setTime(const char *term);
};

struct gpsDecimal
{
        friend class gps;
public:
        bool isValid() const {
                return valid;
        }
        bool isUpdated() const {
                return updated;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }
        int32_t value()         {
                updated = false; return val;
        }

        gpsDecimal() : valid(false), updated(false), val(0)
        {
        }

private:
        bool valid, updated;
        uint32_t lastCommitTime;
        int32_t val, newval;
        void commit();
        void set(const char *term);
};

struct gpsInteger
{
        friend class gps;
public:
        bool isValid() const {
                return valid;
        }
        bool isUpdated() const {
                return updated;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }
        uint32_t value()        {
                updated = false; return val;
        }

        gpsInteger() : valid(false), updated(false), val(0)
        {
        }

private:
        bool valid, updated;
        uint32_t lastCommitTime;
        uint32_t val, newval;
        void commit();
        void set(const char *term);
};

struct gpsSpeed : gpsDecimal
{
        double knots()    {
                return value() / 100.0;
        }
        double mph()      {
                return _GPS_MPH_PER_KNOT * value() / 100.0;
        }
        double mps()      {
                return _GPS_MPS_PER_KNOT * value() / 100.0;
        }
        double kmph()     {
                return _GPS_KMPH_PER_KNOT * value() / 100.0;
        }
};

struct gpsCourse : public gpsDecimal
{
        double deg()      {
                return value() / 100.0;
        }
};

struct gpsAltitude : gpsDecimal
{
        double meters()       {
                return value() / 100.0;
        }
        double miles()        {
                return _GPS_MILES_PER_METER * value() / 100.0;
        }
        double kilometers()   {
                return _GPS_KM_PER_METER * value() / 100.0;
        }
        double feet()         {
                return _GPS_FEET_PER_METER * value() / 100.0;
        }
};

class gps;
class gpsCustom
{
public:
        gpsCustom() {
        };
        gpsCustom(gps &gps, const char *sentenceName, int termNumber);
        void begin(gps &gps, const char *_sentenceName, int _termNumber);

        bool isUpdated() const {
                return updated;
        }
        bool isValid() const {
                return valid;
        }
        uint32_t age() const {
                return valid ? millis() - lastCommitTime : (uint32_t)ULONG_MAX;
        }
        const char *value()     {
                updated = false; return buffer;
        }

private:
        void commit();
        void set(const char *term);

        char stagingBuffer[_GPS_MAX_FIELD_SIZE + 1];
        char buffer[_GPS_MAX_FIELD_SIZE + 1];
        unsigned long lastCommitTime;
        bool valid, updated;
        const char *sentenceName;
        int termNumber;
        friend class gps;
        gpsCustom *next;
};

class gps
{
public:
        gps();
        bool encode(char c); // process one character received from GPS
        gps &operator << (char c) {encode(c); return *this; }

        gpsLocation location;
        gpsDate date;
        gpsTime time;
        gpsSpeed speed;
        gpsCourse course;
        gpsAltitude altitude;
        gpsInteger satellites;
        gpsDecimal hdop;

        static const char *libraryVersion() {
                return _GPS_VERSION;
        }

        static double distanceBetween(double lat1, double long1, double lat2, double long2);
        static double courseTo(double lat1, double long1, double lat2, double long2);
        static const char *cardinal(double course);

        static int32_t parseDecimal(const char *term);
        static void parseDegrees(const char *term, RawDegrees &deg);

        uint32_t charsProcessed()   const {
                return encodedCharCount;
        }
        uint32_t sentencesWithFix() const {
                return sentencesWithFixCount;
        }
        uint32_t failedChecksum()   const {
                return failedChecksumCount;
        }
        uint32_t passedChecksum()   const {
                return passedChecksumCount;
        }

private:
        enum {GPS_SENTENCE_GPGGA, GPS_SENTENCE_GPRMC, GPS_SENTENCE_OTHER};

        // parsing state variables
        uint8_t parity;
        bool isChecksumTerm;
        char term[_GPS_MAX_FIELD_SIZE];
        uint8_t curSentenceType;
        uint8_t curTermNumber;
        uint8_t curTermOffset;
        bool sentenceHasFix;

        // custom element support
        friend class gpsCustom;
        gpsCustom *customElts;
        gpsCustom *customCandidates;
        void insertCustom(gpsCustom *pElt, const char *sentenceName, int index);

        // statistics
        uint32_t encodedCharCount;
        uint32_t sentencesWithFixCount;
        uint32_t failedChecksumCount;
        uint32_t passedChecksumCount;

        // internal utilities
        int fromHex(char a);
        bool endOfTermHandler();
};

#endif // def(_gps_H_)
