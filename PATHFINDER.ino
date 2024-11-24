#include <Servo.h> //libreria de servos
#include "Adafruit_TCS34725.h" //libreria de sensor de color


#define TRIG_PIN 9 //modificar ambos valores del pin cuando se tenga la info
#define ECHO_PIN 10
#define UMBRAL_DISTANCIA 15

const int POS_IZQUIERDA = 15;
const int POS_CENTRO = 80;
const int POS_DERECHA = 155;
const int servoPin = 2;

Servo servoUltraSonic;
Adafruit_TCS34725 sensorRGB = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

struct Color{
  float r;
  float g;
  float b;
};


void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servoUltraSonic.attach(servoPin);

  if (!sensorRGB.begin()) {
    Serial.println("No se encontró el sensor");
    while (1); 
  }
}

void loop() {
  buscarCamino(servoUltraSonic);
}

Color detectarColor(){
  Color color;
  sensorRGB.getRGB(&color.r,&color.g,&color.b);
  return color;
}

long medirDistancia(){
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = (duration/2) / 29.1;
  return distance;
}


int buscarCamino(Servo servoUltra){

  servoUltra.write(POS_IZQUIERDA);  // Mover hacia la izquierda
  delay(1000);
  if(!hayPared()){
    servoUltra.write(POS_CENTRO); //regresar a posicion central
    return 0;
  }  
   
  servoUltra.write(POS_DERECHA); //mover hacia la derecha
  delay(1000); 
  if(!hayPared()){
    servoUltra.write(POS_CENTRO);//Regresar a la posicion inicial
    return 1;
  }
  servoUltra.write(POS_CENTRO);
  return 2;
}

bool hayPared(){
  return medirDistancia() <= UMBRAL_DISTANCIA;
}
