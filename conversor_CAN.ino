# C-digo-conversor-CAN
// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

int sensorPin[6] = {A0,A1,A2,A3,A4,A5};
int sensorValue[6] = {0,0,0,0,0,0};
int cantxValue[6] = {0,0,0,0,0,0};
int resto[6] = {0,0,0,0,0,0};
int restoGeral1 = 0;
int restoGeral2 = 0;

void setup()
{
    Serial.begin(115200);

START_INIT:

    if(CAN_OK == CAN.begin(CAN_125KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}
int i = 0;

void loop()
{      
  for ( i =0; i < 6; i++ )
    {  
        sensorValue[i] = analogRead(sensorPin[i]);      
        cantxValue[i] = sensorValue[i] / 4;
        resto[i] = sensorValue[i] - cantxValue[i]*4;
        
    }

    restoGeral1 = resto[0] + resto[1]*4 + resto[2]*8;    
    restoGeral2 = resto[3] + resto[4]*4 + resto[5]*8;
    unsigned char canMsg[8] = {cantxValue[0], cantxValue[1], cantxValue[2], cantxValue[3], cantxValue[4], cantxValue[5], restoGeral1, restoGeral2};
    CAN.sendMsgBuf(0x00, 0, 8, canMsg);
          
    delay(100);                       // send data per 100ms    
}
