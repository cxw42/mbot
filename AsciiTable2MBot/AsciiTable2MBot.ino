// No longer ascii table!

// define for USB; comment out for 2.4GHz
#define TTYUSB

// Using the mbot fork at
// <https://github.com/nbourre/Makeblock-Libraries>, installed via the
// Library Manager
#include <MeMCore.h>

MeDCMotor MotorL(M1);
MeDCMotor MotorR(M2);

bool buttonWasPressed = false;

// Custom serial-write routine.
// 115200 is ~8.7us per byte.  Cut the rate to give the wireless
// adapter time to keep up.

void swrite(int i)
{
  delayMicroseconds(20);
  Serial.write(i);
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

  swrite('>');
  Serial.flush();

  MotorL.run(0);
  MotorR.run(0);
}

// Buffer
#define MAXBYTES (64)
char bytes[MAXBYTES];
int bytesRead = 0;
bool stringReady = false;

// check the buffer for a motor command.
// Format is `Mlr`, where `l` is left-motor speed and `r` is right-motor
// speed.  Each is one ascii char, with `m` == 0.  So `Mmm` = stop.
void CheckMotorCommand()
{
  if(bytesRead < 3) {
    return;
  }

  if(bytes[0] != 'M') {
    return;
  }

  int lspeed = (int)bytes[1] - 'm';
  int rspeed = (int)bytes[2] - 'm';

  MotorL.run(lspeed);
  MotorR.run(rspeed);
}

// Print the buffer and reset it
void PrintAndReset()
{
    swrite('=');
    for(int i=0; i<bytesRead; ++i) {
      swrite(bytes[i]);
    }

    swrite('\n');
    swrite('>');
    Serial.flush();

    bytesRead = 0;
    stringReady = false;

    // LED
    pinMode(LED_BUILTIN, OUTPUT);

}

bool motorRunning = false;

void checkButton()
{
  const bool buttonIsPressed = !(analogRead(7) > 100);  // incantation from factory firmware
  const bool shouldToggleMotor = buttonIsPressed && !buttonWasPressed;
  buttonWasPressed = buttonIsPressed;

  if(!shouldToggleMotor) {
    return;
  }

  motorRunning = !motorRunning;
  const int speed = motorRunning ? 100 : 0;
  MotorL.run(-speed);
  MotorR.run(speed);
}

void loop()
{
  digitalWrite(LED_BUILTIN, stringReady ? HIGH : LOW);

  if(stringReady) {
    CheckMotorCommand();
    PrintAndReset();
  }

  checkButton();
}

void serialEvent()
{
  while(Serial.available()) {
    auto ch = Serial.read();
    if((ch < 0) || (ch == '\n') || (ch == '\r')) {
      stringReady = true;
      return;
    }
    bytes[bytesRead++] = ch & 0x7f;
    if(bytesRead == MAXBYTES) {
      stringReady = true;
      return;
    }
  }
}
