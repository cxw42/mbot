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

// define for USB; comment out for 2.4GHz
#define TTYUSB

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
}

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
#define MAXBYTES (64)

void loop() {
  char bytes[MAXBYTES];

  swrite('>');
  Serial.flush();

  int got = Serial.readBytesUntil('\n', bytes, MAXBYTES);
  
  // Echo if we got data
  if (got > 0) {
    swrite('=');
    for(int i=0; i<got; ++i) {
      swrite(bytes[i]);
    }

    swrite('\n');
    Serial.flush();
  }
  return; // XXX

#if 0
  if(thisByte == FIRST_BYTE) {
    // prints title with ending line break
    sprintln("ASCII Table ~ Character Map");
  }
#endif

  // prints value unaltered, i.e. the raw binary version of the byte.
  // The Serial Monitor interprets all bytes as ASCII, so 33, the first number,
  // will show up as '!'
  swrite(thisByte);


#if 0
  sprint(", dec: ");
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  sprint(thisByte);
  // But you can declare the modifier for decimal if you want to.
  // this also works if you uncomment it:

  // Serial.print(thisByte, DEC);


  sprint(", hex: ");
  // prints value as string in hexadecimal (base 16):
  sprint(thisByte, HEX);

  sprint(", oct: ");
  // prints value as string in octal (base 8);
  sprint(thisByte, OCT);

  sprint(", bin: ");
  // prints value as string in binary (base 2) also prints ending line break:
  sprintln(thisByte, BIN);
#endif // 0

  // if printed last visible character '~' or 126, stop:
  if (thisByte == LAST_BYTE) {    // you could also use if (thisByte == '~') {
    thisByte = FIRST_BYTE;
  } else {
    // go on to the next character
    thisByte++;
  }
}
