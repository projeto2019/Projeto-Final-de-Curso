#define Vin A0
const float Offset = 510; //327

void setup(){ 
 Serial.begin(9600);
}

void loop(){
 
 float voltage = (5.0 / 1023.0)* (analogRead(Vin)- Offset);
 float current = voltage / 0.066; //(0.66 * (3.3/5));
 
 Serial.print("Current: ");
 Serial.println(current);
 delay(1000);

}
