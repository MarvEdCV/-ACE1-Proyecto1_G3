
#include <LiquidCrystal.h>
//LiquidCrystal lcd(33,32,27,28,29,30);
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
/* CODIGO PRUEBA EDUARDO */
int led=13;
char entrada;
/*FIN EDUARDO*/

byte lock[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11011,
  0b11011,
  0b11111,
  0b00000
};

byte smile[8] = {
 0b00001010,
 0b00001010,
 0b00001010,
 0b00000000,
 0b00000000,
 0b00010001,
 0b00001110,
 0b00000000,
};

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

void setup() {

  lcd.createChar(0, lock);
  lcd.createChar(1, smile);
  
  tiempo1 = millis();
  
  lcd.begin(16,2);
  lcd.clear();

  //INICIO EDUARDO
    pinMode(13,OUTPUT); //Declaramos el pin como salida
  //FIN EDUARDO
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempo2 = millis();
  
  if (tiempo2 < (tiempo1 + 1000)){//hay que subirlo a 10000
    showScreen();
  }else if (tiempo2 > (tiempo1 + 1000)){//hay que subirlo a 10000
    //timer1.update();
    //timer2.update();
    lcd.setCursor(0,0);
    lcd.print("  Esperando...   ");

    lcd.setCursor(0,1);
    lcd.print("                   ");
  }
 
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

void showScreen(){
  lcd.setCursor(3,0);
  lcd.print("Bienvenido");

  //lock
  lcd.setCursor(1,0); //columna y fila
  lcd.write(byte(0));

  //lock
  lcd.setCursor(14,0); //columna y fila
  lcd.write(byte(0));

  lcd.setCursor(0,1);
  lcd.print("Grupo3-Seccion B");
}
