
//
//    FILE: dht21_test.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.01
// PURPOSE: DHT library test sketch for DHT21 && Arduino
//     URL:
//
// Released to the public domain
//

#include <dht.h>

dht DHT;

#define DHT21_PIN 5

void setup()
{
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  //    Serial.print("LIBRARY VERSION: ");
  //    Serial.println(DHT_LIB_VERSION);
  //    Serial.println();
  //    Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

bool StartsWith(const char *a, const char *b)
{
  if(strncmp(a, b, strlen(b)) == 0) return 1;
  return 0;
}


int ReadDelay = 800;
void loop()
{
  String content = "";
  const char separator = ':';

  // READ DATA
  // Serial.print("DHT21, \t");
  int chk = DHT.read21(DHT21_PIN);
  bool debug = false;
  if(debug == true) {
    switch (chk)
    {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    case DHTLIB_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case DHTLIB_ERROR_ACK_L:
      Serial.print("Ack Low error,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      Serial.print("Ack High error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
    }
  }
  // DISPLAY DATA
  // Serial.print(DHT.humidity, 1);
  // Serial.print(",\t\t");
  // Serial.println(DHT.temperature, 1);

  while(Serial.available()) {
#define INPUT_SIZE 100
    const char *request[8];
    char input[INPUT_SIZE + 1];
    byte size = Serial.readBytes(input, INPUT_SIZE);
    // Add the final 0 to end the C string
    input[size] = 0;

    char * cmd;
    cmd = strtok (input," ,.-:");
    int index = -1;
    while (cmd != NULL)
    {
      index++;
      request[index] = cmd;
      cmd = strtok (NULL, " ,.-:");
    }

    //        for (int j = 0; j <= index; j++) {
    //          Serial.print(request[j]);
    //          Serial.print(" - ");
    //        }
    // READ:HUMI:TEMP:HUMI:TEMP:HUMI:TEMP
    if(StartsWith(request[0], "READ")) { 
      for (int j = 1; j <= index; j++) {
        if(StartsWith(request[j], "HUM")) {
          Serial.print( DHT.humidity, 1);
        } 
        else if(StartsWith(request[j], "TEMP")) {
          Serial.print( DHT.temperature, 1);
        } 
        else if(StartsWith(request[j], "DELA")) {
          Serial.print( ReadDelay ,1);
        } 
        else {
          Serial.println("Read ERROR");
          Serial.println(request[j]);
        }
        if (j < index){
          Serial.print( "," );
        }
      }
      Serial.print( "\n" );

    } 
    else if(StartsWith(request[0], "SET")) {
      Serial.println("setting");
      if(index>1){
        if(StartsWith(request[1], "DELA")) {
          ReadDelay = atoi(request[2]);
          Serial.println(atoi(request[2]));
          Serial.println(ReadDelay);
        } 
        else {
          Serial.println("Set ERROR 1");
        }
      } 
      else {
        Serial.println("Set ERROR 2");
      }
    }
  }

  delay(ReadDelay);
}
//
// END OF FILE
//


