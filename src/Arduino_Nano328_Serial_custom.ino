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
#include <DHT.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

//Iclude some c++ libraries
#include <stdio.h>
#include <stdlib.h>

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

// define used sensor type
#define DHTTYPE DHT22

// define used sensor pins
#define ONE_WIRE_BUS 8 			// 18B20 pin
#define DHTPIN 7 				// DHT22 pin

OneWire oneWire(ONE_WIRE_BUS);
DHT dht(DHTPIN, DHTTYPE);

DallasTemperature sensors(&oneWire);

//Forward declaration
int readRotaryEncoder();
int writeLCD_line0();
int writeLCD_line1();
int read_temp();
int read_humidity();

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
	LinxSerialConnection.AttachCustomCommand(3, read_temp); 		// custom command 3
	LinxSerialConnection.AttachCustomCommand(4, read_humidity); 	// custom command 4

	//Initialization of LCD display
	lcd.init();                               	// LCD dimension: 16 characters, 2 lines
	lcd.clear();                            	// clear display
	lcd.backlight();							// activate backlight

	//Initialization of sensors
	sensors.begin();							// 18B20	
 	dht.begin(); 								// DHT22
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

	float newPos = (newPosition / 4);

	*numResponseBytes = sizeof(float);
	memcpy(response, &newPos, sizeof(float));				// copy newPos byte-wise to response

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

/************************
 ***  Sensor readout  ***
************************/

// added by Janosch Kappel

/************************************** Custom command 3 ******************************************/
int read_temp(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
	for(int i=0; i<numInputBytes; i++)
	{
		input[i] = byte(input[i]);
	}

	sensors.requestTemperatures();
	float temp = sensors.getTempCByIndex(0);			// read temperature from 18B20

	int res_inverse  = 8 ; 								// Multiply by 8 to get an int with a resolution of 0.125 
	signed int int_to_send = temp * res_inverse ; 		// with input_value  =  -55.625 and res_inverse = 8, this is -445, or in bits: '1111 1110'  '0100 0011' => High byte: '254', low byte '67' 
	unsigned int value = int_to_send;    				// Make the value an unsigned integer, to shift 0 in from the left, instead of ones.

	*numResponseBytes = 3;
	response[0] = (value & 0xFF00) >> 8;   //mask all but MSB, shift 8 bits to the right; // to test: try B10101010 ; // default: MSB
	response[1] = (value & 0x00FF);        //mask top bits, no need to shift; // to test: try B11001100 ; // default: LSB
	response[2] = byte (res_inverse & 0x00FF);

	// memcpy(response, &temp, sizeof(double));
	// *numResponseBytes = sizeof(double);
}

/************************************** Custom command 4 ******************************************/
int read_humidity(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
	for(int i=0; i<numInputBytes; i++)
	{
		input[i] = byte(input[i]);
	}

	float humidity = dht.readHumidity();				// read humidity from DHT22

	memcpy(response, &humidity, sizeof(float));
	*numResponseBytes = sizeof(float);
}