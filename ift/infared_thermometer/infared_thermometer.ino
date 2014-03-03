#include <i2cmaster.h>

void setup(){
    Serial.begin(9600);
    backlightOn();
    
    i2c_init(); //Initialise the i2c bus
    PORTC = (1 << PORTC4) | (1 << PORTC5);//enable pullups
}

void loop(){
  int dev = 0x5A<<1;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;
  
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);
  
  // read
  i2c_rep_start(dev+I2C_READ);
  data_low = i2c_readAck(); //Read 1 byte and then send ack
  data_high = i2c_readAck(); //Read 1 byte and then send ack
  pec = i2c_readNak();
  i2c_stop();
  
  //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
  double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
  double tempData = 0x0000; // zero out the data
  int frac; // data past the decimal point
  
  // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;
  
  float celcius = tempData - 273.15;
  float fahrenheit = (celcius*1.8) + 32;

  char c[25] = "C: ";
  dtostrf(celcius, 1, 2, &c[3]);
  writeTopLine(c);

  char f[25] = "F: ";
  dtostrf(fahrenheit, 1, 2, &f[3]);
  writeBottomLine(f);

  delay(1000); // wait a second before printing again
}

void backlightOn(){  //turns on the backlight
   int level = 20;
   Serial.write(0x7C);   //command flag for backlight stuff
   Serial.write(level);    //light level.
   delay(10);
}

void goTo(int position) { //position = line 1: 0-15, line 2: 16-31, 31+ defaults back to 0
  if (position < 16){ 
    Serial.write(0xFE);   //command flag
    Serial.write((position+128));    //position
  } else if (position<32) {
    Serial.write(0xFE);   //command flag
    Serial.write((position+48+128));    //position 
  } else { 
    goTo(0);
  }
   delay(10);
}

void writeTopLine(char line[]) {
  writeLine(0, line);
}

void writeLine(int n, char line[]){
  int  i = 0;
  int startPosition = n * 16;
  
  for (;i<16;i++) {
    goTo(i + startPosition);
    if (i > (strlen(line))-1) {
      Serial.write(" ");    
    } else {
      Serial.write(line[i]);      
    }
  }
}

void writeBottomLine(char line[]) {
  writeLine(1, line);
}



