//Laboratorio de microcontroladores
//laboratorio2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//variables globales


int unsigned presionado = 0; //boton presionado
int unsigned estado = 0;
int unsigned contador_c = 0;
int unsigned tiempo_segundos = 0;
int unsigned tiempo_init = 0;

//estados de la maquina de estados

#define estado1 0 //segun e diagrama de señales que esta en la guía.
#define estado2 1
#define estado3 2
#define estado4 3
#define estado5 4

//estados tiempo
#define T_p 5 //este estado lo intentamos usar para ver como parpadea el peatonal
#define T_v 6 //este estado lo intentamos usar para ver como parpadea el peatonal

//definicion metodos
int contador_tiempo();
void interrupcion_tiempo();
void interrupcion_externa();
void maquina_estados();
void Setup_INTERRUPCIONES();

/*
(1 << PB7) led verde carros
(1 << PB6) led rojo carros
(1 << PB5) led VERDE peatonal 1
(1 << PB4) led rojo peatonal 1
(1 << PB3) led verde peatonal 2
(1 << PB2) led ROJO peatonal 2
(1 << PB0) presionado
nota peatonal 1 y 2 funcionan de la misma manera
*/
//interrupciones
ISR (INT0_vect)
{
presionado = 1; //Esta interrupcion me pone el valor de Pulso_presionadoes=1
}
//INTERRUPCION DE TIEMPO PARA MANEJAR DELAY (Basado en link de referencia)
ISR (TIMER0_OVF_vect)      //Interrupt vector for Timer0
{
  contador_tiempo();
}


void interrupcion_tiempo() 
{
 TCCR0A = 0x00; //Modo normal
 TCCR0B = 0x00; 
 TCCR0B |= (1 << CS00) | (1 << CS02); //Prescaling a 1024 
 sei();// Interrupción global
 TCNT0 = 0;
 TIMSK |= (1 << TOIE0); //Habilitando la interrupción del timer0 
}

void interrupcion_externa()  
{
  DDRB |= (1 << PB7)|(1 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2); //Valores de salida
  GIMSK |= (1<<INT0);     // se habilita como entrada PB0
  MCUCR |= (1<<ISC01)|(1<<ISC10)|(1<<ISC11); // Configurando como flanco  para que genere interrupciones en flancos en alto, en bajo.
}

void Interrupciones(){
    interrupcion_externa();
    interrupcion_tiempo();
}
//funciones de tiempo

int contador_tiempo(){
    if (contador_c==63) //aqui se intenta conseguir un delay de 1s
  {
    contador_c=0; //aqui se reinicia para repetir el contador
    ++tiempo_segundos;
  }
  else{
    contador_c++; 
  }  //se incrementa el contador hasta 63 ciclos
   return tiempo_segundos; 
}

void maquina_estados(void){
       if(presionado ==1){
            switch(estado) {
                
                case estado1:
              
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);

                if(tiempo_init>=3){

                
                tiempo_init = contador_tiempo();
                estado = estado2;
                }
                else{
                  estado = estado1;
                }
                
                break;
                
                case estado2:
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                if(tiempo_init>=1){

                tiempo_init = contador_tiempo();
                estado = estado3;
                }
                else{
                  estado = estado2;
                }
                break ;


                case estado3:
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                if(tiempo_init>=10){

                tiempo_init = contador_tiempo();
                estado = estado4;
                }
                else{
                  estado = estado3;
                }
                break ;

                case estado4:
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                //contador_tiempo();
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                //contador_tiempo();
                //PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                //contador_tiempo();
                if(tiempo_init>=3){
                tiempo_init = contador_tiempo();
                presionado = 0;
                }
                else{
                  estado = estado4;
                }
                break;
                
            }
          }
        else{
            PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
        }
}

int main(void)
{
  PORTB &= (0 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4) |(0 << PB3)|(0 << PB2); // Se entablecen los pines como salidas
  presionado = 0; // Se inicializa en estado paso de vehiculos
  
  Interrupciones();
  while (1) {
    maquina_estados(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}