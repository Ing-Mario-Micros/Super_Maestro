/*
 * File:   MainI2C_Controlador.c
 * Author: mario
 *
 * Created on 9 de Agosto de 2022, 10:36 AM
 */

//Envió el cpodigo de I2C:

#include<xc.h>
#define _XTAL_FREQ 48000000
#include "I2C.h"
#include "RS232.h"
#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HSPLL_HS  //Configuración de fuente de reloj interna
#pragma config WDT=OFF
#pragma config PBADEN=OFF
#pragma LVP=OFF                 //Programación de Bajo voltaje Desactivada

void __interrupt () ISR (void); //Interrupción 

/*------------------------------ variables del sistema -----------------*/
extern char Vector_Datos[12] ;
extern char BufferR2[];

void main(void){
  OSCCON=0b01110000;
  __delay_ms(1);
  TRISD=0x00;
  LATD=0;
  TRISB=0xFF;
  TRISC=0;
  LATC=0;
  Activar_I2C_Maestro();
  Activar_RS232();
  /* Configuración de interrupciones */
  GIE=1;    //Habilitación de las interrupciones generales
  while(1){
    __delay_ms(10);
      MensajeRS232("Recibir Dato On");
      Transmitir('\n');
      if(1== Recibir_Dato_DSPIC(0x10,0x1)){
          LATC0=1;
      }
      else{
          LATC0=0;
      }
      MensajeRS232("Recibir Dato Off");
      Transmitir('\n');
      
      LATD=Vector_Datos[0x1];
      if(1== Recibir_Dato_DSPIC(0x10,0x0)){
          LATC0=1;
      }
      else{
          LATC0=0;
      }
      ImprimirDecimal(2.2);
      LATD=Vector_Datos[0x0];
      MensajeRS232(" ");
      MensajeRS232(BufferR2);
      if(BufferR2[0]-48>0 && BufferR2[0]-48<9){
            Enviar_Dato_DSPIC(0x10,0x11,((BufferR2[0]-48)*10)+20);
        }
    if(BufferR2[0]-48==9){
        Enviar_Dato_DSPIC(0x10,0x11,200);
    }
    if(BufferR2[0]-48==0){
         Enviar_Dato_DSPIC(0x10,0x11,0);
    }
      
    LATC2=1;
    __delay_ms(1000);
    LATC2=0;
    __delay_ms(1000);
  }  
}
/* Función de interrupción */
void __interrupt () ISR (void){
     if(RCIF==1){   //Bandera de Interrupción en Cero
        Interrupcion_RS232();
     }
}




