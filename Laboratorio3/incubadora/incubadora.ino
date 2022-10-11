//LABORATORIO 3
//MARIA JOSE ARCE MARIN B60561
//ALEJANDRO CHACON V B
//------------------------------------------------------------
//Declaración Libreris
#include <Adafruit_PCD8544.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>
#include <SoftwareSerial.h>
//------------------------------------------------------------
//Declaración de funciones
float Termistor(); //ver referencia 2

float Calentamiento(float temperatura_operacion);

//------------------------------------------------------------
// links de referencias 
/*
1. https://www.electrosoftcloud.com/pantalla-lcd-nokia-5110-con-arduino-parte-2/
2. https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/ 
*/
//------------------------------------------------------------
//Declaracion de pines

const int Humedad = A3; //sensor humedad
const int Termi = A5;//termistor senal
const int P_Operacion = A4;//punto operacion PID
const int Resistencia = 11; // resistencia calentamiento

//------------------------------------------------------------
//Declaración variables

float E; //error
float diferencia; //margen 

//------------------------------------------------------------
//para funcion termistor de referencia 2.
int Vo = 0;
const float R1 = 10000;
float logRT;
float RT; 
float Temperatura_sensada;

float c1 = 1.009249522e-03; 
float c2 = 2.378405444e-04; 
float c3 = 2.019202697e-07;

//------------------------------------------------------------
//Configuramos la pantalla ver referencia 1.
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 8);
//------------------------------------------------------------
//Funciones 

float Termistor(){ // De referencia 2.
 Vo= analogRead(Termi);
 RT = R1 * (1023.0 / (float)Vo - 1.0);
 logRT = log(RT);
 Temperatura_sensada = (1.0 / (c1 + c2*logRT + c3*logRT*logRT*logRT));
 Temperatura_sensada = Temperatura_sensada - 273.15; // Celsius
 return Temperatura_sensada;
}




float Calentamiento(float temperatura_operacion){
		if (Temperatura_sensada <= 42  || Temperatura_sensada >= 30  || temperatura_operacion <= Temperatura_sensada){ 
		analogWrite(Resistencia, roundf(0)); 
		return 0; //estos son los casos buenos donde no debemos realizar correcciones
    }
    //casos para realizar correcciones en la temperatura y el calentamiento de la resistencia
    else if(Temperatura_sensada > 42){												
		analogWrite(Resistencia, roundf(42*(255/80))); 
		return  42*100/80;
		}
	
    else if(Temperatura_sensada < 30){
		analogWrite(Resistencia, roundf(30*(255/80)));
		return 30*100/80;
		}
	
	else if(temperatura_operacion > Temperatura_sensada){
			analogWrite(Resistencia, roundf(temperatura_operacion*(255/80)));
			return temperatura_operacion*100/80;
		}
	}

void setup(){
      Serial.begin(9600);
      pinMode(13, INPUT);//pc
      pinMode(2,OUTPUT); //azul
      pinMode(3,OUTPUT);//rojo
      display.begin();
      display.setContrast(50); // Contraste de la pantalla
      display.display(); //
      delay(500);
      display.clearDisplay();   // Limpia la pantalla 
      display.setTextSize(0.1);  // Tamano de fuente
      display.setTextColor(BLACK); //Color de texto
}

//------------------------------------------------------------
//----------------------------MAIN----------------------------
//------------------------------------------------------------

void loop() { //Es como el main pero creado por la IDE de Arduino
      
      	float T;
    //llamamos nuestra funcion termistor para encontrar la temperatura sensada
	float Temperatura = Termistor();
	//llamamos nuestra función de LEDS.
    	
    	if(Temperatura<32){ 
      	  digitalWrite(3, LOW); //se apaga luz roja
          digitalWrite(2, HIGH); //se enciende luz azul 
          T = 32; //punto operacion
          E = T -  Temperatura;
        }    
        else if(Temperatura>42){  
          digitalWrite(2, LOW); //se apaga luz azul
          digitalWrite(3, HIGH); //se enciende luz roja
          T = 42;//punto de operacion
          E = T - Temperatura;
        }
       else{ //no se prenden los leds porque estan dentro del punto operación
	  digitalWrite(3, LOW);
          digitalWrite(2, LOW);
          E = 0;
	    

	}
//Leds(Temperatura);
    	
    	float hum = analogRead(Humedad)/10; //porcentaje de humedad
    	float OP = analogRead(A4) / 10;//% punto de operacion PID
    	
    //Desplegamos en la pantalla
    /*Debemos mostrar
    -Temperatura operacion
    -Temperatura sensada
    -Humedad
    -Señal controlador
    */ 
	display.print("T: \n");
	display.print(Termistor()); //Temperatura Termistor
	display.print("C\n"); 
	
    display.print("T Op: ");
	display.print(T); //Temperatura operacion
	display.print("C\n"); 
 
	display.print("PID:");
	display.print(OP); 
    
	display.print("\nH %:");
	display.print(hum); 
	display.print("%\n"); 
    
	display.print("E:");
	display.print(E);
    //display.print("-------------------------\n");
    
	display.display();
	delay(200);
	display.clearDisplay();
	if(digitalRead(13)==LOW){ //Si la comunicación se encuentra apagada entonces tendremos los siguientes valores por defalt
	Serial.print("Porcentaje Humedad x default:");
    	Serial.print(analogRead(Humedad)/10);
    	Serial.println("%");
        Serial.print("--------------------- \n");
        Serial.print("Temperatura x default:");
    	Serial.print(Temperatura_sensada);
    	Serial.println("°C");
        Serial.print("--------------------- \n");
    	
	}
	

   // float nuevo_punto_operacion = Temperatura - E;
	if (E == 0){ 
		diferencia = Calentamiento(Temperatura); // NO Hacer reajuste xq error es 0
	}
    if (E != 0){															
		diferencia = Calentamiento(T);	// Hacer reajuste 
	}
  
}


