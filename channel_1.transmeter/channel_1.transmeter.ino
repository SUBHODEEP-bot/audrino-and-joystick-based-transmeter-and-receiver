#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";


struct Data_Package {
  int xValue;
  int yValue;
  int swValue; /
};

Data_Package data;

void setup() {
  Serial.begin(9600);
  
  pinMode(A2, INPUT_PULLUP); 
  
  radio.begin();
 
  radio.setChannel(50); // you can change it as per your uses
  
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("Transmitter Ready with Switch & Channel 50...");
}

void loop() {

  data.xValue = analogRead(A1); 
  data.yValue = analogRead(A0);
  
  data.swValue = digitalRead(A2);


  Serial.print("X: "); Serial.print(data.xValue);
  Serial.print(" | Y: "); Serial.print(data.yValue);
  Serial.print(" | SW: "); Serial.println(data.swValue);


  radio.write(&data, sizeof(Data_Package));

  delay(20); 
}
