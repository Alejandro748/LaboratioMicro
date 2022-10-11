//Laboratorio de microcontroladores
//laboratorio2
//María José Arce B60561
//Alejandro Chacon B61871
//---------------------------------------------------------------------------------------

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//---------------------------------------------------------------------------------------
//variables globales


int unsigned presionado = 0; //pulso boton
int unsigned estado = 0;
int unsigned contador_c = 0;
int unsigned tiempo_segundos = 0;

//---------------------------------------------------------------------------------------
//estados maquina de estados

#define estado1 0 //segun el diagrama de señales que esta en la guía.
#define estado2 1
#define estado3 2
#define estado4 3

//---------------------------------------------------------------------------------------


//definicion metodos
//metodos guiados por la siguiente referencia: https://www.gadgetronicx.com/attiny85-timer-tutorial-generating-time-delay-interrupts/ 2: https://www.gadgetronicx.com/attiny85-external-pin-change-interrupt/
void interrupcion_tiempo();
void interrupcion_externa();
void Setup_INTERRUPCIONES();
//metodos creados para la lógica
void contador_tiempo();
void maquina_estados();

//---------------------------------------------------------------------------------------
/* Definición de LEDS
(1 << PB7) led ROJO carros
(1 << PB6) led VERDE carros
(1 << PB5) led ROJO peatonal 1
(1 << PB4) led VERDE peatonal 1
(1 << PB3) led ROJO peatonal 2
(1 << PB2) led VERDE peatonal 2
(1 << PB0) Led amarillo VEHICULAR
(1 << PB1) Led amarillo PEATONAL

(1 << PD2) entrada boton
nota peatonal 1 y 2 funcionan de la misma manera y usan el mismo led amarillo
*/

//---------------------------------------------------------------------------------------
//interrupciones
ISR (INT0_vect){
presionado = 1; // Sirve para poner el pulso en 1.
}
//SIrve para manejar el contador de tiempo o delay. 
ISR (TIMER0_OVF_vect){
  contador_tiempo();
}


void interrupcion_tiempo(){
 TCCR0A = 0x00; //Modo normal
 TCCR0B = 0x00; 
 TCCR0B |= (1 << CS00) | (1 << CS02); //Prescaling a 1024 para hacer que la frecuencia sea más pequeña
 sei();// Interrupción global
 TCNT0 = 0;
 TIMSK |= (1 << TOIE0); //Habilitando la interrupción del timer0 
}

void interrupcion_externa(){
  DDRB |= (1 << PB7)|(1 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2)|(1 << PB1); //Valores de salida se usa todo el puerto B
  GIMSK |= (1<<INT0);     // se habilita como entrada PD2
  MCUCR |= (1<<ISC01)|(1<<ISC10)|(1<<ISC11); // Configurando como flanco  para que genere interrupciones en flancos en alto, en bajo.
}

void Interrupciones(){
    interrupcion_externa();
    interrupcion_tiempo();
}

//---------------------------------------------------------------------------------------
//funcion de tiempo

void contador_tiempo(){
    if (contador_c==63) //63 ciclos = 1 segundo
  {
    contador_c=0; // resetamos el contador 
    tiempo_segundos = tiempo_segundos +1; //incrementamos los segundos transcurridos
  }
  else{
    contador_c = contador_c + 1; 
  }  //incrementamos contador

}
//---------------------------------------------------------------------------------------
//metodo lógico
void maquina_estados(void){
       if(presionado ==1){ //revisa si ya se presiono el boton
            switch(estado) {
                
                case estado1: //empieza parpadeando
 
                  if (tiempo_segundos < 3){
                    if(contador_c==15||contador_c==45||contador_c==90){
                      PORTB = (0 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2)|(1 << PB1);
                    }
                    else if(contador_c==10||contador_c==65||contador_c==120){
                      PORTB = (0 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2)|(0 << PB1);
                    }

                  }
                //revisamos si cumple tiempo para pasar al estado 2 si no lo cumple entonces continua en el estado 1
                
                if(tiempo_segundos==3){

                  estado = estado2;
                  contador_c = 0;
                  tiempo_segundos = 0;
                }
                else{
                  estado = estado1;
                }
                
                break;
                
                case estado2: // 
                
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2)|(0 << PB1);
                
                if(tiempo_segundos>=1){

                
                estado = estado3;
                contador_c = 0;
                tiempo_segundos = 0;
              
                
                }
                else{
                  estado = estado2;
                }
                break ;


                case estado3:
                
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2)|(0 << PB1);
                if(tiempo_segundos>=10){

                
                estado = estado4;
                contador_c = 0;
                tiempo_segundos = 0;
                }
                else{
                  estado = estado3;
                }
                break ;

                case estado4:

                  
                  if (tiempo_segundos < 3){
                    if(contador_c ==15||contador_c==45||contador_c==90){
                      PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2)|(0 << PB1);
                    }
                    else if(contador_c==10||contador_c==65||contador_c==120){
                      PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4) |(0 << PB3)|(0 << PB2)|(0 << PB1);
                    }

                  }
                //revisamos si cumple tiempo para pasar al estado 2 si no lo cumple entonces continua en el estado 1
                
                if(tiempo_segundos==3){

                  presionado = 0; //se resetea
                  contador_c = 0;
                  tiempo_segundos = 0;
                  
                }
                else{
                  estado = estado4;
                }
                
                break;
                
            }
          }
        else{ //predeterminado antes de presionar boton
            PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2)|(0 << PB1)|(0 << PB0);
        }
}
//---------------------------------------------------------------------------------------
int main(void)
{
  PORTB &= (0 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4); // Se entablecen los pines como salidas
  presionado = 0; 
  estado = 0; 
  //vamos a empezar suponiendo que no se presiona el boton
  Interrupciones();
  while (1) {
    maquina_estados(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}