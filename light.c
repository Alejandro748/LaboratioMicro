//Laboratorio de microcontroladores
//laboratorio2

#include <avr/io.h>
#include < util/delay.h>

//variables globales

int unsigned boton;
int unsigned ldpv; //paso vehiculos
int unsigned ldvd; //paso peatones
int unsigned ldpp; //vehiculos detenidos
int unsigned ldpd; //peatones detenidos

DDRB = 0x08 ; //Configuracion del puerto ¿Necistamos usar los 8bit?
//ESTADOS:
//estado1=paso vehicular
//estado2=parpadeo de la luz amarilla vehicular
//estado3= paso peatonal
//estado4= parpadeo de la luz peatonal
//estado5?

int main (void)
{

}
/*
Se tienen que aplicar las interrupciones en los cases, de la forma: 
int main(void)
{
  PORTB &= (0<<PB0) | (0<<PB1)| (0<<PB2)|(0<<PB3); // Se entablecen los pines B0,B1,B2 y B3 como salidas
  estado = LDPV; // Se inicializa en estado paso de vehiculos
  Pulso_botones = 0; //Se inicializa el valor del pulso de los botones en 0
  Setup_INTERRUPCIONES();
  while (1) {
    Semaforos_FMS(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}

*/
int casos(void){
    switch( boton) {
       case presionado:
            switch(estado) {
   //án los siguientes paquetes:
             case estado1:
                ldpv = 1; 
                ldvd = 0; 
                ldpp = 0; 
                ldpd = 1;
                estado = estado2;
                break;
                
                case estado2:
                ldpv = 0;
                delay(5);
                ldpv = 1;
                delay(5);
                ldpv = 0;
                delay(5);
                ldpv = 1;
                delay(5);
                ldpv = 0;
                delay(5);
                ldpv = 1;
                delay(5);
                ldvd = 0;
                ldpp = 0;
                ldpd = 1;
                estado = estado3;
                break;
                
                case estado3:
                ldpv = 0;
                ldvd = 1;
                ldpp = 0;
                ldpd = 1;
                estado = estado4;
                break ;
                case estado4:
                ldpv = 0;
                ldvd = 1;
                ldpp = 1;
                ldpd = 0;
                estado = estado5;
                break ;
                case estado5:
                ldpv = 0;
                ldvd = 1;
                ldpp = 0;
                delay(5);
                ldpp = 1;
                delay(5);
                ldpp = 0;
                delay(5);
                ldpp = 1;
                delay(5);
                ldpp = 0;
                delay(5);
                ldpp = 1;
                delay(5);
                ldpd = 0;
                boton = no_presionado;
                break;
                

        case no_presionado:
            ldpv = 1; 
            ldvd = 0; 
            ldpp = 0; 
            ldpd = 1;
            break;
    }
}

