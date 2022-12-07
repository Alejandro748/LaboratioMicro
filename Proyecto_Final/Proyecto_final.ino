//Laboratio de Microcontroladores
//PORYECTO FINAL
//María José Arce B60561
//Alejandro Chacón B61871

//datos importantes del motor servo SM-S3317SR:
//38 oz-in @ 6 V. Torque: 19 oz-in @ 5V or 23 oz-in @ 6V
//    1300 μs: Turn clockwise
//    1500 μs: Stops the motor
//    1700 μs: Turn counterclockwise


#include <Servo.h>
#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>
 // Bluetooth® Low Energy Battery Service
BLEService SecurityService("1811");
// Bluetooth® Low Energy Battery Level Characteristic
BLEUnsignedCharCharacteristic SecurityLevelChar("180E",  // standard 16-bit characteristic UUID
    BLERead | BLENotify); // Los clientes remotos van a recibir los cambios cuando cambie el char


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
//---------------------------------------------------------------------------------BLE-------------------------------------------------------------------------------------------------
  
  pinMode(LED_BUILTIN, OUTPUT); // Se inicializa el LED del arduino para indicar que ya se está conectado a la central (celular)

  // Se inicializa el servicio de BLE:
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!"); //Esto sucede si falla algo en la inicialización del BLE

    while (1);
  }

  BLE.setLocalName("SecurityMonitor"); //Acá se define el local name para el arduino.
  BLE.setAdvertisedService(SecurityService); // se añade el UUID del servicio
  SecurityService.addCharacteristic(SecurityLevelChar); //  Se crea el char para los datos de proximidad
  BLE.addService(SecurityService); // Se añade el servicio de seguridad
  SecurityLevelChar.writeValue(proximity); // Se setea el valor inicial para el char.
  //Se va a empezar a transmitir los paquetes  del Bluetooth Low Energ y van a ser visibles en el dispositivo hasta que este reciba una nueva conexión
  // Se comeinza el  advertising
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");
} 

void loop() { 
//---------INICIALIZACIÓN BLE-----------------------------------------------------------------------------
// Se espera por un dispositivo central de Bluetooth® Low Energy
  BLEDevice central = BLE.central();
  // Si se conectó a una central:
  if (central) {
    Serial.print("Connected to central: ");
    // Se imprime la dirección BT de la central:
    Serial.println(central.address());
    // Se enciende el LED para indicar que existe una conexión:
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {    //se revisa constantemente que la central está conectada, esto debido a la larga duración de la ejecución del código.
//--------------------------------------------------------------------------------------------------------
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

          //se imprimen los valores del sensor en el Serial Monitor y se envían a la central por medio de BLE.
          Serial.print("Distancia de objeto con el portón: "); // print it
          Serial.println(proximity);  
          SecurityLevelChar.writeValue(proximity);
        }      
      }    
    }
    
  }
  digitalWrite(LED_BUILTIN, LOW); // se detiene la conexión BLE, se apaga el LED
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());  
}

