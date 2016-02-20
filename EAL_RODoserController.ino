


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
int _doserPin = 9;
int _doserShakes = 2;

RODoser _doser(aServo, _doserPin, _doserShakes);
//int _doserPwrSigPin = 6; //relay signal pin.

//---End Dosers---

int _floatSwitchSigPin = 1;	//anolog input
int _floatSwitchMax = 1023;
//int _floatSwitchReading;           // the current reading from the input pin


void setup() {

	pinMode(13, OUTPUT);//led
	pinMode(_floatSwitchSigPin, INPUT); //receive switch signal
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

	runMotor = AnalogSwitch::IsOn(_floatSwitchSigPin, _floatSwitchMax);

	if (runMotor) {
		digitalWrite(13, HIGH);

		_doser.Dose();
		digitalWrite(13, LOW);

		SerialExt::Print("Dose complete wiating 6 hours for tank to fill.");
		delay(21600000); //waiting 6 hours for tank to fill
	}
	else if (runMotorDemo)
	{
		vector<RODoser> dosers;
		dosers.push_back(_doser);
		RODoser::RunDemo(dosers);
	}
	delay(1000); //wait for a sec before checking for signal again.
	//delay(2000); //for testing
}
