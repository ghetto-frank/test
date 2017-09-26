
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


#include <RCSwitch.h>

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


        // -----------------is_ready command--------------------
        if (readString.substring(0,8) == "is_ready"){
            Serial.println("");
            Serial.println("=!v!=READY");
            Serial.println("");    
            Serial.println("=!=done=!="); //tell our python script where done sending.
            Serial.println("");      
        }
        // -------------------------------------------------


        // -----------------send command--------------------
        if (readString.substring(0,5) == "send:"){
    
            String code_str;
            unsigned long int code_int;
                     
            code_str = readString.substring(5,readString.length()); //remove first 5 chars.
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
        
        
    } // ---END OF SERIAL COMMANDS---
  
delay(1); //stop maxing out CPU.
}//---END MAIN LOOP.  

// --------------------------------------------------------------------------------------------------------  

