#define DEBUG

#include <XBOXONE.h>
USBHost UsbH;
XBOXONE Xbox(&UsbH);
bool state = false;

struct _packet {
  int _X1, _Y1, _X2, _Y2, _L2, _R2;
  byte buttonStates1, buttonStates2;
};

_packet buffer;

void setup() {
  UsbH.Init();

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("start");
#endif

  pinMode(13, OUTPUT);

  initRadioModule();
}

void loop() {
  
  UsbH.Task();

  readData();
  
  if (Xbox.XboxOneConnected) {

    // Get all button presses into bytes buttonStates1 and buttonStates2
    //-----------------------------------------------------------------------------------------
    bitWrite(buffer.buttonStates1, 0, Xbox.getButtonPress(A));
    bitWrite(buffer.buttonStates1, 1, Xbox.getButtonPress(B));
    bitWrite(buffer.buttonStates1, 2, Xbox.getButtonPress(X));
    bitWrite(buffer.buttonStates1, 3, Xbox.getButtonPress(Y));
    bitWrite(buffer.buttonStates1, 4, Xbox.getButtonPress(UP));
    bitWrite(buffer.buttonStates1, 5, Xbox.getButtonPress(DOWN));
    bitWrite(buffer.buttonStates1, 6, Xbox.getButtonPress(LEFT));
    bitWrite(buffer.buttonStates1, 7, Xbox.getButtonPress(RIGHT));

    bitWrite(buffer.buttonStates2, 0, Xbox.getButtonPress(BACK));
    bitWrite(buffer.buttonStates2, 1, Xbox.getButtonPress(START));
    bitWrite(buffer.buttonStates2, 2, Xbox.getButtonPress(XBOX));
    bitWrite(buffer.buttonStates2, 3, Xbox.getButtonPress(L1));
    bitWrite(buffer.buttonStates2, 4, Xbox.getButtonPress(R1));
    bitWrite(buffer.buttonStates2, 5, Xbox.getButtonPress(L3));
    bitWrite(buffer.buttonStates2, 6, Xbox.getButtonPress(R3));
    bitWrite(buffer.buttonStates2, 7, 0);
    //-----------------------------------------------------------------------------------------

    // Get all analog values
    //-----------------------------------------------------------------------------------------
    buffer._X1 = map(Xbox.getAnalogHat(LeftHatX), -32768, 32767, -1023, 1023);
    buffer._Y1 = map(Xbox.getAnalogHat(LeftHatY), -32768, 32767, -1023, 1023);
    buffer._X2 = map(Xbox.getAnalogHat(RightHatX), -32768, 32767, -1023, 1023);
    buffer._Y2 = map(Xbox.getAnalogHat(RightHatY), -32768, 32767, -1023, 1023);
    buffer._L2 = Xbox.getButtonPress(L2);
    buffer._R2 = Xbox.getButtonPress(R2);
    //-----------------------------------------------------------------------------------------




    // Stream data to NRF module
    //-----------------------------------------------------------------------------------------
    sendData(buffer);
    //-----------------------------------------------------------------------------------------

    // Indicate status on pin 13 LED
    //-----------------------------------------------------------------------------------------
    state = !state;
    digitalWrite(13, state);
    //-----------------------------------------------------------------------------------------

    // Print debug messages
    //-----------------------------------------------------------------------------------------
    debugMsg();
    //-----------------------------------------------------------------------------------------

    // Delay to stabilize (can decrease upto a value of 5 milliseconds)
    //-----------------------------------------------------------------------------------------
    delay(20);
    //-----------------------------------------------------------------------------------------
  }
  digitalWrite(13, LOW);
}


void debugMsg() {
#ifdef DEBUG
  Serial.printf("X1:%d,Y1:%d,X2:%d,Y2:%d,L2:%d,R2:%d,B:", buffer._X1, buffer._Y1, buffer._X2, buffer._Y2, buffer._L2, buffer._R2);
  Serial.print(bitRead(buffer.buttonStates2, 7));
  Serial.print(bitRead(buffer.buttonStates2, 6));
  Serial.print(bitRead(buffer.buttonStates2, 5));
  Serial.print(bitRead(buffer.buttonStates2, 4));
  Serial.print(bitRead(buffer.buttonStates2, 3));
  Serial.print(bitRead(buffer.buttonStates2, 2));
  Serial.print(bitRead(buffer.buttonStates2, 1));
  Serial.print(bitRead(buffer.buttonStates2, 0));
  Serial.print(bitRead(buffer.buttonStates1, 7));
  Serial.print(bitRead(buffer.buttonStates1, 6));
  Serial.print(bitRead(buffer.buttonStates1, 5));
  Serial.print(bitRead(buffer.buttonStates1, 4));
  Serial.print(bitRead(buffer.buttonStates1, 3));
  Serial.print(bitRead(buffer.buttonStates1, 2));
  Serial.print(bitRead(buffer.buttonStates1, 1));
  Serial.println(bitRead(buffer.buttonStates1, 0));
#endif
}