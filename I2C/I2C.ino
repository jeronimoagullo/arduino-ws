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
/* Address of temperature and pressure cofig registers:
   osrs_t -> temprature (bits 7:5)
   osrs_p -> pressure (bits 4:2)
*/
#define CONFIG_OSRS_T_OSRS_P_ADDRESS 0xF4
/* Address of humidity cofig registers:
   osrs_h -> temprature (bits 2:0)
*/
#define CONFIG_OSRS_H_ADDRESS 0xF2

/* this function return the chip_id from sensor BME 280*/
uint8_t ChipIdRead();
/* this function read temperature, humidity and pressure from sensor BME 280*/
void TempHumidPressRead(uint32_t *pressure, uint32_t *temp, uint16_t *humid);
/* this function initialize the sensor MBE 280 */
void InitSensorMBE();
//
///* Functions from BME datasheet */
//double bme280_compensate_H_double(uint32_t adc_H);
//double BME280_compensate_P_double(uint32_t adc_P);
//double BME280_compensate_T_double(uint32_t adc_T);



/* Single-pass function to configure the app */
void setup()
{
  /* Join i2c bus (address optional for master) */
  Wire.begin(0, 2);

  /* start serial for output */
  Serial.begin(115200);

  /* Initilize the sensor MBE 280 */
  InitSensorMBE();
}

/* Recurrent task, called forever */
void loop()
{

  uint32_t temperature = 0;
  uint32_t pressure = 0;
  uint16_t humidity = 0;

  uint8_t chip_id = ChipIdRead();
  TempHumidPressRead(&pressure, &temperature, &humidity);

  /* Send data to console/monitor */
  Serial.printf("Chip id: %02X\n", chip_id);
  Serial.println("Raw values");
  Serial.printf("Temperature: %02X\n", temperature);
  Serial.printf("pressure: %02X\n", pressure);
  Serial.printf("humidity: %02X\n", humidity);

  /* add a CRLF at the end*/
  Serial.println();

  /* Ensure not to flood with a huge amount of fast data */
  delay(500);
}




uint8_t ChipIdRead() {
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


void TempHumidPressRead(uint32_t *pressure, uint32_t *temp, uint16_t *humid) {

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


  *pressure = (registers[2] << 12) | (registers[0] << 8) | registers[1];
  *temp = (registers[5] << 12) | (registers[3] << 8) | registers[4];
  *humid = (registers[6] << 8) | registers[7];

}


void InitSensorMBE() {

  /* Preparing the writting of the humid config regisers in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR);
  /* temperature and pressure config registers */
  Wire.write(CONFIG_OSRS_H_ADDRESS);
  /* write to get oversampling x1 in humidity
     in bytes -> 00000001
  */
  Wire.write(0x01);
  Wire.endTransmission();

  /* Preparing the writting of the temp and pressure config regisers in BME 280 sensor  */
  Wire.beginTransmission(BME_SENSOR);
  /* temperature and pressure config registers */
  Wire.write(CONFIG_OSRS_T_OSRS_P_ADDRESS);
  /* write to get oversampling x1 in temp and pressure
     2 LSB are normal mode
     in bytes -> 00100111
  */
  Wire.write(0x27);
  Wire.endTransmission();

}
