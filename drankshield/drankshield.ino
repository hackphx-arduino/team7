//Make sure the dip switches are turned ON, and none of your shields are using pins A0,A1,A2,A3 or D4

#include <Shieldbot.h>	//includes the Shieldbot Library
#include <Servo.h>

Shieldbot shieldbot = Shieldbot();	//decares a Shieldbot object

Servo myservo;
int servopin = 12;

int switch1pin = 11;
int switch2pin = 12;
int switch3pin = 13;

int switch1state = 0;
int switch2state = 0;
int switch3state = 0;

// Bot sensors
int S1,S2,S3,S4,S5;	//values to store state of sensors

// Bot speed controls
int fwdhi = 127;
int bwdhi = -128;
int fwdlo = 64;
int bwdlo = -64;
int fwdoff = 0;

// servo position
int pos = 0;
int posMin = 0;
int posMax = 180;
int posSpeed = 5;

// wait = 0, fill-up = 1-3, deliver = 4
int fillState = 0;

void setup(){
	Serial.begin(9600);//Begin serial comm for debugging
	shieldbot.setMaxSpeed(50,50);//255 is max, if one motor is faster than another, adjust values
/*
	myservo.attach(servopin);
	myservo.write(posMin);
*/
	pinMode(switch1pin, INPUT);
	pinMode(switch2pin, INPUT);
	pinMode(switch3pin, INPUT);

}

void loop(){
	/*
	debugloop();
	driveBot();
	*/

	switch (fillState) {
		case 0:
			// Waiting for button push, is it pushed?
			getSwitches();
			if (switch1state == HIGH || switch2state == HIGH || switch3state == HIGH) {
				fillState = 1;
				shieldbot.drive(fwdhi,fwdhi);
				delay(200); // Get off of the line
			}
			break;
		default:
			// Fill or deliver
			driveBot();
			break;
	}

}

void debugloop() {
	//Read all the sensors
	S1 = shieldbot.readS1();
	S2 = shieldbot.readS2();
	S3 = shieldbot.readS3();
	S4 = shieldbot.readS4();
	S5 = shieldbot.readS5();

	switch1state = digitalRead(switch1pin);
	switch2state = digitalRead(switch2pin);
	switch3state = digitalRead(switch3pin);

	//Print the status of each sensor to the Serial console
	Serial.print(" S1: ");
	Serial.print(S1);
	Serial.print(" S2: ");
	Serial.print(S2);
	Serial.print(" S3: ");
	Serial.print(S3);
	Serial.print(" S4: ");
	Serial.print(S4);
	Serial.print(" S5: ");
	Serial.print(S5);
	Serial.print(" sw1 ");
	Serial.print(switch1state);
	Serial.print(" sw2 ");
	Serial.print(switch2state);
	Serial.print(" sw3 ");
	Serial.print(switch3state);
	Serial.println();
/*
	for(pos = posMin; pos < posMax; pos += posSpeed) {
		analogWrite(servopin, pos);
		delay(15);
	}
*/
}

void getSwitches() {
	switch1state = digitalRead(switch1pin);
	Serial.print("sw1 ");
	Serial.print(switch1state);
	switch2state = digitalRead(switch2pin);
	Serial.print(" sw2 ");
	Serial.print(switch2state);
	switch3state = digitalRead(switch3pin);
	Serial.print(" sw3 ");
	Serial.print(switch3state);
	Serial.println();
}

void driveBot() {

	//Read all the sensors
	S1 = shieldbot.readS1();
	S2 = shieldbot.readS2();
	S3 = shieldbot.readS3();
	S4 = shieldbot.readS4();
	S5 = shieldbot.readS5();

	//Print the status of each sensor to the Serial console
	Serial.print(" S1: ");
	Serial.print(S1);
	Serial.print(" S2: ");
	Serial.print(S2);
	Serial.print(" S3: ");
	Serial.print(S3);
	Serial.print(" S4: ");
	Serial.print(S4);
	Serial.print(" S5: ");
	Serial.print(S5);
	Serial.print(" sw1 ");
	Serial.print(switch1state);
	Serial.print(" sw2 ");
	Serial.print(switch2state);
	Serial.print(" sw3 ");
	Serial.print(switch3state);
	Serial.print(" ");

	//Note about IR sensors
	//if a sensor sees HIGH, it means that it just sees a reflective surface background(ex. whie)
	//if a sensor sees LOW, it means that it sees a non-reflective surface or empty space (ex. black tape line, or empty space off ledge)

	if (S3 == LOW && S2 == LOW && S4 == LOW && S1 == LOW && S5 == LOW) {
		// get drink
		shieldbot.stop();
		fillOrDeliver();
	} else if (S3 == LOW && S4 == LOW && S2 == LOW) {
		// forward slower
		Serial.println("forward low");
		shieldbot.drive(fwdlo,fwdlo);
	} else if (S3 == LOW && S2 == LOW) {
		// lean right
		Serial.println("lean right");
		shieldbot.drive(fwdhi,fwdlo);
	} else if (S3 == LOW && S4 == LOW) {
		// lean left
		Serial.println("lean left");
		shieldbot.drive(fwdlo,fwdhi);
	} else if (S3 == LOW) {
		// go foward
		Serial.println("forward");
		shieldbot.drive(fwdhi,fwdhi);
	} else if (S2 == LOW) {
		// right low
		Serial.println("right low");
		shieldbot.drive(fwdhi,fwdoff);
		delay(100);
	} else if (S4 == LOW) {
		// left low
		Serial.println("left low");
		shieldbot.drive(fwdoff,fwdhi);
		delay(100);
	} else if (S1 == LOW) {
		// right hard
		Serial.println("left high");
		shieldbot.drive(fwdhi,bwdhi);
		delay(100);
	} else if (S5 == LOW) {
		// left hard
		Serial.println("left high");
		shieldbot.drive(bwdhi,fwdhi);
		delay(100);
	} else {
		// find the line
		Serial.println("find the line");
		shieldbot.drive(fwdhi,fwdhi);
	}
}

int doTheFill = 0;
void shouldFill() {
	doTheFill = 0;
	if (fillState == 1) {
		doTheFill = switch1state;
	} else if (fillState == 2) {
		doTheFill = switch2state;
	} else if (fillState == 3) {
		doTheFill = switch3state;
	}
}

void fillOrDeliver() {
	if (fillState < 4) {
		// fill
		fillState++;

		shouldFill();
		if (doTheFill == 1) {
			Serial.print("fill ");
			Serial.println(fillState);

			// push the fill button
			for(pos = posMin; pos < posMax; pos += posSpeed) {
				myservo.write(pos);
				delay(15);
			}
			delay(15);
			for(pos = posMax; pos>=posMin; pos-=posSpeed) {
				myservo.write(pos);
				delay(15);
			}
			delay(2500); // Wait for fill-up
		} else {
			Serial.print("no need to fill ");
			Serial.println(fillState);
		}

		shieldbot.drive(fwdhi,fwdhi);
		delay(100); // Get off of the line
	} else {
		// delivered
		fillState = 0;
		Serial.println("delivered");
	}
}

