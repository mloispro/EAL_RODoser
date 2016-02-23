


#include <TimerExt.h>
#include <SimpleTimer.h>

#include <AnalogSwitch.h>
#include <RODoser.h>
#include <Servo.h>

#include <StandardCplusplus.h>
using namespace std;


#include <SerialExt.h>
#include <ServoMotor.h>
using namespace Utils;


// pin listing
int _doserPin = 9;
int _mofsetPin = 8;
int _floatSwitchSigPin = 0;	//anolog input

//---Dosers---

//vector<RODoser> _dosers;
Servo aServo;
int _doserShakes = 2;

RODoser _doser(aServo, _doserPin, _doserShakes);
//int _doserPwrSigPin = 6; //relay signal pin.

//---End Dosers---

int _floatSwitchMax = 1023;
//int _floatSwitchReading;           // the current reading from the input pin

SimpleTimer _timer;
SimpleTimer _timer2;

void setup() {

	pinMode(_mofsetPin, OUTPUT);
	pinMode(_floatSwitchSigPin, INPUT); //receive switch signal
	// initialize serial:
	Serial.begin(9600);
	//Wait for four seconds or till data is available on serial, whichever occurs first.
	while (Serial.available() == 0 && millis() < 2000);
	//while (!Serial); // Wait until Serial is ready - Leonardo
	Serial.println("Press - 1 to run, 2 to go back and forth..");

	//timer
	//_timer.setTimeout(3000, Prime);
	//_timer2.setInterval(30 * 1000, Prime);
	_timer.setInterval(4000, RunDoser);
	_timer.setInterval(10000, TimerExt::DigitalClockDisplay);

}

//int _incomingByte = 0;  // a string to hold incoming data from serial com.
void loop() {
	_timer.run();
}

void RunDoser(){
	int incomingByte = SerialExt::Read();
	if (incomingByte == 0){
		incomingByte = 49; //on input so run.
	}
	//_utils.Debug("incomingByte: ", incomingByte);
	bool runMotor = ServoMotor::ShouldRunMotor(incomingByte);
	bool runMotorDemo = ServoMotor::ShouldRunMotorDemo(incomingByte);

	runMotor = AnalogSwitch::IsOn(_floatSwitchSigPin, _floatSwitchMax);

	if (runMotor) {
		digitalWrite(_mofsetPin, HIGH);
		
		_doser.Dose();
		digitalWrite(_mofsetPin, LOW);
		SerialExt::Print("Dose complete wiating 6 hours for tank to fill.");
		//todo: add timer -> delay(21600000); //waiting 6 hours for tank to fill
	}
	else if (runMotorDemo)
	{
		vector<RODoser> dosers;
		dosers.push_back(_doser);
		RODoser::RunDemo(dosers);
	}
	//delay(1000); //wait for a sec before checking for signal again.
	//delay(2000); //for testing
}

