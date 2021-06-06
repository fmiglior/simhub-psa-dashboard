#ifndef __SHCUSTOMPROTOCOL_H__
#define __SHCUSTOMPROTOCOL_H__

#include <Arduino.h>
#include <SPI.h>



class SHCustomProtocol {
private:

public:

	/*
	CUSTOM PROTOCOL CLASS
	SEE https://github.com/zegreatclan/SimHub/wiki/Custom-Arduino-hardware-support

	GENERAL RULES :
		- ALWAYS BACKUP THIS FILE, reinstalling/updating SimHub would overwrite it with the default version.
		- Read data AS FAST AS POSSIBLE in the read function
		- NEVER block the arduino (using delay for instance)
		- Make sure the data read in "read()" function READS ALL THE DATA from the serial port matching the custom protocol definition
		- Idle function is called hundreds of times per second, never use it for slow code, arduino performances would fall
		- If you use library suspending interrupts make sure to use it only in the "read" function when ALL data has been read from the serial port.
			It is the only interrupt safe place

	COMMON FUNCTIONS :
		- FlowSerialReadStringUntil('\n')
			Read the incoming data up to the end (\n) won't be included
		- FlowSerialReadStringUntil(';')
			Read the incoming data up to the separator (;) separator won't be included
		- FlowSerialDebugPrintLn(string)
			Send a debug message to simhub which will display in the log panel and log file (only use it when debugging, it would slow down arduino in run conditions)

	*/

	// Called when starting the arduino (setup method in main sketch)
	void setup() {

  //mcp2515 infos
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); //mcp2515 clock
  mcp2515.setNormalMode();


  carregaCAN(); //load can msg pack on can.h
	}

	// Called when new data is coming from computer
	void read() {
		// EXAMPLE 1 - read the whole message and sent it back to simhub as debug message
		// Protocol formula can be set in simhub to anything, it will just echo it
		// -------------------------------------------------------
	//	String message = FlowSerialReadStringUntil('\n');
	//	FlowSerialDebugPrintLn("Message received : " + message);
	
		/*
		// -------------------------------------------------------
		// EXAMPLE 2 - reads speed and gear from the message
		// Protocol formula must be set in simhub to
		// format([DataCorePlugin.GameData.NewData.SpeedKmh],'0') + ';' + isnull([DataCorePlugin.GameData.NewData.Gear],'N')
		// -------------------------------------------------------
*/

 //read the simhub protocol and store on variables
 //use this protocol: format([SpeedKmh],0)+';'+format([Rpms],0)+';'+format([DataCorePlugin.Computed.Fuel_Percent],0)+';'+ format([WaterTemperature],0)+';'+ [Handbrake] +';' + [TurnIndicatorLeft] +';'+ [TurnIndicatorRight]  +';'+ [TCActive] +';' + [ABSActive]
		
		
		int vel = (FlowSerialReadStringUntil(';').toInt())*0.390909; //adjust the number to match to your dashboard
		int rpm = (FlowSerialReadStringUntil(';').toInt())*0.031286; //adjust the number to match to your dashboard
   int fuel = (FlowSerialReadStringUntil(';').toInt())*1.28;
    int wtemp = (FlowSerialReadStringUntil(';').toInt()) *1.28 ;
    int handbrake = (FlowSerialReadStringUntil(';').toInt()) ;
    int leftblink = (FlowSerialReadStringUntil(';').toInt()) ; 
    int rightblink = (FlowSerialReadStringUntil(';').toInt()) ;
   int esp = (FlowSerialReadStringUntil(';').toInt());
   int babs= (FlowSerialReadStringUntil('\n').toInt()); 

//rpm
 canMsg3.data[0] = (rpm & 0xFF);
//speed
canMsg3.data[2] = (vel & 0xFF);
//blinks 

canMsg4.data[4] = ((leftblink*2)+(rightblink*4)& 0xFF);
//fuel
canMsg6.data[3] = (fuel & 0xff);
//water temp
canMsg1.data[1] = (wtemp &0xff) ;
//tc and abs
canMsg5.data[3] =  ((esp*16) + (babs*32)& 0xff);
//park brake
 canMsg5.data[0] = ((handbrake*4)&0xFF);


	}

	// Called once per arduino loop, timing can't be predicted, 
	// but it's called between each command sent to the arduino
void loop() {

//send can messages to the dashboard
mcp2515.sendMessage(&canMsg1);  
mcp2515.sendMessage(&canMsg2);
mcp2515.sendMessage(&canMsg3);
mcp2515.sendMessage(&canMsg4);
mcp2515.sendMessage(&canMsg5);
mcp2515.sendMessage(&canMsg6);




	}

	// Called once between each byte read on arduino,
	// THIS IS A CRITICAL PATH :
	// AVOID ANY TIME CONSUMING ROUTINES !!!
	// PREFER READ OR LOOP METHOS AS MUCH AS POSSIBLE
	// AVOID ANY INTERRUPTS DISABLE (serial data would be lost!!!)
	void idle() {
	}
};

#endif
