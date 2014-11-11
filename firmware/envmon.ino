#include <SPI.h>
#include <Wire.h>
#include <Scout.h>
#include <GS.h>
#include <bitlash.h>
#include <lwm.h>
#include <js0n.h>
#include <key/key.h>

#include <SFE_TSL2561.h>

#define SKETCH_NAME "Custom"
#define SKETCH_BUILD -1
#define SKETCH_REVISION "unknown"

// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;

// Global variables:
int tmp102Address = 0x48;
boolean gain = 0;
unsigned int ms;
unsigned char time = 2;
double lux = 0.0;
float temperature = 0.0;

int timedelay = 10000;
long prevtime = 0;

void setup(){
	// Setup
	Serial.begin(115200);
	Scout.setup(SKETCH_NAME, SKETCH_REVISION, SKETCH_BUILD);
  
	// Setup TSL2561
	light.begin();
	light.setTiming(gain,time,ms);
	light.setPowerUp();
  
	// Add Bitlash functions
	addBitlashFunction("env.light", (bitlash_function)getlight);
	addBitlashFunction("env.temperature", (bitlash_function)gettemperature);
	addBitlashFunction("envmon.report", (bitlash_function)reportenvdata);
}

void loop(){
	Scout.loop();
  
	// Check time interval
	if((millis() - prevtime) >= timedelay){
		// Get values
		lux = getLux();
		temperature = getTemperature();
  
		// Debug messages
		Serial.print("Light Value: ");
		Serial.print(lux);
		Serial.print(" lx");
		Serial.print(" Temperature: ");
		Serial.print(temperature);
    
		// Report values to the HQ
		reportHQ();
    
		// Update previous time
		prevtime = millis();
	}
}

// Returns LUX
double getLux(){
  unsigned int data0, data1;
  
  light.getData(data0,data1);
    
  light.getLux(gain,ms,data0,data1,lux);
  
  return lux;
}

// Return temperature
float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return celsius;
}

// Bitlash function to retrieve light value
numvar getlight(void){
  return (numvar)lux;
}

// Bitlash function to retrieve temperature
numvar gettemperature(void){
  return (numvar)temperature;
}

// Bitlash function to retrieve env data
numvar reportenvdata(void){
  reportHQ();
  
  return (numvar)1;
}

// Send data to HQ
void reportHQ(void) {
  StringBuffer report(100);
  report.appendSprintf("[%d,[%d, %d],[%d, %d]]",
          keyMap("env", 0),
          keyMap("l", 0),
          keyMap("t", 0),
          (int)lux,
          (int)temperature);
  Scout.handler.report(report);
}