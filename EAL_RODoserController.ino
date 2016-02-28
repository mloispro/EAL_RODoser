
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
int _runEverySeconds = 20;

RODoser _doser(aServo, _doserPin, _doserShakes, _mofsetPin, _runEverySeconds, _floatSwitch);

//---End Dosers---

SimpleTimer _timer;

void setup() {
	//SerialExt::Init(); <-have to init serial in sketch

	Serial.begin(9600);
	while (Serial.available() == 0 && millis() < 2000); // wait until Arduino Serial Monitor opens
	RTCExt::Init();

	_timer.setInterval(4000, RunDoser);
	//_timer.setInterval(10000, PrintRuntime);
}

//int _incomingByte = 0;  // a string to hold incoming data from serial com.
void loop() {
	_timer.run();
}

void RunDoser(){
	int incomingByte = SerialExt::Read();
	if (incomingByte > 0){
		//incomingByte = 49; //on input so run.
		SerialExt::Print("Serial Input: ", incomingByte);
	}

	//1 to run, 2 to run demo
	bool runMotor = ServoMotor::ShouldRunMotorBySerialInput(incomingByte);
	bool runMotorDemo = ServoMotor::ShouldRunMotorDemo(incomingByte);

	if (!runMotor && !runMotorDemo){
		
		runMotor = _doser.ShouldRunMotor(true);
		SerialExt::Debug("runMotor: ", runMotor);
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

//void PrintRuntime(){
//	RTCExt::PrintDigitalRuntime();
//}
