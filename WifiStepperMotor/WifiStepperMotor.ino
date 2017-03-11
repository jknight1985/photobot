#include <ESP8266WiFi.h>
#include <Stepper.h>

WiFiServer server(80); //Initialize the server on Port 80

// Stepper motor
int SPMU = 2048;  // Steps per revolution
int StartMotor = 0; 
Stepper myStepper(SPMU,D3,D2,D1,D0);  

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

  // Start server
    WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
    WiFi.softAP("Photobot", "12345678"); // Provide the (SSID, password);
    server.begin(); // Start the HTTP Server

  // Looking under the hood
    Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
    //IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
    //Serial.print("Server IP is: "); // Print the IP to the monitor window
    //Serial.println(HTTPS_ServerIP);

  // Stepper motor setup
  myStepper.setSpeed(5);
}

// the loop routine runs over and over again forever:
void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Looking under the hood
    Serial.println("Somebody has connected :)"); 
  
  //Read what the browser has sent into a String class and print the request to the monitor
    String request = client.readStringUntil('\r'); 
  //Looking under the hood
    Serial.println(request);  
  
    if (request.indexOf("/OFF") != -1){
      StartMotor=0; 
    }
    else if (request.indexOf("/ON") != -1){
      StartMotor=1;
    }
  // Buttons document to hmtl browser
  String s = "HTTP/1.1 200 OK\r\n";
  s +="Content-Type: text/html,\r\n\r\n";   
  s +="<!DOCTYPE html>\r\n<html>\r\n";
  s +="<br><input type=\"button\" name =\"b1\" value=\"Turn Motor ON\" onclick=\"location.href='/ON'\">";
  s +="<br><br><br>";
  s +="<br><input type=\"button\" name =\"b1\" value=\"Turn Motor OFF\" onclick=\"location.href='/OFF'\">";
  s +="</html>\n";

  //Serve the HTML document to the browser.
  client.flush(); //clear previous info in the stream
  client.print(s); // Send the response to the client
  delay(1);
  Serial.println("Client disonnected"); //Looking under the hood  


  // Stepper motor test
 if(StartMotor)
 {
  myStepper.step(500);   // eine Umdrehung
  delay(1000);
  myStepper.step(-500);   // eine Umdrehung 
  delay(1000); 
 }
}
