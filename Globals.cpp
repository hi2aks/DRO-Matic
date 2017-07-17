/*
*  DROMatic.ino
*  DROMatic OS Globals
*  Devin R. Olsen - July 4th, 2017
*  devin@devinrolsen.com
*/

#include "Globals.h"
#include "Pumps.h"
#include "Regimens.h"
#include "Menus.h"

float flowInRate, flowOutRate, flowMeterConfig[2], minPH, maxPH;
boolean irrigationFlag = false;

byte currentTimerSessions[4], 
currentRegimen, maxRegimens, 
drainTime, 
topOffConcentrate,
topOffAmount,
topOffDelay,
cropStatus, 
feedingType,
lastFeedingWeek,
lastFeedingDay,
pumpDelay,
timerStartHours[4],
timerEndHours[4];

volatile unsigned long pulseInFlowCount, pulseOutFlowCount;

int Key, minPPM, maxPPM, rsvrVol, pumpCalibration;
double currentRsvrVol = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
DS3231  rtc(SDA, SCL);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMOFLEDS, LEDPIN, NEO_GRB + NEO_KHZ800);

//Tentical Sheild
int channel_ids[] = { 111, 112, 113, 114 };
char sensordata[30];                  // A 30 byte character array to hold incoming data from the sensors
byte sensor_bytes_received = 0;       // We need to know how many characters bytes have been received
byte code = 0;                        // used to hold the I2C response code.
byte in_char = 0;                     // used as a 1 byte buffer to store in bound bytes from the I2C Circuit.


File tmpFile;
String nameArry[15], tmpDisplay[5]; //tmpDisplay = suffix, hour, min, day
int tmpInts[6];
float tmpFloats[2];

const char blank[2] PROGMEM = " ";
const char a[2] PROGMEM = "A";
const char b[2] PROGMEM = "B";
const char c[2] PROGMEM = "C";
const char d[2] PROGMEM = "D";
const char e[2] PROGMEM = "E";
const char f[2] PROGMEM = "F";
const char g[2] PROGMEM = "G";
const char h[2] PROGMEM = "H";
const char i[2] PROGMEM = "I";
const char j[2] PROGMEM = "J";
const char k[2] PROGMEM = "K";
const char l[2] PROGMEM = "L";
const char m[2] PROGMEM = "M";
const char n[2] PROGMEM = "N";
const char o[2] PROGMEM = "O";
const char p[2] PROGMEM = "P";
const char q[2] PROGMEM = "Q";
const char r[2] PROGMEM = "R";
const char s[2] PROGMEM = "S";
const char t[2] PROGMEM = "T";
const char u[2] PROGMEM = "U";
const char v[2] PROGMEM = "V";
const char w[2] PROGMEM = "W";
const char x[2] PROGMEM = "X";
const char y[2] PROGMEM = "Y";
const char z[2] PROGMEM = "Z";
const char zero[2] PROGMEM = "0";
const char one[2] PROGMEM = "1";
const char two[2] PROGMEM = "2";
const char three[2] PROGMEM = "3";
const char four[2] PROGMEM = "4";
const char five[2] PROGMEM = "5";
const char six[2] PROGMEM = "6";
const char seven[2] PROGMEM = "7";
const char eight[2] PROGMEM = "8";
const char nine[2] PROGMEM = "9";

const char* const alphabet[37] PROGMEM = {
	blank, a, b, c, d, e, f, g, h, i, j, k, l, m,
	n, o, p, q, r, s, t, u, v, w, x, y, z, zero,
	one, two, three, four, five, six, seven, eight, nine
};


const char _sys[4] PROGMEM = "SYS";
const char _crop[5] PROGMEM = "CROP";
const char _irri[5] PROGMEM = "IRRI";
const char _pump[6] PROGMEM = "PUMPS";
const char _timer[7] PROGMEM = "TIMERS";
const char _datetime[9] PROGMEM = "DATETIME";
const char _EC[3] PROGMEM = "EC";
const char _PH[3] PROGMEM = "PH";
const char _open[5] PROGMEM = "OPEN";
const char _new[4] PROGMEM = "NEW";
const char _delete[7] PROGMEM = "DELETE";
const char _reset[7] PROGMEM = "RESET";
const char _status[7] PROGMEM = "STATUS";
const char _amt[7] PROGMEM = "AMOUNT";
const char _rsvrVol[8] PROGMEM = "RSVRVOL";
const char _pumpCal[8] PROGMEM = "PUMPCAL";
const char _topOffCcnt[8] PROGMEM = "TPFCCNT";
const char _topOffAmnt[8] PROGMEM = "TPFAMT";
const char _topOffDly[8] PROGMEM = "TPFDLY";
const char _drainTime[8] PROGMEM = "DRNTIME";
const char _doses[6] PROGMEM = "DOSES";
const char _prime[6] PROGMEM = "PRIME";
const char _startend[9] PROGMEM = "STARTEND";
const char _weeks[6] PROGMEM = "WEEKS";
const char _flowcal[8] PROGMEM = "FLOWCAL";
const char _manFlush[9] PROGMEM = "MANFLUSH";
const char _ECCal[6] PROGMEM = "ECCAL";
const char _PHCal[6] PROGMEM = "PHCAL";
const char _delay[6] PROGMEM = "DELAY";


//Consolidated Repeating Displays Words
const char System[7] PROGMEM = "SYSTEM";
const char Settings[9] PROGMEM = "SETTINGS";
const char Pump[6] PROGMEM = "PUMP";
const char NumberOf[10] PROGMEM = "NUMBER OF";
const char Configuration[14] PROGMEM = "CONFIGURATION";
const char Concent[12] PROGMEM = "CONCENTRATE";
const char Calib[10] PROGMEM = "CALIBRATE";
const char Irrigation[11] PROGMEM = "IRRIGATION";
const char Reservoir[10] PROGMEM = "RESERVOIR";
const char Timer[6] PROGMEM = "TIMER";
const char Times[6] PROGMEM = "TIMES";
const char TopOff[8] PROGMEM = "TOP OFF";
const char RegimensML[14] PROGMEM = "REGIMENS (ml)";
const char RegimensDoses[14] PROGMEM = "REGIMEN DOSES";
const char Weeks[6] PROGMEM = "WEEKS";
const char Solution[9] PROGMEM = "SOLUTION";
const char Amount[7] PROGMEM = "AMOUNT";
const char Flush[9] PROGMEM = "FLUSHING";

//Direct Translations
const char DateTime[12] PROGMEM = "DATE & TIME";
const char EC[13] PROGMEM = "EC/PPM RANGE";
const char ECS[13] PROGMEM = "EC SENSORS";
const char PH[9] PROGMEM = "PH RANGE";
const char PHS[11] PROGMEM = "PH SENSORS";
const char Open[5] PROGMEM = "OPEN";
const char New[4] PROGMEM = "NEW";
const char Delete[7] PROGMEM = "DELETE";
const char Reset[6] PROGMEM = "RESET";
const char DrainLength[11] PROGMEM = "DRAIN TIME";
const char StartEnd[11] PROGMEM = "START END";
const char PrimePump[14] PROGMEM = "PRIME PUMP";
const char VolumeConfig[14] PROGMEM = "VOLUME CONFIG";
const char PumpDose[16] PROGMEM = "PUMP DOSEING";
const char DelayConfig[16] PROGMEM = "DELAY CONFIGURE";
const char FlowMeters[12] PROGMEM = "FLOW METERS";
const char Manual[14] PROGMEM = "MANUAL SYSTEM";



const char* const screenNames[28][3] PROGMEM = {
	{ _sys, System, Settings },
	{ _pump, Pump, Settings },
	{ _crop, _crop, Settings },
	{ _irri, Irrigation, Settings },
	{ _timer, Timer, Settings },
	{ _datetime, DateTime, Configuration },
	{ _EC, EC, Configuration },
	{ _PH, PH, Configuration },
	{ _ECCal, ECS, Calib },
	{ _PHCal, PHS, Calib },
	{ _open, Open, _crop },
	{ _new, New, _crop },
	{ _delete, Delete, _crop },
	{ _reset, Reset, _crop },
	{ _status, _crop, _status },
	{ _rsvrVol, Reservoir, VolumeConfig },
	{ _topOffCcnt, TopOff, Concent },
	{ _topOffAmnt, TopOff, Amount },
	{ _drainTime, DrainLength, Configuration },
	{ _doses, NumberOf, RegimensDoses },
	{ _weeks, NumberOf, Weeks },
	{ _amt, RegimensML, Configuration },
	{ _delay, PumpDose, DelayConfig },
	{ _pumpCal, Pump, Calib },
	{ _flowcal, FlowMeters, Calib },
	{ _manFlush, Manual, Flush },
	{ _prime, PrimePump, Solution },
	{ _startend, StartEnd, Times }
};

const char jan[4] PROGMEM = "Jan";
const char feb[4] PROGMEM = "Feb";
const char mar[4] PROGMEM = "Mar";
const char apl[4] PROGMEM = "Apl";
const char may[4] PROGMEM = "May";
const char jun[4] PROGMEM = "Jun";
const char jul[4] PROGMEM = "Jul";
const char aug[4] PROGMEM = "Aug";
const char sep[4] PROGMEM = "Sep";
const char oct[4] PROGMEM = "Oct";
const char nov[4] PROGMEM = "Nov";
const char dec[4] PROGMEM = "Dec";

const char* const months[12] PROGMEM = { jan, feb, mar, apl, may, jun, jul, aug, sep, oct, nov, dec };

const char none[5] PROGMEM = "None";
const char hourly[7] PROGMEM = "Hourly";
const char daily[6] PROGMEM = "Daily";
const char weekly[7] PROGMEM = "Weekly";
const char monthly[8] PROGMEM = "Monthly";
const char yearly[7] PROGMEM = "Yearly";
