

import processing.serial.*;

Serial mySerial;  // Create object from Serial class
String myString;      // Data received from the serial port
int nl = 10;
float myVal;

void setup() 
{
  size(400, 400);
  background(0, 0, 0);
  String portName = Serial.list()[1];
  mySerial = new Serial(this, portName, 9600);
}

void draw()
{
  while ( mySerial.available() > 0) {  // If data is available,
    myString = mySerial.readStringUntil(nl);         // read it and store it in val
    if (myString!=null) {
      
      println(myString);
      background(int(getValue(myString, '\t', 0)), int(getValue(myString, '\t', 1)), int(getValue(myString, '\t', 2))); 
    }
  }
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for (int i=0; i<=maxIndex && found<=index; i++) {
    if (data.charAt(i)==separator || i==maxIndex) {
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
