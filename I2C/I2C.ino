/* Include required headers and/or libraries */
#include <Wire.h>
/* Address of the BME 280 sensor */
#define BME_SENSOR 0x76
/* Byte that must be sent to read a specific register */
#define SET_REGISTER 0xF6
/* Address of the chip_ip register */
#define CHIP_IP 0xD0
/* Address of the temperature registers */
#define TEMP_ADDRESS 0xFA

/* Single-pass function to configure the app */
void setup()
{
  /* Join i2c bus (address optional for master) */
  Wire.begin(0, 2);

  /* start serial for output */
  Serial.begin(115200);
}


/* this function return the temperature from sensor BME 280*/
uint16_t TemperatureRead();
/* this function return the chip_id from sensor BME 280*/
uint8_t ChipIdRead();

/* Recurrent task, called forever */
void loop()
{

  uint8_t chip_id = ChipIdRead();
  uint16_t temp = TemperatureRead();

  /* Send data to console/monitor */
  Serial.printf("Chip id: %02X\n", chip_id);
  Serial.printf("Temperature: %02X\n", temp);

  /* add a CRLF at the end*/
  Serial.println();

  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
}



uint16_t TemperatureRead(){

  uint8_t msbyte = 0;
  uint8_t lsbyte = 7;
  
  /* Prepare the reading of the temperature in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR); 
  Wire.write(TEMP_ADDRESS);               
  Wire.endTransmission();    

  /* Request data from BME 280 */
  Wire.requestFrom(BME_SENSOR, 2);

  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the more significant byte */
    msbyte = Wire.read();
    /* Receive the less significant byte */
    lsbyte = Wire.read();

  }
  
  /* join the two bytes */
    return (msbyte << 8) | lsbyte;

}


uint8_t ChipIdRead(){
  uint8_t c = 5;
  /* Prepare the reading of the chip_ip in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR); 
  Wire.write(CHIP_IP);               
  Wire.endTransmission();    

  /* Request data from BME 280 */
  Wire.requestFrom(BME_SENSOR, 1);

  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the byte */
    c = Wire.read();
  }

  return c;


}
