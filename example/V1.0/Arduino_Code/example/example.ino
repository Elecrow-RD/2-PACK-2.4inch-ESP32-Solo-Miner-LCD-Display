//at 9600 bps 8-N-1
#include <SoftwareSerial.h>

SoftwareSerial SoftSerial(16, 17);
unsigned char buffer[256]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array 
void setup()
{
  SoftSerial.begin(9600);               // the SoftSerial baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.

}

void loop()
{
  if (SoftSerial.available())              // if date is comming from softwareserial port ==> data is comming from SoftSerial shield
  {
    while(SoftSerial.available())          // reading data into char array 
    {
      buffer[count++]=SoftSerial.read();     // writing data into array
      if(count == 256)break;
  }
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero

  }
  if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
    SoftSerial.write(Serial.read());       // write it to the SoftSerial shield
}

void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}