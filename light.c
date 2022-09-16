//Laboratorio de microcontroladores
//laboratorio2

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//variables globales

int unsigned presionado = 0;
int unsigned presionado = 0; //pulso presionado
int unsigned estado = 0;
int unsigned contador_c = 0;
int unsigned tiempo_segundos = 0;
int unsigned tiempo_init = 0;

//estados maquina de estados

#define estado1 0 //segun e diagrama de señales que esta en la guía.
#define estado2 1
#define estado3 2
#define estado4 3
#define estado5 4

//estados tiempo
#define T_p 5
#define T_v 6

//definicion metodos
void contador_tiempo();
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
  DDRB |= (1 << PB7)|(1 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2)|(1 << PB0); //Valores de salida
  GIMSK |= (1<<INT0);     // se habilita como entrada PB0
  MCUCR |= (1<<ISC01)|(1<<ISC10)|(1<<ISC11); // Configurando como flanco  para que genere interrupciones en flancos en alto, en bajo.
}

void Interrupciones(){
    interrupcion_externa();
    interrupcion_tiempo();
}
//funciones de tiempo

void contador_tiempo(){
    if (contador_c==63) //waiting for 63 because to get 1 sec delay
  {
    contador_c=0; //making intr_count=0 to repeat the count
    ++tiempo_segundos;
  }
  else  contador_c++; //incrementing contador upto 63

}
/*
//###########################################################################
void Semaforos_FMS(){
  switch (estado){
  //CASO LUZ DE PASO DE VEHICULOS  
  case LDPV:
      estado_t=T_Vehiculos; //Estado que permite hacer parpadear las luces luego de pasar del estado LDPV
      PORTB = (0<<PB3)|(1<<PB2)|(1<<PB2)|(0<<PB0); //Luz de paso vehicular se enciende y Luz Roja de peatones tambien 
      if(Pulso_presionadoes==0) {
        estado=LDPV;
      }
      else if(Pulso_presionadoes==1){
            if(sec>=10){
              Contador_ciclos=0; //Reinicio de contador
              sec=0;
              estado=LDP;
            }
            } 
            break;
  // CASO LUZ DE PARPADEO          
  case LDP:
    if(sec<3){
        if(estado_t==T_Vehiculos){
            if(Contador_ciclos==30||Contador_ciclos==90||Contador_ciclos==150) PORTB = (0<<PB3)|(0<<PB2)|(1<<PB2)|(0<<PB0);
            else if(Contador_ciclos==60||Contador_ciclos==120) PORTB = (0<<PB3)|(1<<PB2)|(1<<PB2)|(0<<PB0);
        }
        if(estado_t==T_Peatones){
            if(Contador_ciclos==30||Contador_ciclos==90||Contador_ciclos==150) PORTB = (1<<PB3)|(0<<PB2)|(0<<PB2)|(0<<PB0);
            else if(Contador_ciclos==60||Contador_ciclos==120) PORTB = (1<<PB3)|(0<<PB2)|(0<<PB2)|(1<<PB0);
        }
    
    }
    else {
        if(estado_t==T_Vehiculos){
            sec=0;
            Contador_ciclos=0;
            estado= LDVD;
        }
        if(estado_t==T_Peatones){
            sec=0;
            Contador_ciclos=0;
            estado= LDPD;
        }
        
    }
    break;
  //CASO LUZ DE VEHICULOS DETENIDOS          
  case LDVD:
    PORTB = (1<<PB3)|(0<<PB2)|(1<<PB2)|(0<<PB0);
    if(sec>=1){
    estado= LDPP;
    Contador_ciclos=0;
    sec=0;
    }
    else estado=LDVD;
    break; 
  //CASO LUZ DE PEATONES PASE
  case LDPP:
    PORTB =(1<<PB3)|(0<<PB2)|(0<<PB2)|(1<<PB0);
    estado_t=T_Peatones;
    if(sec>=10){
      estado= LDP;
      Contador_ciclos=0;
      sec=0;  
    }
    else estado=LDPP;
    break;
  //CASO LUZ DE PEATONES DETENIDOS
  case LDPD:
    PORTB = (1<<PB2)|(1<<PB3);
    if(sec>=1){
    estado= LDPV;if(sec>=10){

                Contador_ciclos=0; //Reinicio de contador
                sec=0;
                estado=LDP;
            }
    Contador_ciclos=0;
    sec=0;
    Pulso_presionadoes=0; //Se regresa el valor del presionado a 0
    }
    else estado=LDPD;
    break; 
  }
}

int main(void)
{
  PORTB &= (0<<PB0) | (0<<PB2)| (0<<PB2)|(0<<PB3); // Se entablecen los pines B0,B1,B2 y B3 como salidas
  estado = LDPV; // Se inicializa en estado paso de vehiculos
  Pulso_presionadoes = 0; //Se inicializa el valor del pulso de los presionadoes en 0
  Setup_INTERRUPCIONES();
  while (1) {
    Semaforos_FMS(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}
int main (void)
{

}
/*

*/
int maquina_estados(void){
       if(presionado ==1){
            switch(estado) {
                
                case estado1:
              
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (0 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(1 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);

                if(tiempo_segundos>=3){

                contador_c=0; //Reinicio de contador
                tiempo_segundos=0;
                estado = estado2;
                }
                else{
                  estado = estado1;
                }
                
                break;
                
                case estado2:
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(0 << PB4) |(1 << PB3)|(0 << PB2);
                if(tiempo_segundos>=1){

                contador_c=0; //Reinicio de contador
                tiempo_segundos=0;
                estado = estado3;
                }
                else{
                  estado = estado2;
                }
                break ;


                case estado3:
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                if(tiempo_segundos>=10){

                contador_c=0; //Reinicio de contador
                tiempo_segundos=0;
                estado = estado4;
                }
                else{
                  estado = estado3;
                }
                break ;

                case estado4:
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(0 << PB5) |(1 << PB4) |(0 << PB3)|(1 << PB2);
                delay(5);
                PORTB = (1 << PB7)|(0 << PB6)|(1 << PB5) |(1 << PB4) |(1 << PB3)|(1 << PB2);
                delay(5);
                if(tiempo_segundos>=3){
                contador_c=0; //Reinicio de contador
                tiempo_segundos=0;
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
  PORTB &= (0 << PB7)|(0 << PB6)|(0 << PB5) |(0 << PB4) |(0 << PB3)|(0 << PB2)|(0 << PB0); // Se entablecen los pines como salidas
  presionado = 0; // Se inicializa en estado paso de vehiculos
  
  Interrupciones();
  while (1) {
    maquina_estados(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}