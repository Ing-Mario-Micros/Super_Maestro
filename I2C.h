/* 
 * File:   I2C.h
 * Author: mario
 *
 * Created on 1 de agosto de 2022, 08:59 AM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include<xc.h>
char Vector_Datos[12];
/*------------------------------ Funciones I2C ---------------------*/
void Activar_I2C_Maestro(void);
void Start(void);
void Stop(void);
void Rstart(void);
unsigned char LecturaDato(unsigned char,unsigned char);
/*------------------------------ Funciones para DS ---------------------*/
void Enviar_Dato_I2C(unsigned char);
unsigned char Recibir_Dato_I2C(unsigned char);
void Enviar_Dato_DSPIC(unsigned char,unsigned char,unsigned char);
unsigned char Recibir_Dato_DSPIC(unsigned char,unsigned char);
/*------------------------------ Funciones I2C ---------------------*/
void Activar_I2C_Maestro(void){
  SSPCON1=0b00101000;
  SSPCON2=0b00000000;
  SSPSTAT=0b11000000;
  SSPADD=120;            //100kHz Calculo necesario SSPADD=(Fosc/4)/FrecI2C 
}
void Start(void){    
  SEN=1;
  while(SEN==1);  
}
void Stop(void){
  PEN=1;
  while(PEN==1);  
}
void Rstart(void){
  RSEN=1;
  while(RSEN==1);  
}
unsigned char LecturaDato(unsigned char Direccion, unsigned char Registro){
  Start();
  SSPBUF=Direccion & 0b11111110;
  SSPIF=0;
  while(SSPIF==0);
  SSPBUF=Registro;
  SSPIF=0;
  while(SSPIF==0);
  Rstart();
  SSPBUF=Direccion | 0b00000001;
  SSPIF=0;
  while(SSPIF==0);
  RCEN=1;
  SSPIF=0;
  while(SSPIF==0);
  
  SSPIF = 0;
  ACKDT = 1;
  ACKEN = 1;
  while(SSPIF==0);
  
  
  Stop();
  return SSPBUF;  
}
/*------------------------------ Funciones para DS ---------------------*/
void Enviar_Dato_I2C(unsigned char Dato){
    while(RB2 == 1);
    __delay_ms(250);
    SSPBUF= Dato; //| 0b00000001; //& 0b11111110;
    SSPIF=0;
    while(SSPIF==0);
}
unsigned char Recibir_Dato_I2C(unsigned char Dato_Adicional){
    while(RB2 == 1);
    __delay_ms(500);
    RCEN=1;
    SSPIF=0;
    while(SSPIF==0);
    //LATD=SSPBUF;
    //LATE2=1;

    /** Envio de ACK **/
    SSPIF=0;
    if(Dato_Adicional==1) ACKDT=0;
    else ACKDT=1;
    ACKEN=1;
    while(SSPIF==0);
    //LATE2=0;
    return SSPBUF;
}
void Enviar_Dato_DSPIC(unsigned char Direccion,unsigned char Registro, unsigned char Valor){
  char Verificador_Temp;
  /** Envio de Start **/
  while(RB2 == 1);
  __delay_ms(500);
  Start();
  LATC2=1;
  LATC1=1;
  /** Envio de Dirección **/
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=0;
  /** Envio de registro a trabajar **/
  Enviar_Dato_I2C(Registro);
  LATC2=1;
  /** Envio de valor al Registro **/
  Enviar_Dato_I2C(Valor);
  LATC2=0;
  /** Envio de Dirección modo Lectura **/
  Rstart();
  Enviar_Dato_I2C(Direccion | 0b00000001);
  LATC2=1;
  /** Resepción de Dato **/
  Verificador_Temp=Recibir_Dato_I2C(0);
  LATC2=0;
  /** Envio de Dirección modo Escritura **/
  Rstart();
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=1;
  /** Envio de valor al Registro **/
  while(RB2 == 1);
  __delay_ms(500);
  if(Registro==Verificador_Temp){
      Enviar_Dato_I2C(2);
      LATC0=1;
  }
  else{
      Enviar_Dato_I2C(1);
      LATC0=0;
  }
  LATC2=0;
  /** Envio de Stop **/
  while(RB2 == 1);
  __delay_ms(500);
  Stop();
  LATC2=1;
  LATC1=0;
}
unsigned char Recibir_Dato_DSPIC(unsigned char Direccion , unsigned char Registro){
    char Dato_Temp , Verificador_Temp;
    /** Envio de Start **/
  while(RB2 == 1);
  __delay_ms(500);
  Start();
  LATC2=1;
  LATC1=1;
  
  /** Envio de Dirección **/
  Enviar_Dato_I2C(Direccion & 0b11111110);
  LATC2=0;
  
  /** Envio de registro a trabajar **/
  Enviar_Dato_I2C(Registro);
  LATC2=1;
  
  /** Envio de Dirección modo Lectura **/
  Rstart();
  Enviar_Dato_I2C(Direccion | 0b00000001);
  LATC2=0;
  
  /** Resepción de Dato **/
  Dato_Temp=Recibir_Dato_I2C(1);
  LATC2=1;
  
  /** Resepción de Dato **/
  Verificador_Temp=Recibir_Dato_I2C(0);
  LATC2=0;
  
  /** Envio de Stop **/
  while(RB2 == 1);
    __delay_ms(500);
  Stop();
  if(Registro==Verificador_Temp){
      Vector_Datos[Registro]=Dato_Temp;
      
      LATC2=1;
      LATC1=0;
      return 1; 
  }
  else{
      
      LATC2=1;
      LATC1=0;
      return 0;
  }
}

#endif	/* I2C_H */

