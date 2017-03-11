#include <ESP8266WiFi.h>
#include <Stepper.h>

WiFiServer server(80); //Initialize the server on Port 80
unsigned char SENS_PIN = A0;

// Stepper motor
int SPMU = 32;  
Stepper myStepper(SPMU, 2,3,4,5);  


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
    //Serial.begin(9600);
  
  // Start server
    WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
    WiFi.softAP("Photobot", "12345678"); // Provide the (SSID, password);
    server.begin(); // Start the HTTP Server

  // Looking under the hood
    Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
    IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
    Serial.print("Server IP is: "); // Print the IP to the monitor window
    Serial.println(HTTPS_ServerIP);

  // Stepper motor setup
  myStepper.setSpeed(800);
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
  
  if (request.indexOf("/") != -1){
    //TODO
    }

  // Buttons document to html browser
  String s = "HTTP/1.1 200 OK\r\n";
  s +="Content-Type: text/html,\r\n\r\n";  
  s += "<meta http-equiv=\"refresh\" content=\"1\">"; // Update every 5 seconds
  s +="<body>Light: "; 
  s +=String('bla');
  s += "</body>"; 
  s +="<br><input type=\"button\" name =\"b1\" value=\"Update\" onclick=\"location.href='/ON'\">";
  s +="<!DOCTYPE html>\r\n<html>\r\n";
  s +="<br><br><br>";
  s +="</html>\n";

  //Serve the HTML document to the browser.
  client.flush(); //clear previous info in the stream
  client.print(s); // Send the response to the client
  delay(1);
  Serial.println("Client disonnected"); //Looking under the hood  

  // Stepper motor test
  myStepper.step(600);   // eine Umdrehung
    delay(500);
    myStepper.step(-600);  // wieder rückwarts
    delay(500);

  
}
