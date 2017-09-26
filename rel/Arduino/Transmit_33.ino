

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
        String readString = "";
    
        while (Serial.available()) {
            delay(2);
        
            if (Serial.available() > 0) {
                char c = Serial.read();  //gets one byte from serial buffer        
                readString += c; //makes the string readString    
            } 
         
        }
    
        readString.toLowerCase();  //nice eh
        // -----------------------------------------------------------------------------      


        // -----------------is_ready command--------------------
        if (readString.substring(0,8) == "is_ready"){
            Serial.println("");
            Serial.println("=!v!=READY");
            Serial.println("");    
            Serial.println("=!=done=!=");  
            Serial.println("");      
        }
        // -------------------------------------------------


        // -----------------send command--------------------
        if (readString.substring(0,5) == "send:"){
    
            String code_str;
            unsigned long int code_int;
                     
            code_str = readString.substring(5,readString.length());
            code_str.replace(" ", "");
            code_int = code_str.toInt();
            
            mySwitch.send(code_int, 24); // send code.
            Serial.println("");
            Serial.print("=!v!=Sent: ");
            Serial.println("");
            Serial.println(code_int);     
            Serial.println("=!=done=!=");
            Serial.println("");       
        }
        // -------------------------------------------------


    } // ---END OF SERIAL COMMANDS---
  
delay(1); //stop maxing out CPU.
}//---END MAIN LOOP.  

// --------------------------------------------------------------------------------------------------------  

