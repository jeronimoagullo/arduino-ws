/* Include required headers and/or libraries */
#include <Wire.h>
#include "SparkFunBME280.h"

#define SLAVE_ADDRESS 0x76

/* Instantiate a BME280 object called BME280_obj */
BME280 mybme;

/*
 * Single-pass function to configure the app
 */
void setup()
{
  /* Start serial for output */
  Serial.begin(115200);

  /* Join I2C bus and set it to 400 kHz */
  Wire.begin(0, 2);
  Wire.setClock(400000);

  /* Address the sensor */
  mybme.setI2CAddress(SLAVE_ADDRESS);

  /* Check communication before continue */
  if (mybme.beginI2C(Wire) == false)
  {
    Serial.printf("The sensor did not respond. Please check wiring.\n");

    /* Stop here (WDT will reset at some point) */
    while(1);
  }
  
  mybme.setTempOverSample(16); //0 to 16 are valid. 0 disables temp sensing. See table 24.
  mybme.setPressureOverSample(16); //0 to 16 are valid. 0 disables pressure sensing. See table 23.
  mybme.setHumidityOverSample(16); //0 to 16 are valid. 0 disables humidity sensing. See table 19.
  
  mybme.setMode(MODE_NORMAL); //MODE_SLEEP, MODE_FORCED, MODE_NORMAL is valid. See 3.3
}

/*
 * Recurrent task, called forever
 */
void loop()
{
  /* Welcome message! Useful as a control point */
  Serial.printf("Ahoy! ESP8266 here!\n---\n");
  float humid = mybme.readFloatHumidity();
  float pressure = mybme.readFloatPressure();
  float altitude = mybme.readFloatAltitudeMeters();
  float temp = mybme.readTempC();

  Serial.printf("Humidity: %.2f\n",humid);

  Serial.printf(" Pressure: %.2f\n",pressure);

  Serial.printf(" Alt: %.2f\n",altitude);

  Serial.printf("Temp: %.2f\n", temp);

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   *  Ask for the data and print to console here !!
   * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
}
