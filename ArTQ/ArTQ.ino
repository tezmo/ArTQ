int CodeIn;// used on all serial reads
int KpinNo; 
int Koutpin;
int leftThrottle = A0;    // linear pot
int rightThrottle = A1;   // linear pot
int airBrake = A2;        // linear pot
int Flaps = A3;           // linear pot

String KoldpinStateSTR, KpinStateSTR, Kstringnewstate,Kstringoldstate;
String altit, altitold, vertsp, vertspold, hdgset, hdgsetold, flaps, flapsold;
void setup() 
{
  Kstringoldstate = "111111111111111111111111111111111111111111111111111111111111111111111";
  for (int KoutPin = 22; KoutPin < 70; KoutPin++)// Get these pins ready as inputs  
  {
    pinMode(KoutPin, INPUT);
    digitalWrite(KoutPin, HIGH);  
  }
 Serial.begin(115200);   
}

void loop() {
  {INPUTS();} //Check the "Inputs" section
  if (Serial.available()) {
    CodeIn = getChar();
    if (CodeIn == '=') {EQUALS();} // The first identifier is "="
    if (CodeIn == '<') {LESSTHAN();}// The first identifier is "<"
    if (CodeIn == '?') {QUESTION();}// The first identifier is "?"
    if (CodeIn == '/') {SLASH();}// The first identifier is "/" (Annunciators)
  }//end of serial read bit
}//end of void loop

char getChar()// Get a character from the serial buffer
{
  while(Serial.available() == 0);// wait for data
  return((char)Serial.read());// Thanks Doug
}

void EQUALS(){
  CodeIn = getChar(); // Get the second identifier
  switch(CodeIn) {// Now lets find what to do with it
   case 'b': // found AP altitude setting
      delay (11);
      altit = "";
      altit += getChar();
      altit += getChar();
      altit += getChar();
      altit += getChar();
      altit += getChar();
      if (altit != altitold){
        altitold = altit;
      }        
    break;
   case 'c': // found vertical speed setting
      delay (11);
      vertsp = "";
      vertsp += getChar();
      vertsp += getChar();
      vertsp += getChar();
      vertsp += getChar();
      vertsp += getChar();
      if (vertsp != vertspold){
        vertspold = vertsp;
       }
      break;
   case 'd': // found AP heading setting
      delay (11);
      hdgset = "";
      hdgset += getChar();
      hdgset += getChar();
      hdgset += getChar();
      if (hdgset != hdgsetold){
        hdgsetold = hdgset;
       }       
   break;
    }//end of case switch
  }// end of void equals

void LESSTHAN(){    // The first identifier was "<"
CodeIn = getChar(); // Get another character
  switch(CodeIn) {// Now lets find what to do with it
    case 'A'://Found the second identifier
       //Do something
    break;
     
    case 'B':
       //Do something
    break;
     
    case 'f':
       // Do something
      case 'G'://Found the second identifier ("G" Flaps position)
      flaps = "";
      flaps += getChar();
      flaps += getChar();
      flaps += getChar();
      if (flaps != flapsold){
       flapsold = flaps; 
      }
      break; 
      }//end of case method  
   }//end of lessthan

void QUESTION(){    // The first identifier was "?"
//do something
}// end of question

void SLASH(){    // The first identifier was "/" (Annunciator)
  //Do something
}

void INPUTS() 
{
  int ThrottleLeftValue = map(analogRead(leftThrottle), 0, 1023, 0, 255);
  int ThrottleRightValue = map(analogRead(leftThrottle), 0, 1023, 0, 255);
  
  Kstringnewstate = "";
  for (int KpinNo = 22; KpinNo < 70; KpinNo++){// Pins 22 thru to 69 are all inputs
    KpinStateSTR = String(digitalRead(KpinNo)); 
    KoldpinStateSTR = String(Kstringoldstate.charAt(KpinNo - 22));//the 22 needs to be the same as the 22 in the "for" loop line.
    if (KpinStateSTR != KoldpinStateSTR)
    {
      if (KpinNo == 22){if (KpinStateSTR == "1" ){Serial.println ("C410");}else {Serial.println ("C411");}}//Nav lights
      if (KpinNo == 23){if (KpinStateSTR == "1" ){Serial.println ("C420");}else {Serial.println ("C421");}}//Beacon lights
      if (KpinNo == 24){if (KpinStateSTR == "1" ){Serial.println ("C430");}else {Serial.println ("C431");}}//Landing lights
      if (KpinNo == 25){if (KpinStateSTR == "1" ){Serial.println ("C440");}else {Serial.println ("C441");}}//Taxi lights
      if (KpinNo == 26){if (KpinStateSTR == "1" ){Serial.println ("C450");}else {Serial.println ("C451");}}//Strobe lights
      if (KpinNo == 27){if (KpinStateSTR == "1" ){Serial.println ("C460");}else {Serial.println ("C461");}}//Panel lights
      if (KpinNo == 28){if (KpinStateSTR == "1" ){Serial.println ("C470");}else {Serial.println ("C471");}}//Recognitian lights
      if (KpinNo == 29){if (KpinStateSTR == "1" ){Serial.println ("C480");}else {Serial.println ("C481");}}//Wing lights
      if (KpinNo == 30){if (KpinStateSTR == "1" ){Serial.println ("C490");}else {Serial.println ("C491");}}//Logo lights
      if (KpinNo == 31){if (KpinStateSTR == "1" ){Serial.println ("C500");}else {Serial.println ("C501");}}//Cabin lights
      if (KpinNo == 32 & KpinStateSTR == "1" ){Serial.println ("B11");}//Autopilot altitude set
      if (KpinNo == 33 & KpinStateSTR == "1" ){Serial.println ("B12");}//Autopilot altitude set
      if (KpinNo == 34 & KpinStateSTR == "1" ){Serial.println ("B13");}//Autopilot vert speed set
      if (KpinNo == 35 & KpinStateSTR == "1" ){Serial.println ("B14");}//Autopilot vert speed set
      // add more direct inputs here but raise the pinNo (35) in the above live and the line below.
      //now the "keys" bit. Note the pinNo 35 in the line above and the line below.
      if (KpinNo > 35){
      Serial.print ("D"); 
      if (KpinNo < 10) Serial.print ("0");
      Serial.print (KpinNo);
      Serial.println (KpinStateSTR);
      }//end of "keys" bit
    }//end of yes it's different
    Kstringnewstate += KpinStateSTR;
  }//end of "for" loop
  Kstringoldstate = Kstringnewstate;
}//end of OUTPUTS void









