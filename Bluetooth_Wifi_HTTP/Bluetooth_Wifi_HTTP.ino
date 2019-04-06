#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <esp_wpa2.h>
#include "index.h"

WebServer server(80);
BluetoothSerial SerialBT;
 
//Credenciais para connectar á internet
const char* ssid = "NOS-FBB0"; //MEO-66B281
const char* password = "8b5db907fbd2"; // 670F64B8D4
const char* username = "username";

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
  
  WiFi.begin(ssid,password); // connect to wifi
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
  server.on("/", handleRoot);// This is display page
  server.on("/readADC", handleADC);// To get update of ADC Value only
 
  server.begin();// Start server
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

  //If communication by BT
  if(SerialBT.available()){ 
    String msg = SerialBT.readString();
    char commandID = msg[0];

    switch(commandID){

      case '1':
      SerialBT.println(touchRead(T0));
      break;

      case '2':
      SerialBT.println("Teste");
      break;

      default:
      SerialBT.println("Erro");
      break;
      
    }
  } 

  // If you lost WiFi connection
  if(WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      delay(2000);
  }

}

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
 server.send(200, "text/plane", adcValue); //Responde com o valor do sensor
}
