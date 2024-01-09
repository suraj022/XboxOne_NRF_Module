#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 1
#define CSN_PIN 0

RF24 radio(CE_PIN, CSN_PIN);

#define RF_POWER RF24_PA_HIGH

const uint8_t data_address[5] = {64, 87, 39, 29, 56};

void initRadioModule() {
  if (!radio.begin()) {
    // Serial.println("radio hardware is not responding!!");
    bool _state = false;
    while (1) {
      delay(100);
      _state = !_state;
      digitalWrite(13, _state);
    }
  }

  radio.setPALevel(RF_POWER); 
  radio.openWritingPipe(data_address);     // always uses pipe 0
  radio.openReadingPipe(1, data_address);  // using pipe 1
  radio.setAutoAck(0, false);
  radio.setAutoAck(1, false);
  radio.startListening();  // put radio in RX mode
}

void sendData(struct _packet _buf) {
  radio.stopListening();
  delay(1);
  radio.write(&_buf, sizeof(_buf));
  delay(1);
  radio.startListening();
}

void readData(){
  _packet _buf;
  if (radio.available()) {
    radio.read(&_buf, sizeof(_buf));
  }
}
