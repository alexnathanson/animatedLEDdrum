
int mode = 1; // behaviour of animation
int total = 22;  //total amound of frames in given sequence
int pinout = 0;
int frame = 1; // current frame
int delaytime = 150;  // delay time
int palindrome = 1; //direction for palindrome
int instances =  1;  // total instances of LEDs perceived at a given time.
boolean stringComplete = false;  //whether the string is complete

int totAnimation = 4; // total frames of animation
int centerPin = 1;  //centerPin frame count
int centerOut = 34; //output pin for center animation
boolean flag = true; // center animation palindrome flag

char inByte; // incoming serial byte
String maxInput = "";  //a string to hold incoming data from Max
int getLength = 0;
char pos;
int posint = 0;
int inputVal = 0;
String inputSub ="";
int getInt;
String stringpos;
int trigger = 0;
char passthrough; //pass Serial input to Serial1

boolean triggerMode = false;
boolean triggerIt = false;

/* Serial syntax
    First number
      1) animation mode
      2) delay time
      3) trigger mode
      4) trigger it
    Second number
      data
*/

void setup() {
  
  //serial setup
  Serial.begin(9600);
  //Serial1.begin(9600);
  
  // pinMode settings
  
  //Ring LEDs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  // Eye LEDs
  pinMode(34, OUTPUT);
  pinMode(35, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  
  // transistors for multiplexing
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
}

void loop() {
  
//****************************respond to incoming data**********************************************
if (stringComplete){
       // get 1st value
        pos = maxInput.charAt(0);
        stringpos = String(pos);
        posint = stringpos.toInt();
        posint = posint - 0;
      //  Serial.print("first pos");
      //  Serial.println(posint);
        
         //get 2nd value   
      getLength = maxInput.length();
     // Serial.print("length ");
       //     Serial.println(getLength);

      inputSub = maxInput.substring(1, getLength);  
         //   Serial.println("Sub String " + inputSub);
       inputVal = inputSub.toInt();
         //     Serial.print("Value ");
          //      Serial.println(inputVal);   
         
   // if 1st char of input string is 1 then effect mode
   if (posint == 1){                    
     Serial.print("mode ");
     mode = inputVal;
     Serial.println(mode);
   }
   // if 1st char of input string is 2 then effect delaytime
   else if (posint == 2){   
    Serial.print("delaytime ");  
     delaytime = inputVal; 
   if (delaytime > 500) {
     delaytime = 255;
   }
 else if (delaytime < 10) {
 delaytime  = 10;
 }
     Serial.println(delaytime);
   }
   
   // trigger mode
   else if (posint == 3) {
     Serial.print("trigger ");
     
     trigger = inputVal;
     
     if (trigger == 1) {
       triggerMode = true;
       Serial.println("true");
     }
     else if (trigger == 0) {
       triggerMode = false;
       Serial.println("false");
     }
   }
   
   // trigger it
   else if (posint == 4){
      Serial.println("Next frame!");
      triggerIt = false;
   }
   
   // clear string and reset flag
   maxInput = "";
   stringComplete = false;
}

//****************************modes*******************************

if (triggerIt == false){
  
//palindrome
if (mode == 0) {  
  pinout = frame + 1;
   
   centerPin = centerAnimation(centerPin);
   centerOut = centerPin + 33;
  //Multiplexing
  
  pinout = multiplex(pinout);
   
    lightIt(pinout, centerOut, delaytime);

   
   // count up
   if(palindrome == 1){
   frame++;
   }
   
   // count down
   if (palindrome == 0){
   frame = frame - 1;
   }
   
   // count up when frame hits minimum
   if (frame == 1) {
     palindrome = 1;
   }
   
   // count down when frame hits maximum
   if (frame == total) {
     palindrome = 0;
   }
   
   // print current frame number
  // Serial.println(frame);
  }
  
  //ascending
else if (mode == 1){ 
    pinout = frame + 1;

    centerPin = centerAnimation(centerPin);
    centerOut = centerPin + 33;
   //account for change in pin sequence on Mega
   //this should be a function!!!
   //Multiplexing
  
   pinout = multiplex(pinout);
  
   lightIt(pinout, centerOut, delaytime);
   
   frame++;
   
   if (frame > total){
    frame=1;
  }
  
   // print current frame number
 //  Serial.println(frame);
}

//descending
else if (mode == 2) { 
    pinout = frame + 1;
    
    centerPin = centerAnimation(centerPin);
    centerOut = centerPin + 33;
    
   //Multiplexing
  
   pinout = multiplex(pinout);
     
  lightIt(pinout, centerOut, delaytime);
  
  frame = frame - 1;
  
  if (frame < 1){
    frame = total;
  }
  
   // print current frame number
 //  Serial.println(frame);
}

//random mode
else if (mode == 3) {
   pinout = random(22);
   pinout++;
   
   centerPin = centerAnimation(centerPin);
    centerOut = centerPin + 33;
 
   
   pinout = multiplex(pinout);
 
    lightIt(pinout, centerOut, delaytime);

}
}
  if (triggerMode == true){
  triggerIt = true;
  }
  else if (triggerMode == false) {
    triggerIt = false;
  }
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {  
  
  while (Serial.available()){
    //get new byte
    inByte = Serial.read();   
    // read from port 0, send to port 1
   passthrough = inByte;
  // Serial1.write(passthrough);
   // add it to the inputString;
    maxInput += inByte;
   //Serial.println(maxInput);
    //   maxInput = "2853";
   stringComplete = true;
   }
  }
  
  // Multiplex function - sets transistor values and adjusts pinout value.
  int multiplex(int pinout) {
      if (pinout > 12){
        digitalWrite(24, HIGH);
        digitalWrite(25, LOW);
        pinout = pinout - 11;
        }
      else {
        digitalWrite(24, LOW);
        digitalWrite(25, HIGH);
        }
    
    return pinout;
    }
   
   void lightIt(int pinout, int centerPin, int delaytime){
     
     // Ring LEDs
     digitalWrite(pinout, HIGH);
     // Center LEDs
     digitalWrite(centerPin, HIGH);
     delay(delaytime);
     digitalWrite(pinout, LOW);
     digitalWrite(centerPin, LOW);
     }
     
   int centerAnimation(int currentFrame){
     if (currentFrame == 4){
         flag = false;
      }
     else  if (currentFrame == 1){
       flag = true;
     }
     
     if (flag == true){
       currentFrame++;
     }
     else if (flag == false){
       currentFrame = currentFrame - 1;
       }
       
     return currentFrame;
   }
