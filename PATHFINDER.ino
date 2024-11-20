#include <Servo.h> //libreria de servos
#include "Adafruit_TCS34725.h" //libreria de sensor de color

Servo ultraSonic;
Adafruit_TCS34725 sensorRGB = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

struct Color{
  float r;
  float g;
  float b;
};

//el diablo anda suelto, va pisando el mismo pavimento, recorriendo el barrio por comleto
void setup() {
  Serial.begin(9600);
  ultraSonic.attach(2);

  // Verificar si el sensor se inicializó correctamente
  if (!sensorRGB.begin()) {
    Serial.println("No se encontró el sensor");
    while (1); // Detener ejecución si el sensor no se encontró
  }
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
  servo.write(0);  // Mover a 0 grados (posicion inicial)
  delay(1000);     

  servo.write(80);   // Mover a 80 grados 
  delay(1000);       

  servo.write(156); //mover a 156 grados (ajuste fino)
  delay(1000);     
}
