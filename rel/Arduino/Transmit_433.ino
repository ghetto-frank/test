
// Transmit_433 Program.
// -------
// simple program that send sends 24 bit, 433mhz decimal codes that is passed by
// a 115200 baud rate serial connection. example serial session:
// #SEND:
// send:0000000
// #RECV:
// =!v!=Sent: 0000000
// =!=done=!=
// (not including \n newlines)
// -------
// clear plans to incorprate IR Tranmiter support as mays well as not had to implemnt in the python script.
//IR LIB USED URL: https://github.com/z3t0/Arduino-IRremote NOTE: The IR LED Uses PWM pin 3.


#include <RCSwitch.h> // for 433mhz transmiter on pin 10.
#include <IRremote.h> // for ir led on pin 3.


// -----------------------


IRsend irsend;
RCSwitch mySwitch = RCSwitch();

// ------------------------------------------------
void setup() {
    Serial.begin(115200); //baud rate.
    mySwitch.enableTransmit(10);  // Using Pin #10
}
// ------------------------------------------------


// --------------------------------------------------------------------------------------------------------      

void loop() { //---START MAIN LOOP

  
    if (Serial.available()) { // ---START OF SERIAL COMMANDS---
   
        // -----------------------Build seralport data into readString------------------        
        String readString = ""; //clear string.
    
        while (Serial.available()) {
            delay(2); //account for any lag. 
        
            if (Serial.available() > 0) {
                char c = Serial.read();  //gets one byte from serial buffer        
                readString += c; //makes the string readString    
            } 
         
        }
    
        readString.toLowerCase();  //nice eh, easyer for if statement. 
        // -----------------------------------------------------------------------------      

        // ================OTHER COMMANDS=============
        
        // -----------------is_ready command--------------------
        if (readString.substring(0,8) == "is_ready"){
            Serial.println("");
            Serial.println("=!v!=READY"); //let software knows where ready and communicating ok.
            Serial.println("");    
            Serial.println("=!=done=!="); //tell our python script where done sending.
            Serial.println("");      
        }
        // -------------------------------------------------

        // ================RADIO COMMANDS=============
        
        // -----------------radio send command--------------------
        if (readString.substring(0,11) == "radio_send:"){
    
            String code_str;
            unsigned long int code_int;
                     
            code_str = readString.substring(11,readString.length()); //remove "radio_send:".
            code_str.replace(" ", ""); 
            code_int = code_str.toInt(); //convert to an int value to be accepted by mySwitch.
            
            mySwitch.send(code_int, 24); // send 24 bit code in decimal.
            Serial.println("");
            Serial.print("=!v!=Sent: ");
            Serial.println("");
            Serial.println(code_int);     
            Serial.println("=!=done=!="); //tell python script done.
            Serial.println("");       
        }
        // -------------------------------------------------
        
        
        // ================IR COMMANDS================
        
        
        // ----------------TEST-----------------------------
        if (readString.substring(0,11) == "ir_send:red"){
            irsend.sendNEC(0xF720DF, 32); //*4. 32 send the code useing NEC protocal.
            Serial.println("=!v!=OK");
            Serial.println("=!=done=!=");
        }
        // -------------------------------------------------

        // ----------------TEST-----------------------------
        if (readString.substring(0,13) == "ir_send:green"){
            irsend.sendNEC(0xF7A05F, 32); //*4.
            Serial.println("=!v!=OK");
            Serial.println("=!=done=!=");
        }
        // -------------------------------------------------

        // ----------------TEST-----------------------------
        if (readString.substring(0,12) == "ir_send:blue"){
            irsend.sendNEC(0xF7609F, 32); //*4. 32 
            Serial.println("=!v!=OK");
            Serial.println("=!=done=!=");
        }
        // -------------------------------------------------
        
        
        
        
    } // ---END OF SERIAL COMMANDS---
  
delay(1); //stop maxing out CPU.
}//---END MAIN LOOP.  

// --------------------------------------------------------------------------------------------------------  

