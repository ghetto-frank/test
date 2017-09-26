import SimpleHTTPServer
import SocketServer
import serial #--------NOTE: changed 'ser.isOpen()', To: 'ser.is_open'.. (Difrence In Version.)
import time


import socket #tell if beast is on.

global ser #out serial lib.
ser = "" #to avid not asined error.





#------------------------------------------------------------------------------------------------
def do_connect(device_path, baud_rate):
	
    global ser

   
    try:
        ser = serial.Serial(device_path, baud_rate, timeout=1.0)  # open serial port
    except:
        return (-1, "") #failed to connect.
		
 
    if not ser.isOpen() == True:
        return (-2, "") #cant open port.
    
    #------connected--- 

    time.sleep(3) #give device a little time before sending data.
    #note: only seems to work with 2 sec delay. 
   
    
    err_code1 = do_send("is_ready")       


    if not err_code1[0] == 0: 
        ser.close()
        return (-3, "")#connected, but to wrong device.
    
   
    return (0, str(err_code1[1])) #return 0, device name.
#------------------------------------------------------------------------------------------------    


 
#------------------------------------------------------------------------------------------------    
def is_connected():
	
    global ser
	
    if ser == "": #not yet asined.
        return False	
	
    if ser.isOpen() == True:
        return True
    else:
        return False
#------------------------------------------------------------------------------------------------    

    

#------------------------------------------------------------------------------------------------   
def do_send(send_line):
	
    global ser
    
    #-------------------------------
    if ser.isOpen() == False:
        return (-1, "") #ser not open.
        
    send_line = str(send_line)
    send_line = send_line.strip()
     
    if len(send_line) < 1:
        return (-2, "") #no data to send.
    #--------------------------------           

    try:
        ser.write(send_line + "\r\n")    # write the string.
    except:
        return (-3, "")
	
    time.sleep(1 / 1000.0) #wait for data to reach device.
        
        
    in_line = "" #data from device.
    out_line = "" #line to return from device.
    line_timeout = 0
        
    #----------------------------------------------------------------------------------
    while not in_line[0:10] == "=!=done=!=": #last line must be read to read all buffer. 
 
        if line_timeout > 10: #done line sould always be < 10 lines in.
            return (-4, "") #line timeout/non-vailid command to device.
        else:
            line_timeout = line_timeout + 1
        
        #---------------
        try:
            in_line = ser.readline()
            in_line = in_line.lower() #convert all to lower case.
            in_line = in_line.strip()
            in_line = str(in_line)
        except:
            print("Error In Reciving/Decodeing The Data From Device!")
            return (-5, "")
			
        if in_line[0:5] == "=!v!=": #extract the line where intrested in.
            out_line = str(in_line[5:])

    #----------------------------------------------------------------------------------
    return (0, out_line)
#------------------------------------------------------------------------------------------------   




#----------------------------------------------------------------------------------------------------------
def do_port_open(str_ip, int_port): #my way of testing if a device is live, as my pc does not responde to a ping.

    try:
        str_ip = str(str_ip)
        int_port = int(int_port)
    except:
        return -2 #error in ip/port.

    #------------------------
    try:
        sock.settimeout(2) #fail after 2 seconds on connection fail.
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        result = sock.connect_ex((str_ip, int_port))
    except:
        return -1 #port closed/fail connect.
			
    if result == 0:
        return 0 #Port is open.
    else:
        return -1 #Port is closed.
#----------------------------------------------------------------------------------------------------------



#----------------------------------------------------------------------------------------------------------
def do_send_code(int_code):
	
	
    print(" - SEND CODE ASK: " + str(int_code))
    #res = do_send("send:" + str(int_code))
    #print(str(res))
    
    beast_on = -1 #asume pc off/fail port connect.
    #--------------------------------
    if int_code == 0000000:    #EXEPTION RADIO CODE HERE!
		
        print(" - Checking If Beast Is Off!: ")
        beast_on = do_port_open("xxx.xxx.xxx.xx", 80) #check if server pc is up running web server. 
          
        if beast_on == 0: #port connected ok.
            print("   - ERROR: Server PC Is On. Not Switching Off.")
            return -1 #radio code not sent
        else:
            print("   - OK: Server PC Appers To Be Off. Switching Off.")
            print(" - OK, Sending...")
            #res = do_send("radio_send:" + str(int_code)) #Send The Radio Code
            	
    else:
        print(" - OK, Sending...")
        res = do_send("radio_send:" + str(int_code)) #Send The Radio Code
        
    #--------------------------------
     
    return 0 #radio code sent ok.
#---------------------------------------------------------------------------------------------------------- 
 


#---------------------------------------------------------------------------------------------------------- 
class GetHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):

    def do_GET(self):
		
        print("_______________________________________________________")
        request_path = self.path
        send_code = request_path.replace("/sendcode.html?", "")
        
        print("Send Code: " + send_code)


        try:
            send_code = int(send_code)
        except:
            send_code = 0

        if send_code > 0 and send_code < 9999999999:
            do_send_code(send_code)

        SimpleHTTPServer.SimpleHTTPRequestHandler.do_GET(self)
#---------------------------------------------------------------------------------------------------------- 
 
print("____________________________________________________________________")
print("  == Home Automation Project == ")
print(" ----- V1.0 Alpha, You Turn Me Ooonnn.. (Or Off...) -----")
print("````````````````````````````````````````````````````````````````````")
print("********************************************************************")
print("")
print(" - Connecting To Arduino...")

tr = do_connect("/dev/ttyACM1", 115200)

print("   - Arduino:'" + str(tr)) + "'."

if not tr[0] == 0: #there was an error connecting here!
    quit()
    

print("")
print(" - OK!")
print(" - Bringing Up Server...")
 


PORT = 80
Handler = GetHandler

httpd = SocketServer.TCPServer(("", PORT), Handler)

print("")
print(" - OK!")
print("\n - Listening...")

httpd.serve_forever() #fires do_GET funtion witch fires of what i need ;)
