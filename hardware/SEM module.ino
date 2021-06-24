#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
 
HTTPClient http;
 
const char* ssid     = "D7Y30R32";
const char* password = "123456789";
bool reconnecting = false;
String url = "http://smartenergymeter1234.herokuapp.com/meter/";
String m_id = "4";    // put your meter id here 
unsigned long StartTime = millis();
int pulses = 0;
 
void setup() {
 
  Serial.begin(9600);
  Serial.flush();
  Serial.println();
  Serial.println("This is Smart Energy Meter Module");
 
  WiFi.mode(WIFI_STA); 
  if(setup_WiFi()){
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    reconnecting =true;
  }
  
  // allow connection reuse (if server supports it)
  http.setReuse(true);
}
 
void loop() {
  int ldrv=analogRead(A0);
  if(ldrv>12) pulses+=1;
  
 
//Serial.println(ldrv);
  //elapsed time 
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  //Serial.println(ElapsedTime);
  if(ElapsedTime > 5000){
      // check for WiFi connection
      if ((WiFi.status() == WL_CONNECTED)) {
        WiFiClient client;
        Serial.println(pulses);
        http.begin(client, poll(m_id, String(pulses)));      //poll(m_id, pulse)
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
          Serial.printf("[STATUS: %d][PULSE: %d]: ", httpResponseCode,pulses);
    
          // HTTP 200 OK
          if (httpResponseCode == HTTP_CODE_OK) {
            http.writeToStream(&Serial);
          }
          Serial.println("");
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
        }
    
        http.end();
      }
      else{
        // RECONNECTING WIFI
        if(setup_WiFi()){
            Serial.println("Reconnected");
          }
        
        }
    StartTime = CurrentTime;  // reset start time
    pulses = 0;   // reset pulse count
//  delay(60000);
  }
}
 
// function  to setup wifi and handle reconnection
bool setup_WiFi(){
  if(reconnecting)Serial.print("Reconnecting : ");
  else Serial.print("Connecting : ");
 
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  return true;
}
 
// helper function to get complete url
String poll(String m_id, String pulse){
  return url +m_id +"/"+pulse+"/";
}