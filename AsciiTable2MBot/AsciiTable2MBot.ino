/*
  ASCII table

  Prints out byte values in all possible formats:
  - as raw binary values
  - as ASCII-encoded decimal, hex, octal, and binary values

  For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII

  The circuit: No external hardware needed.

  created 2006
  by Nicholas Zambetti <http://www.zambetti.com>
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/ASCIITable
*/

// Don't use the system-provided serialEventRun()
void serialEventRun() {}

// define for USB; comment out for 2.4GHz
#define TTYUSB

// first visible ASCIIcharacter '!' is number 33:
#define FIRST_BYTE (33)
 
// character '~' or 126
#define LAST_BYTE (126) 
 
int thisByte = FIRST_BYTE;
// you can also write ASCII characters in single quotes.
// for example, '!' is the same as 33, so you could also use this:
// int thisByte = '!';

// 115200 is ~8.7us per byte.  Cut the rate to give the wireless
// adapter time to keep up.

void swrite(int i)
{
  delayMicroseconds(20);
  Serial.write(i);
}

void sprint(char *s, int base=DEC)
{
  while(char c = *s++) {
    swrite(c);
  }
}

void sprintln(char *s, int base=DEC)
{
  sprint(s, base);
  swrite(0x10);
}

void setup() {
#ifdef TTYUSB
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#else
  Serial.begin(115200, SERIAL_8N2);
#endif

  Serial.setTimeout(2000);
  swrite('>');
  Serial.flush();
}

#define MAXBYTES (64)
char bytes[MAXBYTES];
int readBytes = 0;
bool stringReady = false;

// Print the buffer and reset it
void PrintAndReset()
{
    swrite('=');
    for(int i=0; i<readBytes; ++i) {
      swrite(bytes[i]);
    }

    swrite('\n');
    swrite('>');
    Serial.flush();

    readBytes = 0;
    stringReady = false;
}

void loop() 
{
  if(stringReady) {
    PrintAndReset();
  }
}

void serialEvent()
{
  while(Serial.available()) {
    auto ch = Serial.read();
    if((ch < 0) || (ch == '\n')) {
      stringReady = true;
      return;
    }
    bytes[readBytes++] = ch & 0x7f;
    if(readBytes == MAXBYTES) {
      stringReady = true;
      return;
    }
  }

}
