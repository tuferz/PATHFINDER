#include <ServoTimer2.h> //libreria de servos
#include "Adafruit_TCS34725.h" //libreria de sensor de color


#define TRIG_PIN 8 //modificar ambos valores del pin cuando se tenga la info
#define ECHO_PIN 7
#define UMBRAL_DISTANCIA 25

int celdas = 0;

//variables del servo del ultrasonico
const int POS_IZQUIERDA = 2140;
const int POS_CENTRO = 1450;
const int POS_DERECHA = 600;
const int servoPin = 3;

//variables de los motores
const int leftForward = 9;
const int leftReverse = 6;
const int rightForward = 10;
const int rightReverse = 5;

//variables de los IR
int s1, s2, s3, s4;
const int x1 = 14;
const int x2 = 15;
const int x3 = 16;
const int x4 = 17;

ServoTimer2 servoUltraSonic;
Adafruit_TCS34725 sensorRGB = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

struct Color{
  float r;
  float g;
  float b;
};


void setup() {
  Serial.begin(9600);

  pinMode(leftForward, OUTPUT);
  pinMode(leftReverse, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightReverse, OUTPUT);

  pinMode(x1, INPUT);
  pinMode(x2, INPUT);
  pinMode(x3, INPUT);
  pinMode(x4, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servoUltraSonic.attach(servoPin);

 // if (!sensorRGB.begin()) {
 //   Serial.println("No se encontró el sensor");
 //   while (1); 
 // }
}

void loop() {
  //buscarCamino(servoUltraSonic);
  
  avanzar();
  buscarCamino(servoUltraSonic);
  Serial.println(celdas);
  delay(500);
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



int buscarCamino(ServoTimer2 servoUltra){

  servoUltra.write(POS_IZQUIERDA);  // Mover hacia la izquierda
  delay(1000);
  if(!hayPared()){
    servoUltra.write(POS_CENTRO); //regresar a posicion central
    Serial.print("Camino encontrado a la izquierda");
    return 0;
  }  
   
  servoUltra.write(POS_DERECHA); //mover hacia la derecha
  delay(1000); 
  if(!hayPared()){
    servoUltra.write(POS_CENTRO);//Regresar a la posicion inicial
    Serial.print("Camino encontrado a la derecha");
    return 1;
  }
  servoUltra.write(POS_CENTRO);
  Serial.print("Callejon >:(");
  return 2;
}

bool hayPared(){
  return medirDistancia() <= UMBRAL_DISTANCIA;
}

void avanzar() {
  s1 = digitalRead(x1);
  s2 = digitalRead(x2);
  s3 = digitalRead(x3);
  s4 = digitalRead(x4);

  while(s1 == 0 || s2 == 0){

    if(s2 == 0 && s3 ==0){
      analogWrite(leftForward, 122); 
      analogWrite(leftReverse, 0);
      analogWrite(rightForward, 128); 
      analogWrite(rightReverse, 0);  
    }
    
    if(s2 == 1 && s3 == 0){
      analogWrite(leftForward, 122); 
      analogWrite(leftReverse, 0);
      analogWrite(rightForward, 138); 
      analogWrite(rightReverse, 0);
    }
    if(s2 == 0 && s3 == 1){
      analogWrite(leftForward, 132); 
      analogWrite(leftReverse, 0);
      analogWrite(rightForward, 122); 
      analogWrite(rightReverse, 0);
    }
    if(s1 == 0 || s2 == 0){
      celdas++;
    }
    s1 = digitalRead(x1);
    s2 = digitalRead(x2);
    s3 = digitalRead(x3);
    s4  = digitalRead(x4);
  }
  detener();
  
}

void retroceder() {
  analogWrite(leftForward, 0);
  analogWrite(leftReverse, 122); 
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 128); 
}

void girarIzquierda() {
  analogWrite(leftForward, 0);
  analogWrite(leftReverse, 122); 
  analogWrite(rightForward, 128); 
  analogWrite(rightReverse, 0);
}

void girarDerecha() {
  analogWrite(leftForward, 122); 
  analogWrite(leftReverse, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 128);
}

void detener(){
  analogWrite(leftForward, 0); 
  analogWrite(leftReverse, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 0);
}



