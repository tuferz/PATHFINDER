#include <Servo.h> //libreria de servos
#include "Adafruit_TCS34725.h" //libreria de sensor de color

Servo ultraSonic;
Adafruit_TCS34725 sensorRGB = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

struct Color{
  float r;
  float g;
  float b;
}//okno

//el diablo anda suelto, va pisando el mismo pavimento, recorriendo el barrio por comleto
void setup() {
  Serial.begin(9600);
  ultraSonic.attach(2);
}

void loop() {
  USSweep(ultraSonic);
}

Color detectarColor(){
  Color color;
  sensorRGB.getRGB(&color.r,&color.g,&color.b);
  return color;
}


void USSweep(Servo servo){
  servo.write(0);  // Move to 90 degrees (initial position)
  delay(1000);     //Wait 1 second

  servo.write(80);   // Move 90 degrees in one direction (0 degrees)
  delay(1000);       // Wait 1 second

  servo.write(156); // Move 180 degrees in the opposite direction
  delay(1000);      // Wait 1 second
}
