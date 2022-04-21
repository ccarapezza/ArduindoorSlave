#include <Wire.h>
int latchPin = 3;
int clockPin = 4;
int dataPin = 2;

byte dataByteStore = B10101010;

void setup() {

  // Unimos este dispositivo al bus I2C con dirección 1
  Wire.begin(1);

  // Registramos el evento al recibir datos
  Wire.onReceive(receiveEvent);

  // Iniciamos el monitor serie para monitorear la comunicación
  Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, LOW);

  writeData(dataByteStore);

  Serial.println("Started...");
}

void loop() {
  // put your main code here, to run repeatedly:

}

void receiveEvent(int bytes) {

  Serial.print("receiveEvent, bytes: ");
  Serial.println(bytes);

  char idFunction = Wire.read();
  Serial.print("idFunction: ");
  Serial.println(idFunction);

  switch (idFunction) {
    case 'A':
      setChannelOutput();
      break;
    case 'B':
      toggleChannelOutput();
      break;
    default:
      Serial.println("E: No function found...");
  }
}

void setChannelOutput(){
  Serial.println("setChannelOutput");
  byte bitIndex = Wire.read();
  int state = Wire.read();

  Serial.print("lastParams:");
  while(Wire.available()){
    Serial.println(Wire.read());
  }

  updateBit(bitIndex, state);

  // Activamos/desactivamos salida
  writeData(dataByteStore);
}

void toggleChannelOutput(){
  Serial.print("toggleChannelOutput");
  int bitIndex = Wire.read();

  toggleBit(bitIndex);

  // Activamos/desactivamos salida
  writeData(dataByteStore);
}

void updateBit(int n, int state){
  dataByteStore = (dataByteStore & ~(1UL << n)) | (state << n);
}

void toggleBit(int n){
  dataByteStore ^= 1UL << n;
}

void writeData(byte data){
  shiftOut(dataPin, clockPin, MSBFIRST, dataByteStore);
  // enviamos el dato y pulsamos para que pase
  // al registro de salida y lo podamos ver
  digitalWrite(latchPin,HIGH); // pulso ALTO
  digitalWrite(latchPin,LOW); // pulso BAJO
}