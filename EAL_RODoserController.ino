

#include <AnalogSwitch.h>
#include <RODoser.h>
#include <Servo.h>

#include <StandardCplusplus.h>
using namespace std;


#include <SerialExt.h>
#include <ServoMotor.h>
using namespace Utils;


//---Dosers---

//vector<RODoser> _dosers;
Servo aServo;
int _doserPin = 4;
int _doserShakes = 2;

RODoser _doser(aServo, _doserPin, _doserShakes);
int _doserPwrSigPin = 12; //relay signal pin.

//---End Dosers---

int _floatSwitchSigPin = 1;	//anolog input
//int _floatSwitchReading;           // the current reading from the input pin


void setup() {
	pinMode(_doserPwrSigPin, OUTPUT); //to send relay signal
	//pinMode(_floatSwitchSigPin, INPUT); //receive switch signal
	// initialize serial:
	Serial.begin(9600);
	//Wait for four seconds or till data is available on serial, whichever occurs first.
	while (Serial.available() == 0 && millis() < 2000);
	//while (!Serial); // Wait until Serial is ready - Leonardo
	Serial.println("Press - 1 to run, 2 to go back and forth..");

}

//int _incomingByte = 0;  // a string to hold incoming data from serial com.
void loop() {

	int incomingByte = SerialExt::Read();
	if (incomingByte == 0){
		incomingByte = 49; //on input so run.
	}
	//_utils.Debug("incomingByte: ", incomingByte);
	bool runMotor = ServoMotor::ShouldRunMotor(incomingByte);
	bool runMotorDemo = ServoMotor::ShouldRunMotorDemo(incomingByte);

	runMotor = AnalogSwitch::IsOn(_floatSwitchSigPin);

	if (runMotor) {
		SerialExt::Print("Signaling Relay ON, pin: ", _doserPwrSigPin);
		digitalWrite(_doserPwrSigPin, LOW); //to send relay signal

		_doser.Dose();
		SerialExt::Print("Signaling Relay OFF, pin: ", _doserPwrSigPin);
		digitalWrite(_doserPwrSigPin, HIGH);
	}
	else if (runMotorDemo)
	{
		vector<RODoser> dosers;
		dosers.push_back(_doser);
		RODoser::RunDemo(dosers);
	}
	delay(1000); //wait for a sec before checking for signal again.
	delay(7000); //for testing
}

//bool IsOn(){
//	//read switch
//	_floatSwitchReading = analogRead(_floatSwitchSigPin);
//	SerialExt::Print("Switch Val: ", _floatSwitchReading);
//	if (_floatSwitchReading > 900){
//		return true;
//	}
//	return false;
//}