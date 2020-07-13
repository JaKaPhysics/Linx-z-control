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

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoNano328.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoNano328* LinxDevice;

// Change these two numbers to the pins connected to your encoder.
	//   Best Performance: both pins have interrupt capability
	//   Good Performance: only the first pin has interrupt capability
	//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2,3);

//Forward declaration
int readRotaryEncoder();

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxArduinoNano328();
  
  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);

  

  //Attach the custom command to the LINX Listener
  //AttachCustomCommand(customCommandNumber, nameOfCustomCommand)
  LinxSerialConnection.AttachCustomCommand(0, readRotaryEncoder);
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

int readRotaryEncoder(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{

	// INIT:
	// long oldPosition  = -999;					oldPosition cannot be initialized with this value every time the function is called, this has to be done in LabVIEW
	
	for(int i=0; i<numInputBytes; i++)
	{
		input[i] = byte(input[i]);
	}

	/* long oldPosition = (long) input;						//oldPosition is turned over by LabVIEW

	long newPosition = oldPosition + 1;
	response[0] = (unsigned char) newPosition;
	*numResponseBytes = sizeof(newPosition); */

		long newPosition = myEnc.read();
		
		response[0] = (unsigned char) newPosition;
		*numResponseBytes = sizeof(newPosition);
	 

	return 0;
}