int level = 20;
int position = 0;

void setup() {
  Serial.begin(9600);
  backlightOn();
}

void backlightOn(){  //turns on the backlight
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

void loop() {
  delay(1000);
  writeTopLine("Hola mundi");
  
  writeBottomLine("soak my cork");

}
