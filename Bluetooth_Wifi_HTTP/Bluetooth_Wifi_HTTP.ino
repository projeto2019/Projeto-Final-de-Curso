#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <esp_wpa2.h>
#include "index.h"


WebServer server(80);
BluetoothSerial SerialBT;
 
//Credenciais para connectar á internet
const char* ssid = "eduroam"; //MEO-66B281
const char* password = "your password"
const char* username = "your username";
 

//exectuado quando alguem acede á pagina web.
void handleRoot() {
 String webPage = MAIN_page;
 //Read HTML contents
 server.send(200, "text/html", webPage); //Responde com pagina web
}

//Exectudado quando alguem faz um pedido(ajax).
void handleADC() {
 int a = touchRead(T0);
 String adcValue = String(a);
 server.send(200, "text/plane", adcValue); //Responde como o valor do sensor
}


 
void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

    
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)username, strlen(username));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)password, strlen(password));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  WiFi.begin(ssid,password); //connect to wifi
  if(WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
     delay(5000); 
  }

  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
  server.begin();

  server.on("/", handleRoot);      //This is display page
  server.on("/readADC", handleADC);//To get update of ADC Value only
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");

  if(!SerialBT.begin("ESP32")){
    Serial.println("An error occurred initializing Bluetooth");
  }else{
    Serial.println("Bluetooth initialized");
  }
}

 
void loop(void){
  server.handleClient();
  delay(1);
  //Serial.println(touchRead(T0));

  if(SerialBT.available()){
    char command = SerialBT.read();
    Serial.println(command);
    SerialBT.println(touchRead(T0));
 } 

  if(WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(2000);
  }
}
