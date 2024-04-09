#include <ESP8266WiFi.h>

const char* ssid = "koala";
const char* password = "koala1234";

int ledPin = 4;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = 0;
  int index = request.indexOf("/LED=");
  if (index != -1)  {
    index += 5;
    String parameter = request.substring(index);
    int value = parameter.toInt();
    analogWrite(ledPin, value);
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width\"><title>118銷售網</title></head>");
  client.print("<body><script>function updateLED(value) {var xhr = new XMLHttpRequest();xhr.open(\"GET\", \"/LED=\" + value, true);xhr.send();}</script>");
  client.print("Led pin is now: ");
  client.println("<br><br>");
  client.println(value);
  client.println("<input type=\"range\" min=\"0\" max=\"255\" onchange=\"updateLED(this.value)\">亮度</body></html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}