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
//------------------------------------------------------------
//Declaración de funciones
float Termistor(); //ver referencia 2
void Leds();
float Calentamiento(float temperatura_operacion);
void setup();
//------------------------------------------------------------
// links de referencias 
/*
1. https://www.electrosoftcloud.com/pantalla-lcd-nokia-5110-con-arduino-parte-2/
2. https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/ 
*/
//------------------------------------------------------------
//Declaracion de pines
const int Rojo = 3; //led rojo
const int Azul = 2; //led azul
const int Humedad = A3; //sensor humedad
const int Termi = A5;//termistor senal
const int P_Operacion = A4;//punto operacion PID
const int Resistencia = 11; // resistencia calentamiento
const int PC = 13; //se crea un canal de comunicación con la PC
//------------------------------------------------------------
//Declaración variables
float inicial;
float final;
float E; //error
float diferencia; //margen 
float T;
//------------------------------------------------------------
//para funcion termistor de referencia 2.
int Vo = 0;
const float R1 = 10000;
float logRT;
float RT; 
float Temperatura_sensada;
float Temperatura;
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

void Leds(){
    if(T<32.0){ 
	    digitalWrite(Rojo, LOW); //se apaga luz roja
        digitalWrite(Azul, HIGH); //se enciende luz azul 
    }    
    else if(T>42.0){  
 	    digitalWrite(Azul, LOW); //se apaga luz azul
        digitalWrite(Rojo, HIGH); //se enciende luz roja
	}
    else{ //no se prenden los leds porque estan dentro del punto operación
	    digitalWrite(Rojo, LOW);
        digitalWrite(Azul, LOW);
	    

	}
}


float Calentamiento(float temperatura_operacion){
	
	if (Temperatura_sensada <= 42  || Temperatura_sensada >= 30  || temperatura_operacion <= Temperatura){ 
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


void setup() {
    Serial.begin(9600); // segun referencia 2 este es el inicio de puerto serial
    //A continuación realizamos configuración de los puertos como salidas y entradas
    pinMode(PC, INPUT);
    pinMode(Azul,OUTPUT);
    pinMode(Rojo,OUTPUT);

    //Realizamos configuración pantalla ver referencia 1.
    display.begin();
    display.setContrast(60); // Contraste a 60
    display.display(); 
    delay(500); //Tiempo espera
    display.clearDisplay();   // Limpiar pantalla 
    display.setTextSize(2);  // Fuente tamaño 2
    display.setTextColor(BLACK); //Texto negro
    }
//------------------------------------------------------------
//----------------------------MAIN----------------------------
//------------------------------------------------------------

void loop() { //Es como el main pero creado por la IDE de Arduino
	
	inicial = (analogRead(A4)); //op
    float hum = analogRead(Humedad)/10; //porcentaje de humedad
    float OP = inicial / 10;//% punto de operacion PID
    float T = Temperatura_sensada + E; //temperatura de operacion
    
	//llamamos nuestra función de LEDS.
    Leds();
    //Desplegamos en la pantalla
    /*Debemos mostrar
    -Temperatura operacion
    -Temperatura sensada
    -Humedad
    -Señal controlador
    */ 
	display.print("Temperatura Sensada: \n");
	display.print(Termistor()); //Temperatura Termistor
	display.print("°C"); 
	display.print("-------------------------\n");
    display.print("Temperatura Operacion: ");
	display.print(T); //Temperatura operacion
	display.print("°C"); 
    display.print("-------------------------\n");
	display.print("Punto de Operación PID:");
	display.print(OP); 
    display.print("-------------------------\n");
	display.print("Porcentaje de Humedad:");
	display.print(hum); 
	display.print("%"); 
    display.print("-------------------------\n");
	display.print("Error:");
	display.print(E);
    display.print("-------------------------\n");
    
	display.display();
	delay(200);
	display.clearDisplay();
	if(digitalRead(PC)==LOW){ //Si la comunicación se encuentra apagada entonces tendremos los siguientes valores por defalt
		Serial.print("Porcentaje Humedad x default:");
    	Serial.print(analogRead(Humedad)/10);
    	Serial.println("%");
        Serial.print("--------------------- \n");
        Serial.print("Temperatura x default:");
    	Serial.print(Temperatura_sensada);
    	Serial.println("°C");
        Serial.print("--------------------- \n");
    	
	}
	//llamamos nuestra funcion termistor para encontrar la temperatura sensada
	Termistor();
	//Revisamos el margen de error para los calentamientos
	final = inicial/5; // 5 tension
	E = final - Temperatura_sensada;
    float nuevo_punto_operacion = Temperatura_sensada - E;
	if (E == 0){ 
		diferencia = Calentamiento(nuevo_punto_operacion); // NO Hacer reajuste xq error es 0
	}
    if (E != 0){															
		diferencia = Calentamiento(nuevo_punto_operacion);	// Hacer reajuste 
	}
  
}



