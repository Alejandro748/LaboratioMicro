#include <Servo.h>
#include <ArduinoBLE.h>
const int buttonPin = A0;  // Se define el botón en el pin A0
int buttonState = 0;  // Se crea la variable para el estado del botón

int ledState = LOW; // Se crea la variable para el estado del LED
int proximity = 250; // Se crea la variable para la proximidad. 0 es muy cerca y 250 es muy lejos.
unsigned long previousMillis = 0; //Se crea la variable que va contando los mili segundos.

const long intervalLong = 15000; //El intervalo que se utiliza para consultar el estado del sistema (sensor de proximidad)
//const long intervalMed = 500;
//const long intervalShort = 100;

Servo myServo; //Declaración del servomotor

 
void setup() { 
//--------------------------------------------------------------------------------------sensorARDUINO------------------------------------------------------------------------------
Serial.begin(9600);
  while (!Serial);

  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
  }

  // Se ponen los LED como salidas
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Se apagan todos los LED
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  
//---------------------------------------------------------------------------------servo--------------------------------------------------------------------------------
  myServo.attach(D8); // El servo se conecta al pin digital 8 del arduino.
  myServo.writeMicroseconds(1500);  // Se inicializa el motor detenido.
  pinMode(buttonPin, INPUT);  //se inicializa el botón como entrada.
void loop() { 
unsigned long currentMillis = millis(); //se comienzan a contar los milisegundos que dura la ejecución.
      
      buttonState = digitalRead(buttonPin); // se lee el estado del botón

      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (buttonState == HIGH) { //si el botón está presionado, se comienza el siguiente protocolo:
        myServo.writeMicroseconds(1300); //el servo gira sentido horario (abre)
        delay(1100);
        myServo.writeMicroseconds(1500);  // Stop
        delay(5000);  //se queda abierto durante más tiempo de lo habitual
        delay(5000);
        delay(5000);
        delay(5000); 
        delay(5000);
        delay(5000);           
        myServo.writeMicroseconds(1500);  // Stop
        delay(5000);
        
        myServo.writeMicroseconds(1700);  // el servo gira sentido antihorario (cierra)       
        
        delay(1100);
   
     
    
        myServo.writeMicroseconds(1500);  // Stop
        delay(5000); //un leve delay cuando termina el protocolo.
        delay(5000); 

      }

      else{ //Si el botón NO está presionado, se inicia con el demo principal
         
        // check if a proximity reading is available
        if (APDS.proximityAvailable()) { //se revisa si la lectura de proximidad esta disponible:
          // La proximidad se lee de la siguiente manera:
          // 0   => cerca
          // 255 => lejos
          // -1  => error
          int proximity = APDS.readProximity();

          if (proximity > 150) { //Si la proximidad está lejos
            if (currentMillis - previousMillis >= intervalLong) { //aca lo revisa contando los milisegundos y comprandolos con las cantidad que se seteo al principio.
              previousMillis = currentMillis; //se tiene que se guarda el valor del contador de milisegundos
            // print value to the Serial Monitor
              // Si el LED está apagado o vice-versa:
              if (ledState == LOW) {
                ledState = HIGH;
              } else {
                ledState = LOW;
              }
              digitalWrite(LEDG, ledState); //Se setea el LED verde y se apaga el resto.
              digitalWrite(LEDR, HIGH);
              digitalWrite(LEDB, HIGH);
              myServo.writeMicroseconds(1300);  // Empieza a girar sentido horario
              delay(1100);                      // espera aprox 2s
              myServo.writeMicroseconds(1500);  // Stop
              delay(1000);  
              delay(1000);
              myServo.writeMicroseconds(1700);  // Empieza a girar sentido antihorario        
              delay(1100);                       // espera aprox 2s
   
     
    
              myServo.writeMicroseconds(1500);  // Stop
              delay(1000); 

              myServo.writeMicroseconds(1500);  // Stop
              delay(1000);        
            }
        
          }

          else if(proximity > 50 && proximity <= 150){ //Se revisa si se encuentra algo a una distancia intermedia
            if (currentMillis - previousMillis >= intervalLong) {
              previousMillis = currentMillis;
                      
              
              if (ledState == LOW) {
                ledState = HIGH;
              } else {
                ledState = LOW;
              }
              digitalWrite(LEDB, ledState); //Se enciende el LED azul
              digitalWrite(LEDR, HIGH);
              digitalWrite(LEDG, HIGH);
              myServo.writeMicroseconds(1300);  // gira en sentido horario
              delay(1100);                      // espera aprox 2s

              myServo.writeMicroseconds(1500);  // Stop
              delay(1000);

              myServo.writeMicroseconds(1700);  // gira en sentido antihorario
              delay(1100);
   
     
    
              myServo.writeMicroseconds(1500);  // Stop
              delay(1000); 

       
            }
              
          }

          else { //Ahora, si el objeto se encuentra demasiado cerca del sensor, se activa el protocolo de seguridad: detención de todo el sistema.
            if (currentMillis - previousMillis >= intervalLong) {
              previousMillis = currentMillis;
              
              if (ledState == LOW) {
                ledState = HIGH;
              } else {
                ledState = LOW;
              }

              digitalWrite(LEDR, ledState); //se enciende el LED rojo.
              digitalWrite(LEDB, HIGH);
              digitalWrite(LEDG, HIGH);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
              myServo.writeMicroseconds(1500);  // Stop
              delay(5000);
            }
     
      
          }
       }      
      }    
    }
    
  }
}
