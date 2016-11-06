#include <SparkFunTSL2561.h>
#include <Wire.h>
// Create an SFE_TSL2561 object, here called "light":

SFE_TSL2561 light;

// Global variables:

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

int pirPin = 2;
int minSecsBetweenEmails = 60; // 1 min
long lastSend = -minSecsBetweenEmails * 1000l;
double lux_old=200;
int blink_count=0;
double lux_diff_threshold=300;
int blink_threshold=5;
double time_span_threshold=10000;
long last_blink=0;
void printError(byte error)
  // If there's an I2C error, this function will
  // print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error,DEC);
  Serial.print(", ");
  
  switch(error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

void setup()
{
  pinMode(pirPin, INPUT);
  Serial.begin(9600);
  light.begin();
  gain=0;
  unsigned char time = 2;

  // setTiming() will set the third parameter (ms) to the
  // requested integration time in ms (this will be useful later):
  
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
  
  Serial.println("Powerup...");
  light.setPowerUp();
}

void loop()
{
  unsigned int data0, data1;
  long now = millis();
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);
  
    // To calculate lux, pass all your settings and readings
    // to the getLux() function.
    
    // The getLux() function will return 1 if the calculation
    // was successful, or 0 if one or both of the sensors was
    // saturated (too much light). If this happens, you can
    // reduce the integration time and/or gain.
    // For more information see the hookup guide at: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor
  
    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    double lux_diff;
    
    // Perform lux calculation:

    good = light.getLux(gain,ms,data0,data1,lux);
    
    // Print out the results:
  
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) {
      Serial.print(" (good)");
      lux_diff=lux-lux_old;
      Serial.print(" lux difference: ");
      Serial.print(lux_diff);
      if(lux_diff<=-lux_diff_threshold || lux_diff>=lux_diff_threshold){
          blink_count++;
          last_blink=now;
        }
      lux_old=lux;} else {
      Serial.println(" (BAD)");}
  }
  else
  {
    // getData() returned false because of an I2C error, inform the user.

    byte error = light.getError();
    
    printError(error);

    
  }




  Serial.print(" blink count: ");
  Serial.println(blink_count);
if (blink_count>=5)
{
  blink_count=0;
if (now > (lastSend + minSecsBetweenEmails * 1000l))
{
Serial.println("MOVEMENT");
lastSend = now;
}
else
{
Serial.println("Too soon");
}
}
//if it has been some time since the last blink detected, reset the blink count
if(now-last_blink>time_span_threshold){
    blink_count=0;
  }
delay(500);
}
