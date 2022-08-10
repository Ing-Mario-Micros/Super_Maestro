/* 
 * File:   RS232.h
 * Author: mario
 *
 * Created on 9 de agosto de 2022, 08:41 PM
 */

#ifndef RS232_H
#define	RS232_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#define Tam_Vec 20              //Define el tamaño del Vector de recepción RS232


char b = 0;          //Variable contadora de recepción
char BufferR2[Tam_Vec];   //Vector de guardado de recepción

void Interrupcion_RS232 (void);
/* Configurar RS232 */
void Activar_RS232(void);
/* Función imprimir Cadena de caracteres */
void MensajeRS232(char *);    //Función para imprimir cadena de Caracteres
/*  Funcion para imprimer el numero decimal  */
void ImprimirDecimal (float); //Función para imprimir un valor decimal en el puerto serial


/*  Rs232 Funciones de transmición y recepción*/
void Transmitir(unsigned char); //Función encargada de enviar datos Seriales
unsigned char Recibir(void);    //Función encargada de Resivir datos Seriales



void Interrupcion_RS232 (void){
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
/* Configurar RS232 */
void Activar_RS232(void){
    /* Configuración del puerto Serial*/
  TXSTA=0b00100000;     //Habilitación de trasnmición de datos
  RCSTA=0b10010000;     //Encendido del Puerto Serial y habilitación de Recepción
  BAUDCON=0b00000000;   //Configuración por Tabla 20-3 (9600Baudio)
  SPBRG=77;             //Configuración por Tabla 20-3 (9600Baudio)
  /* Configuración de interrupciones */
  RCIF=0;   //Bandera de Interrupción en Cero
  RCIE=1;   //Activación de la Interrupción para recepción de datos
  PEIE=1;   //Habilitaciónon de las interrupciones por periferico
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
#endif	/* RS232_H */

