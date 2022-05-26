// Laboratorio 3 - incubadora
// Marlon Lazo Coronado y Gokeh Avila Blanco

#include <math.h>
#include <stdio.h>
#include <Adafruit_PCD8544.h>
#include <avr/io.h>
#include <util/delay.h>

//definición de pines
const int calentador=9;
const int LEDazul=3;
const int comm=13;
const int LEDrojo=2;

// definimos LCD
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 8);
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define NUMFLAKES 10


// Parámetros para las ecuaciones de S-H para la temperatura con el termistor
float R1 = 100000; 
float logR2;
float R2;
float temp;

// coeficientes steinhart-hart, 
float c1 = 0.8586139205e-03, c2 = 2.059709585e-04, c3 = 0.8130635267e-07; 

float T_cycle;
int valorCalentador=0;

int valorTermistor;     // Entrada A0
float voltajeTermistor; // Entrada A04

float valorDeseado;
float valorDeseadoDisplay;

int valorHumedad; // Entrada A5
float humedadNormalizada; 
float humedadDisplay;

float error;

// ecuacion para temp segun R del termistor
void hart(){ 
  R2 = R1 * (1023.0 / (float)valorTermistor - 1.0);
  logR2 = log(R2);
  temp = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); 
  temp = temp -273.15; // convertimos a °C
}

// para activar el led correspondiente segun la temp
void aviso_seguridad(){ 
  if ((temp < 30)){ 
    digitalWrite(LEDazul, HIGH);
    digitalWrite(LEDrojo, LOW);
  }
  else if ((temp > 30) & (temp < 42)){ 
    digitalWrite(LEDrojo, LOW);
    digitalWrite(LEDazul, LOW);
  }
  else if ((temp > 42)){ 
    digitalWrite(LEDazul, LOW);
    digitalWrite(LEDrojo, HIGH);
  }
}

// se pone el led int. si se pasa el 50% de la humedad relativa
void indicador_humedad(){ 
  if (humedadNormalizada > 50){
    digitalWrite(LED_BUILTIN, HIGH);    
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// parpadeo
void parpadeo(){ 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);                       
  digitalWrite(LED_BUILTIN, LOW);    
  delay(300);
}

// para no sobrecalentar los huevos
float ajusteCalentador(float target){
  if(target > 42){
    if(temp <= 42){
      analogWrite(calentador, roundf(42*(255/80)));
      return 42*100/80;
    }else{
      analogWrite(calentador, roundf(0*(255/80)));
      return 0*100/80;
    }
  }else if(target < 30){
    if(temp >= 30){
      analogWrite(calentador, roundf(0*(255/80)));
      return 0*100/80;
    }else{
      analogWrite(calentador, roundf(30*(255/80)));
      return 30*100/80;
    }
  }else{
    if(target < temp){
      analogWrite(calentador, roundf(0*(255/80)));
      return 0*100/80;
    }else{
      analogWrite(calentador, roundf(target*(255/80)));
      return target*100/80;
    }
  }
}

void display_refresh(){
  display.setCursor(0,0);
  display.print("T.Set:");
  display.println(valorDeseadoDisplay);
  display.print("Temp:");
  display.println(temp);
  display.print("Duty:");
  display.print(T_cycle);
  display.println("%");
  display.print("Hum:");
  display.print(humedadDisplay);
  display.println("%");
  display.display();
  display.clearDisplay();
}

float pidControl(float set, float current){
    return set - current;
}

void setup() {
  Serial.begin(9600);
  Serial.println("PCD test");
  display.begin();
  display.setContrast(75);
  display.display(); 
  delay(500);
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(BLACK);
  pinMode(LED_BUILTIN, OUTPUT); // se configuran los pines como salida
  pinMode(LEDazul, OUTPUT);
  pinMode(LEDrojo, OUTPUT);
  pinMode(comm, INPUT);
}

void loop() { // loop infinito
  if(digitalRead(comm) == LOW){
    Serial.print("temp:");
    Serial.print(temp);
    Serial.println(" C");
    Serial.print("HUMEDAD:");
    Serial.print(humedadDisplay);
    Serial.println("%");
  }
  valorDeseado = analogRead(A4);
  valorDeseadoDisplay = (valorDeseado)/5.11;

  // se lee el voltaje que entra al pin A0
  valorTermistor = analogRead(A0); 
  // se convierte el valor de 10 bits a un valor segun el voltaje
  voltajeTermistor = valorTermistor/204.6; 

  // se lee el voltaje que entra al pin A5
  valorHumedad = analogRead(A5); 
  // se convierte el valor de 10 bits a un valor normalizado
  humedadNormalizada = valorHumedad/14.61428571; 
  humedadDisplay = humedadNormalizada+20;
  aviso_seguridad();

  hart();

  error = pidControl(valorDeseadoDisplay, temp);

  if(error != 0){
    T_cycle = ajusteCalentador(temp+error);
  }else{
    T_cycle = ajusteCalentador(0);
  }
  display_refresh();
}
