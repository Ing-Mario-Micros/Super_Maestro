/*
 * File:   MainI2C_Controlador.c
 * Author: mario
 *
 * Created on 1 de Agosto de 2022, 09:24 AM
 */

//Envió el cpodigo de I2C:

#include<xc.h>
#define _XTAL_FREQ 8000000
#include "I2C.h"
#pragma config FOSC=INTOSC_EC
#pragma config WDT=OFF
#pragma config PBADEN=OFF

#define Tam_Vec 20              //Define el tamaño del Vector de recepción RS232


char b = 0;          //Variable contadora de recepción
char BufferR2[Tam_Vec];   //Vector de guardado de recepción

void ImprimirDecimal (float); //Función para imprimir un valor decimal en el puerto serial
void MensajeRS232(char *);    //Función para imprimir cadena de Caracteres

void Transmitir(unsigned char); //Función encargada de enviar datos Seriales
unsigned char Recibir(void);    //Función encargada de Resivir datos Seriales

void __interrupt () ISR (void); //Interrupción 

/*------------------------------ variables del sistema -----------------*/
extern char Vector_Datos[12] ;

void main(void){
  OSCCON=0b01110000;
  __delay_ms(1);
  TRISD=0x00;
  LATD=0;
  TRISB=0xFF;
  TRISC=0;
  LATC=0;
  Activar_I2C_Maestro();
  /* Configuración del puerto Serial*/
  TXSTA=0b00100000;     //Habilitación de trasnmición de datos
  RCSTA=0b10010000;     //Encendido del Puerto Serial y habilitación de Recepción
  BAUDCON=0b00000000;   //Configuración por Tabla 20-3 (9600Baudio)
  SPBRG=12;             //Configuración por Tabla 20-3 (9600Baudio)
  /* Configuración de interrupciones */
  RCIF=0;   //Bandera de Interrupción en Cero
  RCIE=1;   //Activación de la Interrupción para recepción de datos
  PEIE=1;   //Habilitaciónon de las interrupciones por periferico
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
         RCIF=0;
         BufferR2[b]=Recibir();
         b++;
         if(BufferR2[b-1]=='\n'||b==Tam_Vec){ 
             while(b!=Tam_Vec){
                 BufferR2[b]=0;
                 b++;
             }
             b=0;
         }
     }
}
/* Función imprimir Cadena de caracteres */
void MensajeRS232(char* a){
//Función que escribe una cadena de caracteres variable en la pantalla
//a es una cadena de caracteres guardada en una variable *char
//Ejemplo: char aux[4]="Hola"; MensajeLCD_Var(aux);	
	while (*a != '\0'){
		//Transmitir(*a); 
        if(*a != '\n'){
          Transmitir(*a);  //En el caso de no querer enviar el salto de linea
                           //Se puede usar esta función
        }
		a++;
	}		
}
/*  Funcion para imprimer el numero decimal  */
void ImprimirDecimal (float An){
    char Entero; //prueba de impresion decimal
    int Decimal,aux1,i;
    if(An<0){
        Transmitir('-');
        An=An*(-1);
    }
    Entero=(int)An;             //La Patre entera de la división se guarda en Entero
    An= An-Entero;              //Dejar solamente la parte decimal
    An= An*10000;               //Convierto los decimales a enteros para poder extraerlos
    Decimal = (int)An;          //La Parte decimal de la división se guarda en decimal
    //aux2 = Entero;
    //aux3 = Decimal;
    i=10;
    while(Entero%i!=Entero){
        i=i*10;
    }
    i=i/10;
    aux1=Entero;
    while(i>=1){
        Transmitir((aux1/i)+48);
        aux1=aux1%i;
        i=i/10;
    }
    Transmitir('.');
    i=1000;
    aux1=Decimal;
    while(i>=1){
        Transmitir((aux1/i)+48);
        aux1=aux1%i;
        i=i/10;
    }
    Transmitir(' ');
}

/*  Rs232 Funciones de transmición y recepción*/
void Transmitir(unsigned char BufferT){
  while(TRMT==0);
  TXREG=BufferT;  
}
unsigned char Recibir(void){
    //while(RCIF==0);
    return RCREG;
}
