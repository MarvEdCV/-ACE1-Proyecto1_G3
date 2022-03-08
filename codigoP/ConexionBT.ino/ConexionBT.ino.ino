/* CODIGO PRUEBA EDUARDO */
int led=13;
char entrada;
int pinBuzzer = 53;
/*FIN EDUARDO*/

void setup() {
    pinMode(pinBuzzer, OUTPUT);
    noTone(pinBuzzer);
  // put your setup code here, to run once:
   //INICIO EDUARDO
   pinMode(13,OUTPUT); //Declaramos el pin como salida
  //FIN EDUARDO
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   //INICIO EDUARDO
    if(Serial.available()>0){
      entrada = Serial.read();
    }
    if(entrada=='E'){;
      digitalWrite(led,HIGH);
    }
    if(entrada=='A'){
      digitalWrite(led,LOW);
    }
  //FIN EDUARDO
}
