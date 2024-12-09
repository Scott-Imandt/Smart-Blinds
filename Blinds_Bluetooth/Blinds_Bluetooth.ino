#include <Servo.h> // accsess the arduino servo lib

Servo servoA; // creates a variable for the servo
Servo servoB;
int valA;
int valB;
int phase = 0;
int outputValueA;
int outputValueB;
int data;

int test[4];
int sendData[3];

int servoA_Min = 1;
int servoB_Min = 1;
int servoA_Max = 90;
int servoB_Max = 90;

void setup() {
  servoA.attach(9); //ensures that the servo is on pin 9
  servoB.attach(6);//ensures that the servo2 is on pin 6
  Serial.begin(9600);
}

void loop() {
  photoresistorWrite();
  bluetoothData();
  delay(100);
}

void photoresistorWrite() {

  if (phase == 1 || test[1] == 231) {
    phase = 1;
//    Serial.print("phase 1:  ");

    valA = analogRead(1);// reads the value of the the (photo resistor from A1 )
    if(valA > 200)valA = 200;
    if(valA <35) valA = 35;
    valB = valA;
    //Serial.println(val);
    valA = map(valA, 35, 200 , servoA_Min, servoA_Max); // converts reading from (poteniomiter) to an output value in degrees of rottation that the servo can understand
    valB = map(valB, 35, 200 , servoB_Min, servoB_Max);
//    Serial.println(val);
    servoA.write(valA);
    servoB.write(valB);
    delay(1000);
  }
}

void bluetoothData() {
    int i = 0;
    boolean newData = false;
    while(Serial.available()>0){
      delay(10);
      int temp = Serial.read();

      if(temp != 999 || i == 3){
        test[i] = temp;
//        Serial.println(test[i]);   
        i++;
      }
//      else{      
        newData = true;        
//      }     
    }
    if(test[0] == 0){    
      if (test[1] == 0) {
        phase = 0;     
      }
       
       if(newData == true) {
        
//      Serial.print("phase 0:  ");
        outputValueA = map(test[1], 1, 100, servoA_Min, servoA_Max);
        outputValueB = map(test[1], 1, 100, servoB_Min, servoB_Max);
//      Serial.println(outputValue);
        servoA.write(outputValueA);
        servoB.write(outputValueB);
//      Serial.println(test[1]);
        
      }
    }

    if(test[0] == 1){
      test[0] = 3;

      switch(test[1]){
        case 0:
          if(test[2] == 1){ servoA_Max = test[3];} else{ servoA_Min = test[3];}

        break;

        case 1:
          if(test[2] == 1){ servoB_Max = test[3];} else{ servoB_Min = test[3];}

        break;
      }

      
      
    }

    if(test[0] == 2){
      test[0] = 3;
//      Serial.println(test[2]);
      btSendData(test[2]);
      
    }
}

  void btSendData(int maxOrMin){
    if(maxOrMin == 1){
      sendData[0] = servoA_Max; sendData[1] = servoB_Max;
    }
    else{
      sendData[0] = servoA_Min; sendData[1] = servoB_Min;
    }
    
//    sendData[2] = servoA_Max;

    for(int i=0; i<2; i++){
      Serial.print(sendData[i]);
      Serial.print("\t");
    }  
   
  //Serial.println("!!Data sent!!");
      Serial.flush();
}
