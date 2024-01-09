// #define DEBUG
#include <Servo.h>
Servo S1, S2, S3;

bool _state = false;

int offset = 0;

bool leftPressed = false;
bool rightPressed = false;

struct _packet {
  int _X1, _Y1, _X2, _Y2, _L2, _R2;
  byte buttonStates1, buttonStates2;
};

_packet buffer;

void setup() {
// put your setup code here, to run once:
#ifdef DEBUG
  Serial.setRx(PA10);
  Serial.setTx(PA9);
  Serial.begin(115200);
  Serial.println("start");
#endif
  pinMode(PF1, OUTPUT);
  initRadioModule();

  S1.attach(A0);
  S1.writeMicroseconds(1500);
  S2.attach(A1);
  S2.writeMicroseconds(1500);
  S3.attach(A2);
  S3.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly
  digitalWrite(PF1, LOW);
  if (readData()) {
    bool dpad_left = bitRead(buffer.buttonStates1, 6);
    bool dpad_right = bitRead(buffer.buttonStates1, 7);

    if (dpad_left != leftPressed) {
      leftPressed = dpad_left;
      if (dpad_left) {
        offset -= 10;
      }
    }
    if (dpad_right != rightPressed) {
      rightPressed = dpad_right;
      if (dpad_right) {
        offset += 10;
      }
    }
    debugMsg();
    int acc = buffer._R2 - buffer._L2;
    S1.writeMicroseconds(map(acc, -1023, 1023, 1000, 2000));
    S2.writeMicroseconds(map(buffer._Y2, -1023, 1023, 1000, 2000));
    S3.writeMicroseconds(map(buffer._X1 + offset, -1023, 1023, 1000, 2000));
    _state = !_state;
    digitalWrite(PF1, _state);
    delay(2);
  }
}


void debugMsg() {
#ifdef DEBUG
  // Serial.printf("X1:%d,Y1:%d,X2:%d,Y2:%d,L2:%d,R2:%d,B:\n", buffer._X1, buffer._Y1, buffer._X2, buffer._Y2, buffer._L2, buffer._R2);
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
