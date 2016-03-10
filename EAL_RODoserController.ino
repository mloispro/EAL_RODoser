
//#include <TimeLib.h>
//#include <Time.h>
//#include <Wire.h>
#include <SimpleTimer.h>

#include <AnalogSwitch.h>
#include <RODoser.h>
#include <Servo.h>

#include <StandardCplusplus.h>
using namespace std;

#include <SerialExt.h>
#include <ServoMotor.h>
//#include <DigitalTime.h>
#include <RTCExt.h>
#include <PinValMemoryExt.h>
using namespace Utils;


// pin listing
int _doserPin = 9;
int _mofsetPin = 8;
int _floatSwitchSigPin = 0;	//anolog input

//---Dosers---
int _floatSwitchMax = 1023;

//vector<RODoser> _dosers;
Servo aServo;
AnalogSwitch _floatSwitch(_floatSwitchSigPin);
int _doserShakes = 2;
//long _runEverySeconds = 18000; //<- dont use multiplication will get an overflow
long _runEverySeconds = 30; //for testing


RODoser _doser;

//---End Dosers---

SimpleTimer _timer;

void setup() {
	//SerialExt::Init(); <-have to init serial in sketch

	Serial.begin(9600);
	while (Serial.available() == 0 && millis() < 2000); // wait until Arduino Serial Monitor opens
	//RTCExt::Init();
	_doser = RODoser(aServo, _doserPin, _doserShakes, _mofsetPin, _runEverySeconds, _floatSwitch);
	_doser.PrintSerialInstructions();

	_timer.setInterval(4000, RunDoser);
	//_timer.setInterval(10000, PrintRuntime);
	//SetRTCTime();
}

//int _incomingByte = 0;  // a string to hold incoming data from serial com.
void loop() {
	_timer.run();
}

void RunDoser(){
	//SerialExt::Debug("_runEverySeconds ",_runEverySeconds);
	
	int incomingNum = SerialExt::Read();
	if (incomingNum > 0){
		bool processed = _doser.ProcessSerialInput(incomingNum);
	}

	//1 to run, 2 to run demo
	bool runMotor = ServoMotor::ShouldRunMotorBySerialInput(incomingNum);
	bool runMotorDemo = ServoMotor::ShouldRunMotorDemo(incomingNum);

	if (!runMotor && !runMotorDemo){
		
		runMotor = _doser.ShouldRunMotor(true);
		//SerialExt::Debug("runMotor: ", runMotor);
	}

	if (runMotor) {
		_doser.Dose();
	}
	else if (runMotorDemo)
	{
		vector<RODoser> dosers;
		dosers.push_back(_doser);
		RODoser::RunDemo(dosers);
	}
}


void SetRTCTime(){
	RTCExt::SetRTCTime(8, 59, 0, 9, 3, 2016);
}
//void PrintRuntime(){
//	RTCExt::PrintDigitalRuntime();
//}
