#include <Wire.h> 

#include <Adafruit_Sensor.h> // install from

#include <Adafruit_MPL3115A2.h> // install from https://github.com/adafruit/Adafruit_MPL3115A2_Library

#include <Adafruit_TSL2591.h> // install from 

#include <Adafruit_MCP9808.h> // install from 

// initiate altimeter object (MPL3115A2)
Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
  
// initiate LUX sensor object (TSL2591)
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// initiate Precision temp sensor object (MCP9808)
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();




void setup()
{
  Serial.begin(115200);   //begin serial connection for communication with 'puter or phone

  // configure LUX sensor (TSL2591)
  configLux();
}

void loop()
{
  readTemp();  // read and print temperature data from MCP9808
  
  readLux();  // read and print light data from TSL2591
  
  readPressure();  // read and print pressure data from MPL3115A2
  
}


void readPressure(void)
{  
  float pascals = baro.getPressure();
  // Our weather page presents pressure in Inches (Hg)
  // Use http://www.onlineconversion.com/pressure.htm for other units
  Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

  float altm = baro.getAltitude();
  Serial.print(altm); Serial.println(" meters");

  float tempC = baro.getTemperature();
  Serial.print(tempC); Serial.println("*C");

}


void readTemp(void) {
  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t"); 
  // Serial.print(f); Serial.println("*F");
 }

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configLux(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         ");
  tsl2591Gain_t gain = tsl.getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println("1x (Low)");
      break;
    case TSL2591_GAIN_MED:
      Serial.println("25x (Medium)");
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println("428x (High)");
      break;
    case TSL2591_GAIN_MAX:
      Serial.println("9876x (Max)");
      break;
  }
  Serial.print  ("Timing:       ");
  Serial.print((tsl.getTiming() + 1) * 100, DEC); 
  Serial.println(" ms");
  Serial.println("------------------------------------");
  Serial.println("");
}


/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void readLux(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
  Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
  Serial.print("Full: "); Serial.print(full); Serial.print("  ");
  Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
}
