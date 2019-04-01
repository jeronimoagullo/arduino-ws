/* Include required headers and/or libraries */
#include <Wire.h>
/* Address of the BME 280 sensor */
#define BME_SENSOR 0x76
/* Byte that must be sent to read a specific register */
#define SET_REGISTER 0xF6
/* Address of the chip_ip register */
#define CHIP_IP 0xD0
/* Address of the sensor registers */
#define FIRST_ADDRESS 0xF7
/* Bytes of registers */
#define BYTES_REGISTERS 8


/* Single-pass function to configure the app */
void setup()
{
  /* Join i2c bus (address optional for master) */
  Wire.begin(0, 2);

  /* start serial for output */
  Serial.begin(115200);
}

/* this function return the chip_id from sensor BME 280*/
uint8_t ChipIdRead();
/* this function read temperature, humidity and pressure from sensor BME 280*/
void TempHumidPressRead(uint32_t temp, uint32_t humid, uint16_t pressure);

/* Recurrent task, called forever */
void loop()
{

  uint32_t temperature = 0;
  uint32_t humidity = 0;
  uint16_t pressure = 0;

  uint8_t chip_id = ChipIdRead();
  TempHumidPressRead(&temperature, &humidity, &pressure);

  /* Send data to console/monitor */
  Serial.printf("Chip id: %02X\n", chip_id);

  /* add a CRLF at the end*/
  Serial.println();

  Serial.printf("Temperature: %02X\n", temperature);
  Serial.printf("humidity: %02X\n", humidity);
  Serial.printf("pressure: %02X\n", pressure);


  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
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


void TempHumidPressRead(uint32_t *temp, uint32_t *humid, uint16_t *pressure){

  /* the values of the registers */
  uint8_t registers[BYTES_REGISTERS];
  uint8_t i = 0;
  
  /* Prepare the reading of the temperature in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR); 
  Wire.write(FIRST_ADDRESS);               
  Wire.endTransmission();    

  /* Request data from BME 280 */
  Wire.requestFrom(BME_SENSOR, BYTES_REGISTERS);

  /* Wait for data to be available */
  while (Wire.available())
  {
    /* Receive the bytes of registers from 0xF7 to 0xFE */
    registers[i] = Wire.read();
    i++;
  }

  *pressure = (registers[0] << 8) | registers[1];
  *temp = (registers[2] << 12) | (registers[3] << 8) | registers[4];
  *humid = (registers[5] << 12) | (registers[6] << 8) | registers[7];
  
}
