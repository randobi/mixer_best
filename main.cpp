



#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>

BlynkTimer timer;

char auth[] = "70jLqZ_WIgoghZDbeBGL2B8Gg2FJ39dM";
char ssid[] = "RoboLand";
char pass[] = "P0pShop9724";

// On and Off Times (as int, max=32secs)
const unsigned int onTime = 10000;
const unsigned int offTime = 5000;

//Tracks the last time event fired
unsigned long previousMillis = 0;
 
// Interval is how long we wait
unsigned long interval = onTime;
 
// Used to track if LED should be on or off
boolean LEDstate = true;
int ledPin = 5;
//int ledPin;

//Virtual function pins incremented on Blynk app
int cycleCount; //incremented by V2
int restTime; //incremented by V3
int mixTime; //incremented by V4

/*Used for duration of respective time
unsigned long int stirTime; // =(mix time * minutes) interval (millis)
unsigned long int waitTime; // =(rest time * minutes) interval (millis)
timer.setInterval(1000L, Blynk.syncVirtual(V0));*/

//from Dual_Blynk for delay setup
//unsigned long currentMillis = 0; 
unsigned long previous_OnBoardLED_Millis = 0;        // will store last time LED was updated
//unsigned long previous_AddedLED_Millis = 0; 

// constants won't change:
//unsigned long int interval_1 = stirTime;           // interval at which to blink (milliseconds)
//unsigned long interval_2;
//boolean ledPin = true;

void setup() {
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,165),8080);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  //timer.setInterval(1000L, ); // Match the vPin with that of the Blynk function
}

void runcycle() { 
  Serial.println("Start of runcycle");
  digitalWrite(ledPin, LEDstate);
  unsigned long currentMillis = millis();
  //int waitTime = restTime * offTime;
  //int stirTime = mixTime * onTime;

    /*for (int i = cycleCount; i > 0; i--)
      {*/
      
    if (currentMillis - previous_OnBoardLED_Millis >= interval) { //use stirTime if this works
    // save the last time you blinked the LED

      if (LEDstate){
      interval = offTime;
      }
        else
        {
        interval = onTime;
        }

      LEDstate = !(LEDstate);

      previous_OnBoardLED_Millis = currentMillis;
      //previousMillis = currentMillis;
      //digitalWrite(ledPin,HIGH); }

      Blynk.virtualWrite(7, 255); //blynk LED blob, this has to be a value 0-255;
      Blynk.virtualWrite(8,0);

      //delay(stirTime);      
      Serial.print("Cycle "); Serial.println(cycleCount);
        
      //digitalWrite(5,LOW);
      //delay(waitTime);
        
      cycleCount --;
      Blynk.virtualWrite(1,cycleCount);
      Serial.print("Mix cycles remaining " ); Serial.println(cycleCount);
        if(cycleCount == 0){
        Blynk.virtualWrite(7,0);
        Blynk.virtualWrite(8,255);
        }
    }
    Serial.println("End of runcycle");
}


//Establish Cycle parameters: Cycles,mix time, rest time
  
    BLYNK_WRITE(V2){    //CYCLES this is the number of cycles button pin
    if(param.asInt() == 1) { //this is a Blynk function: assigning incoming value from pin V1 to a variable
      cycleCount++;            // is this the param?
        if(cycleCount > 10){
          cycleCount = 1;
        }      
      }
    Blynk.virtualWrite(1,cycleCount); // 1 is Blynk widget that increments count with button push
    Serial.print("Cycle count = "); Serial.print(cycleCount); Serial.println(" cycles.");
    //Blynk.virtualWrite(3, 127); //blynk LED blob, this has to be a value 0-255;
    }

  BLYNK_WRITE(V3){    //REST is the time between mix periods
    if(param.asInt() == 1) { //this is a Blynk function: assigning incoming value from pin V1 to a variable
      restTime++;            // is this the param?
        if(restTime > 10){
          restTime = 1;
        }            
      }
    Blynk.virtualWrite(6,restTime); // 1 is Blynk widget that increments count with button push
    Serial.print("Rest time = "); Serial.print(restTime); Serial.println(" minutes.");   
  } 

  BLYNK_WRITE(V4){ //MIX is the duration of mix period
    if(param.asInt() == 1) {//this is a Blynk function: assigning incoming value from pin V4 to a variable
      mixTime++; 
        if(mixTime > 10){
          mixTime = 1;//Serial.print (mixTime);
        }
    }
      Blynk.virtualWrite(5,mixTime); 
      Serial.print("Mix time = "); Serial.print(mixTime); Serial.print(" minutes.\n");     
  } 

  BLYNK_WRITE(V0){ //this is the program start button
    if (param.asInt()==1){
      for (int i = cycleCount; i >= 1; i--){
      runcycle();
      } 
    }
  }  

void loop() {
  timer.run();
  Blynk.run();
}