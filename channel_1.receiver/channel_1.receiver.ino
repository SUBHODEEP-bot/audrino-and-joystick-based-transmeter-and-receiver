#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define enA 2  
#define in1 3
#define in2 4
#define enB 7   
#define in3 5
#define in4 6

#define extraIN1 17 // Nano A3
#define extraIN2 18 // Nano A4

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

struct Data_Package {
  int xValue;
  int yValue;
  int swValue; 
};
Data_Package data;

bool motorStatus = false;
bool lastButtonState = HIGH;

void setup() {
  pinMode(enA, OUTPUT); pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  pinMode(extraIN1, OUTPUT);
  pinMode(extraIN2, OUTPUT);
  
  Serial.begin(9600);
  radio.begin();

  radio.setPALevel(RF24_PA_MIN); 
  radio.setChannel(50); // channel 50 (you can change as per your uses)
  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("Receiver 1 Started...");
  Serial.println("Waiting for data from Transmitter...");
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));


    Serial.print("Recv -> X: "); Serial.print(data.xValue);
    Serial.print(" | Y: "); Serial.print(data.yValue);
    Serial.print(" | SW: "); Serial.print(data.swValue);
    Serial.print(" | Motor: "); Serial.println(motorStatus ? "ON" : "OFF");
    
  
    if (data.xValue < 150) {
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW); 
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW); 
      digitalWrite(enA, HIGH); digitalWrite(enB, HIGH);
    }
    else if (data.xValue > 850) {
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH); 
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH); 
      digitalWrite(enA, HIGH); digitalWrite(enB, HIGH);
    }
    else if (data.yValue < 150) {
      digitalWrite(in1, HIGH); digitalWrite(in2, LOW); 
      digitalWrite(in3, LOW); digitalWrite(in4, HIGH); 
      digitalWrite(enA, HIGH); digitalWrite(enB, HIGH);
    }
    else if (data.yValue > 850) {
      digitalWrite(in1, LOW); digitalWrite(in2, HIGH); 
      digitalWrite(in3, HIGH); digitalWrite(in4, LOW); 
      digitalWrite(enA, HIGH); digitalWrite(enB, HIGH);
    }
    else {
      digitalWrite(enA, LOW); digitalWrite(enB, LOW);
      digitalWrite(in1, LOW); digitalWrite(in2, LOW);
      digitalWrite(in3, LOW); digitalWrite(in4, LOW);
    }

    if (data.swValue == LOW && lastButtonState == HIGH) {
      motorStatus = !motorStatus;
      delay(50); // Debounce delay
    }
    lastButtonState = data.swValue;

    if (motorStatus == true) {
      digitalWrite(extraIN1, HIGH);
      digitalWrite(extraIN2, LOW);
    } else {
      digitalWrite(extraIN1, LOW);
      digitalWrite(extraIN2, LOW);
    }
  } 
}
