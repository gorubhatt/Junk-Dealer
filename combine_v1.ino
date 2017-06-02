// pin 2 and pin 5 for utrasonic sensor
// pin 3 for Inductive Sensor
// pin A9 and A10 for capacitive sensor
//pin A5 and A6 for dragger motor
//A0 and A1 for weight sensor 
#include <Stepper.h>

const int stepsPerRevolution = 200;  
const int trigPin = 2;
const int echoPin = 5;
const int ref_detection=15;
int check=0;           //1- Metal 2-Plastic 3- Landfill
int ind=0;
int capref,cap=0;
int sensorValind,sensorValcap;
int stepcount = 0;      // number of steps the motor has taken

int a0;
float analog_val_wt=0.0;
float sum_wt=0.0;
float avg_wt=0.0;
float tare_wt=0.0;
int num_samples=50;

void setup()
{
  pinMode(3, INPUT_PULLUP);
  
  pinMode(A9, INPUT);
  pinMode(A10,INPUT);

  pinMode(A5,INPUT);
  pinMode(A6,INPUT);

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
    
  Serial.begin(9600);
  capref=analogRead(A9)-analogRead(A10);
}

long microsecondsToCentimeters(long microseconds)
 {
  return microseconds / 29 / 2;
  
 }

void tare()
{
  for(int i=0;i<num_samples;i++)
    {
      sum_wt+=(analogRead(A9)-analogRead(A10));
      delay(10); 
    }
    tare_wt=sum_wt/num_samples;
    tare_wt=(tare_wt*5.0)/1024;
    tare_wt*=100;
    Serial.print("ref = ");
    Serial.println(Tare_wt);
    sum_wt=0;
}

float check_wt()
{
    int i=0;
    sum_wt=0;
    for(i=0;i<30;i++)
    {
      a0=(analogRead(A9)-analogRead(A10));
      analog_val_wt= (a0*5.0)/1024;
      analog_val_wt=analog_val-tare_wt;
      sum_wt+=(analog_val_wt*100);
      delay(10);
    }
    avg_wt=sum_wt/30.0;
    Serial.println(avg_wt);    
    avg_wt=0.0;
    delay(300); 
    return (avg_wt*5);
}

void loop()
  {

          tare();
          long duration,cm;
                       // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
                       // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
          pinMode(trigPin, OUTPUT);
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
        
          // Read the signal from the sensor: a HIGH pulse whose
          // duration is the time (in microseconds) from the sending
          // of the ping to the reception of its echo off of an object.
          pinMode(echoPin, INPUT);
          duration = pulseIn(echoPin, HIGH);
          
          cm = microsecondsToCentimeters(duration);
          Serial.print(cm);
          Serial.print("cm");
          Serial.println();
          delay(1000);

          if(cm<ref_detection)
          {                                 // Checking Junk Type
              sensorValind = digitalRead(3);
              sensorValcap=analogRead(A9)-analogRead(A10);
              
              if (sensorValind == HIGH) 
                  { 
                    Serial.println("Inductive on");
                    ind = 1;
                  }
                  else //if(sensorVal = LOW)
                  {
                    Serial.println("Inductive off");
                    ind = 0;
                  }

            if (sensorValcap > 150) 
                  { 
                    Serial.println("Capacitive on");
                    cap = 1;
                  }
                  else //if(sensorVal = LOW)
                  {
                    Serial.println("Capacitive off");
                    cap = 0;
                  }

          if(cap==1 && ind==1)
            check=1;
          else if(cap==0 && ind==0)
            check=2;
          else
            check=3;
              
              Serial.print("Check = ");
              Serial.println(check);

           delay(300); 
                                         // Adjusting Bin Divider
                                      //Motor 2
           int motorSpeed = 10;       // reconfigure it to calibrate with the bins
                
            if (motorSpeed > 0) {
            myStepper.setSpeed(motorSpeed);
            myStepper.step(66);
            stepcount++;
            Serial.println("1");
            delay(1000);
            if(stepcount==3)
            break;
            }

                                             //Taking the junk to weight sensor 
            analogWrite(A5,255);
            analogWrite(A6,0);
            delay(3000);
            analogWrite(A5,0);
            analogWrite(A6,0);

                                              // Measuring Weight

            float weight= check_wt();
            Serial.print("Weight = ");
            Serial.println(weight);
            delay(5000);                                  
                                              // Transfer junk to Bin
            
            analogWrite(A5,255);
            analogWrite(A6,0);
            delay(3000);
            analogWrite(A5,0);
            analogWrite(A6,0);
                                              //Print Details On LCD Display 

            
              
          }
          
  }
  


