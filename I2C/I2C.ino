/* Include required headers and/or libraries */
#include <Wire.h>
/* Address of the BME 280 sensor */
#define BME_SENSOR 0x76
/* Byte that must be sent to read a specific register */
#define SET_REGISTER 0xF6
/* Address of the chip_ip */
#define CHIP_IP 0xD0


/* Single-pass function to configure the app */
void setup()
{
  /* Join i2c bus (address optional for master) */
  Wire.begin(0, 2);

  /* start serial for output */
  Serial.begin(115200);
}


/* Recurrent task, called forever */
void loop()
{
  /* Prepare the reading og the chip_ip in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR); 
  Wire.write(CHIP_IP);               
  Wire.endTransmission();    

  /* Request data from BME 280 */
  Wire.requestFrom(BME_SENSOR, 1);

  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the byte */
    uint8_t c = Wire.read();

    /* Send it to console/monitor */
    Serial.printf("Received byte: %02X\n", c);
  }

  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
}
