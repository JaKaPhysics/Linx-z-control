/****************************************************************************************  
**  Created by: S.Stock
**  Published: 16.11.2018
**
**  Youtube Channel: www.youtube.com/c/sstock
**  
*****************************************************************************************
**  LCD with Arduino Nano using LabVIEW LINX Custom Commands VI's 
**  Pin Layout: 5V->VCC, GND->GND, A4->SDA, A5->SCL
****************************************************************************************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LinxArduinoNano328.h>
#include <LinxSerialListener.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);    //Default setup for 2004A
LinxArduinoNano328* LinxDevice;                                   //LinxDevice => Arduino Nano

int myLCD_Line0();
int myLCD_Line1();
int myLCD_Line2();
int myLCD_Line3();

/****************************************************************************************/
void setup()
{
  lcd.begin(20,4);                                              //LCD dimension
  lcd.clear();                                                  //Clear display
  
  LinxSerialConnection.AttachCustomCommand(0, myLCD_Line0);     //Custom command 0
  LinxSerialConnection.AttachCustomCommand(1, myLCD_Line1);     //Custom command 1
  LinxSerialConnection.AttachCustomCommand(2, myLCD_Line2);     //Custom command 2
  LinxSerialConnection.AttachCustomCommand(3, myLCD_Line3);     //Custom command 3
  
  LinxDevice = new LinxArduinoNano328();                        //LinxDevice => Arduino Nano
  LinxSerialConnection.Start(LinxDevice, 0);  
}

/****************************************************************************************/
void loop()
{
  LinxSerialConnection.CheckForCommands();
}

/**************************************Custom command 0******************************************/
int myLCD_Line0(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
    
    lcd.setCursor (0,0);
    lcd.print("                    ");                          //Clear Line 0 (empty space)
    lcd.setCursor (0,0);
for(int i = 0; i < (numInputBytes); i++)
  {
    input[i] = byte(input[i]);
    lcd.write(input[i]);
    response[i]=input[i];
  }
    *numResponseBytes = numInputBytes;                          //Sends bytes in return (used for error handling).
 return 0;
}
/**************************************Custom command 1******************************************/

int myLCD_Line1(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
   
    lcd.setCursor (0,1);
    lcd.print("                    ");                          //Clear Line 1 (empty space)
    lcd.setCursor (0,1);
for(int i = 0; i < (numInputBytes); i++)
  {
    input[i] = byte(input[i]);
    lcd.write(input[i]);
    response[i]=input[i];
  }
    *numResponseBytes = numInputBytes;                          //Sends bytes in return (used for error handling).
 return 0;
}
/**************************************Custom command 2******************************************/

int myLCD_Line2(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
    
    lcd.setCursor (0,2);
    lcd.print("                    ");                          //Clear Line 2 (empty space) 
    lcd.setCursor (0,2);
for(int i = 0; i < (numInputBytes); i++)
  {
    input[i] = byte(input[i]);
    lcd.write(input[i]);
    response[i]=input[i];
  }
    *numResponseBytes = numInputBytes;                          //Sends bytes in return (used for error handling).
 return 0;
}
/**************************************Custom command 3******************************************/

int myLCD_Line3(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
    
    lcd.setCursor (0,3);
    lcd.print("                    ");                         //Clear Line 3 (empty space)
    lcd.setCursor (0,3);
for(int i = 0; i < (numInputBytes); i++)
  {
    input[i] = byte(input[i]);
    lcd.write(input[i]);
    response[i]=input[i];
  }
    *numResponseBytes = numInputBytes;                        //Sends bytes in return (used for error handling).
 return 0;
}

/***************************************End of code***********************************************/
