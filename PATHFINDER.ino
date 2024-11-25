#include <ServoTimer2.h> //libreria de servos
#include "Adafruit_TCS34725.h" //libreria de sensor de color


#define TRIG_PIN 8 //modificar ambos valores del pin cuando se tenga la info
#define ECHO_PIN 7
#define UMBRAL_DISTANCIA 30

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
  //avanzar();
  s1 = digitalRead(x1);
  s2 = digitalRead(x2);
  s3 = digitalRead(x3);
  s4 = digitalRead(x4);

  Serial.print(s1);
  Serial.print(s2);
  Serial.print(s3);
  Serial.println(s4);

  delay(1000);

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

void caminar(){
  while(true){
    int s1 = digitalRead(x1);
    int s2 = digitalRead(x2);
    int s3 = digitalRead(x3);
    int s4 = digitalRead(x4);

    if(hayPared()){
      detener();
      break;
    }

    if(s2 == 0 && s3 == 0){
      avanzar();
    }else if(s2 == 0 && s3 == 1){

      while(s2 == 0 && s3 ==1){

        girarDerecha();
        s2 = digitalRead(x2);
        s3 = digitalRead(x3);

      }
    }else if(s2 == 1 && s3 == 0){
      while (s2 == 1 && s3 == 0){
        girarIzquierda();
        s2 = digitalRead(x2);
        s3 = digitalRead(x3);
      }
    }else if(s1 == 0 || s4 == 0){
      detener();
      Serial.println("Interseccion detectada");
      delay(1000);
      avanzar();
    }else{
      detener();
      Serial.println("fuera de la linea, deteniendose");
    }
    delay(10);
  }
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
  analogWrite(leftForward, 135); 
  analogWrite(leftReverse, 0);
  analogWrite(rightForward, 150); 
  analogWrite(rightReverse, 0);
}

void retroceder() {
  analogWrite(leftForward, 0);
  analogWrite(leftReverse, 135); 
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 150); 
}

void girarIzquierda() {
  analogWrite(leftForward, 0);
  analogWrite(leftReverse, 135); 
  analogWrite(rightForward, 150); 
  analogWrite(rightReverse, 0);
}

void girarDerecha() {
  analogWrite(leftForward, 135); 
  analogWrite(leftReverse, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 150);
}

void detener(){
  analogWrite(leftForward, 0); 
  analogWrite(leftReverse, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightReverse, 0);
}



