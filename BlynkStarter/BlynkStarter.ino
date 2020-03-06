/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define LED_PIN 0
#define LED 0

const int freq = 5000;     // 5KHz frequency is more than enough. Remember we used 100 before.
const int ledChannel = 0;  // You can have up to 16 pwm channels (0 to 15)
const int resolution = 10; // 10 bit resolution for 1023. Freq should be greater than resolution.

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int time_count = 0; // timer counter global variable
int seconds = 0;
String content = "";  // null string constant ( an empty string )
BlynkTimer timer;
int firsttime = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "5Zsu5jBmCbg7n6IS-e2VUTuXu3iFDyl_";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "EE-IOT-Platform-02";
char pass[32] = "g!TyA>hR2JTy";

//global variables
uint32_t LED_Brightness;
uint32_t LED_State;
int32_t currentDutyCycle;
int32_t timeCounter;
String message;

void myTimerEvent() // Every 10 ms
{
    if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        time_count = 0; // reset time counter
    }
    if ((time_count % 10 == 0) && (firsttime == 1)){
        seconds++;
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        Blynk.virtualWrite(V3, seconds);
        if (content != "") {
            Blynk.virtualWrite(V2, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
    firsttime = 1;
}

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  // configure LED PWM functionality
   ledcSetup(ledChannel, freq, resolution);

   // attach the channel to the GPIO to be controlled
   ledcAttachPin(LED, ledChannel);
   timer.setInterval(10L, myTimerEvent); // 10 ms interval
}

BLYNK_WRITE(V0)//switch
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int val = param.asInt(); // assigning incoming value from pin V1 to a variable
    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (val == 1) {
       LED_State = 1;
       ledcWrite(LED_PIN, LED_Brightness);
    }
    else {
        LED_State = 0;
        ledcWrite(LED_PIN, 0);
    }
}

BLYNK_WRITE(V1)//slider
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
    LED_Brightness = pinValue;
    if (LED_State == 1) {
       ledcWrite(LED_PIN, LED_Brightness);
    }
    
}

BLYNK_WRITE(V2)//display
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int displayvalue = param.asInt(); // assigning incoming value from pin V1 to a variable
    ledcWrite(LED_PIN, LED_Brightness);
    
    
}

void loop()
{
  Blynk.run();
  timer.run();
}
