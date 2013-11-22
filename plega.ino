
// Modified by Victor B.
#include <SoftwareSerial.h>
#include <MsTimer2.h>
 boolean okt= true;
 int med=0,grd=0,peq=0;
   char code[10]; 
 int  val = 0; 
 int bytesread = 0; 
 int contadorzero=0,contadorunos=0,contatarge=20;
#define rxPin 8
#define txPin 7
#define START_BYTE 0x0A 
#define STOP_BYTE 0x0D
#define rele 12
#define actRfid 9
#define led 11
#define segsin 1
#define segcon 1
/***************************************/
/***************************************/
#define grupo 1
/***************************************/
#define maquina 5
/***************************************/
/***************************************/
boolean ledestado=LOW;
// RFID reader SOUT pin connected to Serial RX pin at 2400bps to pin8

void setup()
{ 
  Serial.begin(9600);  // Hardware serial for Monitor 2400bps
  
  MsTimer2::set(100, flash); // 500ms period
  MsTimer2::start(); //contador rapido
  
  
  pinMode(actRfid,OUTPUT);       // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(actRfid, LOW);    // Activate the RFID reader 
  pinMode(rele,OUTPUT);
  digitalWrite(rele, HIGH);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(led,OUTPUT);
  Serial.print("9999999999,");
  Serial.print(grupo);
  Serial.print(",");
  Serial.print(maquina);
  Serial.println(",0,0,0,0");
  
}


void loop()
{ 
  
 digitalWrite(actRfid, LOW); 
 
 SoftwareSerial RFID = SoftwareSerial(rxPin,txPin); 
 RFID.begin(2400);
  
 int sensorValue1 = digitalRead(2);
 int sensorValue2 = digitalRead(3);

 
 if((val = RFID.read()) == START_BYTE)
  {   // check for header 
  
    bytesread = 0; 
    while(bytesread<10)
    {  // read 10 digit code 
      val = RFID.read(); 
      
      if(!((val>47)&&(val<91)))  {  // if header or stop bytes before the 10 digit reading 
       break; 
         
      }
      code[bytesread] = val;         // add the digit           
      bytesread++;                   // ready to read next digit  
    } 

    if(bytesread == 10)
    {  // if 10 digit read is complete
     contadorzero=0;
    code[bytesread] = ',';
     RFID.flush();
     
     contatarge++;
     
    if(contatarge>(segcon)) {
  
   
   Serial.print(code);
   //Serial.print(",");
   Serial.print(grupo);
   Serial.print(",");
   Serial.print(maquina);
   Serial.print(",");
   Serial.print("0");
   Serial.print(",");
   Serial.print(grd);
   Serial.print(",");
   Serial.print(med);
   Serial.print(",");
   Serial.println(peq);
   contatarge=0;
   
   
    
    }  
     
      //Serial.println(code); 
    
      digitalWrite(rele, LOW);      // print the TAG code 
    }
   
    bytesread = 0; 
    
   delay(random(300,600));
    digitalWrite(actRfid, HIGH); 
    
   

  } 
   else{
     if (contadorzero>1000){
       if (contadorunos>segsin) {
         Serial.print("0000000000,");
         Serial.print(grupo);
         Serial.print(",");
         Serial.print(maquina);
         Serial.println(",0,0,0,0");
         digitalWrite(rele, HIGH);
         contadorunos=0;
       }
   
   delay(random(400,600));
   contadorunos++;
     }
     contadorzero++;
    
    digitalWrite(actRfid, HIGH);
   
   }
} 

/********************************************************/ 

/********************************************************/ 
/***************** Deteccion de toallas *****************/
void toallas(){
  
boolean sensorValue1 = digitalRead(2);
boolean sensorValue2 = digitalRead(3);


if (sensorValue1 && sensorValue2 && okt) {
  med++;
  okt=!okt;
 
}
else if  (!sensorValue1 && sensorValue2 && okt){
  grd++;
  okt=!okt;
}
else if  (sensorValue1 && !sensorValue2 && okt ) {
  peq++;
  okt=!okt;
}
else if (!sensorValue1 && !sensorValue2 && !okt) okt=true;


}
/********************************************************/ 
void flash()
{
 
 toallas();
  if (!digitalRead(rele)) {
    ledestado = !ledestado;
    digitalWrite(led, ledestado);
  }
  else digitalWrite(led, HIGH);;
    
    
}


