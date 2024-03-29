#include "SPI.h"
#include "RF24.h"
#include "nRF24L01.h"

// <<Transmitter>> //
// Define pins and time interval
#define CE_PIN 9
#define CSN_PIN 10
#define INTERVAL_MS_TRANSMISSION 250

// Initialize NRF24L01 radio
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

// NRF24L01 buffer limit is 32 bytes (max struct size)
struct payload {
  long dataX;
  long dataY;
  byte dataB;
};
payload payload;

// <<Joystick>> //
// Define analog pins for joystick and button
#define ANALOG_X_PIN A4
#define ANALOG_Y_PIN A3
#define ANALOG_BUTTON_PIN A2

// Define structures for button and analog readings
struct button {
  byte pressed = 0;
};
struct analog {
  short x, y;
};

void setup() {
  // Start Serial communication
  Serial.begin(9600);
  // Set pin mode for analog button
  pinMode(ANALOG_BUTTON_PIN, INPUT_PULLUP);

  // Initialize NRF24L01 radio configuration
  radio.begin();
  // Append ACK packet from the receiving radio back to the transmitting radio
  radio.setAutoAck(false); // (true|false)
  // Set the transmission datarate
  radio.setDataRate(RF24_250KBPS); // (RF24_250KBPS|RF24_1MBPS|RF24_2MBPS)
  // Greater level = more consumption = longer distance
  radio.setPALevel(RF24_PA_MAX); // (RF24_PA_MIN|RF24_PA_LOW|RF24_PA_HIGH|RF24_PA_MAX)
  // Default value is the maximum 32 bytes
  radio.setPayloadSize(sizeof(payload));
  // Act as transmitter
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  // Read analog joystick values and button state
  analog analog;
  payload.dataX = analogRead(ANALOG_X_PIN);
  payload.dataY = analogRead(ANALOG_Y_PIN);
  payload.dataB = isAnalogButtonPressed(ANALOG_BUTTON_PIN);

  // Send data over NRF24L01
  radio.write(&payload, sizeof(payload));

  // Print sent data to Serial Monitor
  Serial.print("X:");
  Serial.println(payload.dataX);
  Serial.print("Y:");
  Serial.println(payload.dataY);
  Serial.print("Button:");
  Serial.println(payload.dataB);
  Serial.println("Sent");

  // Delay before next transmission
  delay(INTERVAL_MS_TRANSMISSION);
}

// Function to check if the analog button is pressed
bool isAnalogButtonPressed(int pin) {
  return digitalRead(pin) == 0;
}
