/*
Remember that you need to use the HID.cpp and USBAPI.h in the arduino core before this works..
*/

JoyState_t JoySt;


//axis
const int throttleLeft = A0;
const int throttleRight = A1;
const int SpeedBrake = A2;
const int flaps = A3;


//buttons
const int RevThrottleL = 0;
const int RevThrottleR = 1;
const int AtDisengangeLeft = 2;
const int AtDisengageRight = 3;
const int blackButtonLeft = 4;
const int blackButtonRight = 5;
const int CutOffLeft = 6;
const int CutOffRight = 7;
const int ParkingBrake = 8;
const int ManElecCutOff = 9;
const int AutoPilotCutOff = 10;

void setup()
{
  pinMode(13, OUTPUT);

  JoySt.throttleLeft = 0;
  JoySt.throttleRight = 0;

  JoySt.speedBrake = 0;
  JoySt.flaps = 0;

  JoySt.xAxis = 0;
  JoySt.yAxis = 0;
  
  JoySt.rudder = 0;
  JoySt.yaw = 0;
 
 
  //8 positions, 0 is top then turn clockwise going up, 8 is center
  JoySt.hatSw1 = 0;
  JoySt.hatSw2 = 0;
  
  //32 buttons
  JoySt.buttons = 0;


 // set button pins as input with pullup enabled
 pinMode(RevThrottleL, INPUT_PULLUP);
 pinMode(RevThrottleR, INPUT_PULLUP);
 pinMode(AtDisengangeLeft, INPUT_PULLUP);
 pinMode(AtDisengageRight, INPUT_PULLUP);
 pinMode(blackButtonLeft, INPUT_PULLUP);
 pinMode(blackButtonRight, INPUT_PULLUP);
 pinMode(CutOffLeft, INPUT_PULLUP);
 pinMode(CutOffRight, INPUT_PULLUP);
 pinMode(ParkingBrake, INPUT_PULLUP);
 pinMode(ManElecCutOff, INPUT_PULLUP);
 pinMode(AutoPilotCutOff, INPUT_PULLUP);
}


void loop() {
  
  constrain(throttleLeft, 965, 18);
  JoySt.throttleLeft = map(analogRead(throttleLeft), 965, 18, 255, 0);

  constrain(throttleRight, 15, 998);
  JoySt.throttleRight = map(analogRead(throttleRight), 15, 998, 255, 0);

  constrain(SpeedBrake, 0, 270);
  JoySt.speedBrake = map(analogRead(SpeedBrake), 0, 270, 255, 0);

  constrain(flaps, 0, 270);
  JoySt.flaps = map(analogRead(flaps), 0, 1023, 255, 0);
 
  //Check the flaps & all buttons.
  checkInputs(); 

  // Send JoyStick info over to PC
  Joystick.setState(&JoySt);
  ReleaseKey();
  delay(5);
}


// Release buttons
void ReleaseKey() {
  JoySt.buttons = JoySt.buttons & 0;
}


void checkInputs() {

  //  Check buttons
  if (digitalRead(RevThrottleL) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 1;
  }
  if (digitalRead(RevThrottleR) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 2;
  }
  if (digitalRead(AtDisengangeLeft) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 4;
  }
  if (digitalRead(AtDisengageRight) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 8;
  }
  if (digitalRead(blackButtonLeft) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 16;
  }
  if (digitalRead(blackButtonRight) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 32;
  }
  if (digitalRead(CutOffLeft) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 64;
  }
  if (digitalRead(CutOffRight) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 128;
  }
  if (digitalRead(ParkingBrake) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 256;
  }
  if (digitalRead(ManElecCutOff) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 512;
  }
  if (digitalRead(AutoPilotCutOff) == LOW){ //pressed
    JoySt.buttons = JoySt.buttons | 1024;
  }
}
