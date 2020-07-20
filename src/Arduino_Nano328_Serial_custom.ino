/****************************************************************************************	
**  This is example LINX firmware for use with the Arduino Uno with the serial 
**  interface enabled.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
**  Written By Sam Kristoff
**
**  BSD2 License.
****************************************************************************************/

//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoNano328.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoNano328* LinxDevice;

//Default setup for 2004A
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Change these two numbers to the pins connected to your encoder.
	//   Best Performance: both pins have interrupt capability
	//   Good Performance: only the first pin has interrupt capability
	//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2,3);

//Forward declaration
int readRotaryEncoder();
int writeLCD_line0();
int writeLCD_line1();

long oldPosition = -999;
long newPosition;

//Initialize LINX Device And Listener
void setup()
{
	//Instantiate The LINX Device
	LinxDevice = new LinxArduinoNano328();
	
	//The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
	LinxSerialConnection.Start(LinxDevice, 0);

	//Attach the custom command to the LINX Listener
	//AttachCustomCommand(customCommandNumber, nameOfCustomCommand)
	LinxSerialConnection.AttachCustomCommand(0, readRotaryEncoder);	// custom command 0
	LinxSerialConnection.AttachCustomCommand(1, writeLCD_line0);  	// custom command 1
	LinxSerialConnection.AttachCustomCommand(2, writeLCD_line1);  	// custom command 2

	//Initialization of LCD display
	lcd.init();                               	// LCD dimension: 16 characters, 2 lines
	lcd.clear();                            	// clear display
	lcd.backlight();							// activate backlight
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}


/****************************************************************************************
**  User Functions
****************************************************************************************/

/************************
 ***  Rotary Encoder  ***
************************/

// added by Janosch Kappel

/************************************** Custom command 0 ******************************************/
int readRotaryEncoder(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{	
	for(int i=0; i<numInputBytes; i++)
	{
		input[i] = byte(input[i]);
	}

	long newPosition = myEnc.read();						// read encoder position
	delay(50);												// wait

	/* if (newPosition =! oldPosition)							// check if position changed
	{
		if (newPosition > oldPosition)						// check if step up
		{
			oldPosition = newPosition;
			response[0] = (unsigned char) 2;
		}
		if (newPosition < oldPosition)						// check if step down
		{
			oldPosition = newPosition;
			response[0] = (unsigned char) 1;
		}
	}
	if (newPosition == oldPosition)							
	{
		oldPosition = newPosition;
		response[0] = (unsigned char) 0;
	}
	
	*numResponseBytes = sizeof(response[0]); */
	
	response[0] = (unsigned char) (newPosition / 4);		// divide newPosition by 4 to get a step size of 1 
	*numResponseBytes = sizeof(newPosition);				// sends back the size (in bytes) of the response

	return 0;												// return value is used for error handling
}

/************************
 ****  LCD control  ****
************************/

// added by Janosch Kappel

/************************************** Custom command 1 ******************************************/
int writeLCD_line0(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
	lcd.setCursor (0,0);
    lcd.print("                    ");                  	// clear line 0 (empty space)
    lcd.setCursor (0,0);

	for(int i = 0; i < (numInputBytes); i++)
	{
		input[i] = byte(input[i]);							// writes character after character to LCD
		lcd.write(input[i]);
		response[i]=input[i];
	}

    *numResponseBytes = numInputBytes;                 		// sends back the size (in bytes) of the response
 	return 0;												// return value is used for error handling
}

/************************************** Custom command 2 ******************************************/
int writeLCD_line1(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
    lcd.setCursor (0,1);
    lcd.print("                    ");                    	// clear Line 1 (empty space)
    lcd.setCursor (0,1);

	for(int i = 0; i < (numInputBytes); i++)
	{
		input[i] = byte(input[i]);							// writes character after character to LCD
		lcd.write(input[i]);
		response[i]=input[i];
	}

    *numResponseBytes = numInputBytes;                  	// sends back the size (in bytes) of the response
 	return 0;												// return value is used for error handling
}