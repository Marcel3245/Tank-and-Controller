#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"
#include "CytronMotorDriver.h"

//<<Receiver>>//
#define CE_PIN 9
#define CSN_PIN 10
#define INTERVAL_MS_SIGNAL_LOST 1000
#define INTERVAL_MS_SIGNAL_RETRY 250
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
//NRF24L01 buffer limit is 32 bytes (max struct size)
struct payload {
  long dataX;
  long dataY;
  byte dataB;
};
payload payload;
unsigned long lastSignalMillis = 0;

//<<Joystick>>//
#define ANALOG_X_CORRECTION 255
#define ANALOG_Y_CORRECTION 255

//<<PWM_DIR_DUAL>>//
CytronMD motorR(PWM_DIR, 5, 4);
CytronMD motorL(PWM_DIR, 6, 7);

void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  
  radio.begin();
  //Append ACK packet from the receiving radio back to the transmitting radio
  radio.setAutoAck(false); //(true|false)
  //Set the transmission datarate
  radio.setDataRate(RF24_250KBPS); //(RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  //Greater level = more consumption = longer distance
  radio.setPALevel(RF24_PA_MIN); //(RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
  //Default value is the maximum 32 bytes1
  radio.setPayloadSize(sizeof(payload));
  //Act as receiver
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  unsigned long currentMillis = millis();

  if (radio.available() > 0) {
    radio.read(&payload, sizeof(payload));
    digitalWrite(8, HIGH);

    int ValueX = readAnalogAxisLevel(payload.dataX) - ANALOG_X_CORRECTION;
    int ValueY = readAnalogAxisLevel(payload.dataY) - ANALOG_Y_CORRECTION;

    Serial.println("Received");
    Serial.print("X:");
    Serial.println(ValueX );
    Serial.print("Y:");
    Serial.println(ValueY);
    Serial.print("Button:");
    Serial.println(payload.dataB);

    motor(ValueX, ValueY); 

    lastSignalMillis = currentMillis;
  }
  if (currentMillis - lastSignalMillis > INTERVAL_MS_SIGNAL_LOST) { 
    lostConnection();
  }
}

int readAnalogAxisLevel(int axis)
{
  return map(axis, 0, 1023, 0, 510);
}
void lostConnection()
{
  Serial.println("We have lost connection, preventing unwanted behavior");
  digitalWrite(8, LOW);
  motor(0,0);
  delay(INTERVAL_MS_SIGNAL_RETRY);
}

void motor (int X, int Y) {
//Forward
if (X > 5 && Y > -5 && Y < 5){
  motorR.setSpeed(X);
  motorL.setSpeed(X);
  }
//Back
else if (X < -5 && (Y > -5 || Y < 5)){
  motorR.setSpeed(X);
  motorL.setSpeed(X);
  }
//Left
else if (Y < -5){
  motorR.setSpeed(-Y);
  motorL.setSpeed(Y);
  }
//Right
else if (Y > 5){
  motorR.setSpeed(-Y);
  motorL.setSpeed(Y);
  }
else{
  motorR.setSpeed(0);
  motorL.setSpeed(0);
  }
}
