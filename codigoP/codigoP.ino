#include <Keypad.h>
#include <LiquidCrystal.h>

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

// LiquidCrystal ------------------------
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
// END LiquidCrystal --------------------

// Keypad ------------------------
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'7', '8', '9', 'A'},
  {'4', '5', '6', 'B'},
  {'1', '2', '3', 'C'},
  {'*', '0', '#', 'D'}
}; // * es OK y # es cancel

byte rowPins[rows] = {2, 3, 4, 5};
byte colPins[cols] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
// END Keypad --------------------

void setup() {

  //EMOJIS
  lcd.createChar(0, lock);
  lcd.createChar(1, smile);
  
  tiempo1 = millis();
  
  //INICIO LCD
  lcd.begin(16,2);
  lcd.clear();

  //INICIO EDUARDO
   pinMode(13,OUTPUT); //Declaramos el pin como salida
  //FIN EDUARDO
  Serial.begin(9600);
}

void loop() {
  bool conection = true;
  // put your main code here, to run repeatedly:
  tiempo2 = millis();
  
  if (tiempo2 < (tiempo1 + 1000)){//hay que subirlo a 10000
    showScreen();
  }else if (tiempo2 > (tiempo1 + 1000)){//hay que subirlo a 10000
    char code = keypad.getKey();
    
     if (conection){ //Si esta conectado a Bluetooh que entre
        lcd.setCursor(0,0);
        lcd.print(" *USUARIO*     ");
    
        lcd.setCursor(0,1);
        lcd.print("                   ");
      
        if(code != NO_KEY){
          Serial.println("KeyPad: " +String(code));
        }
     }else{ //si no esta conectado a bluethoo que espere
         //timer1.update();
        //timer2.update();
        lcd.setCursor(0,0);
        lcd.print("  Esperando...   ");
    
        lcd.setCursor(0,1);
        lcd.print("                   ");
      }
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
  lcd.print("Grupo3-Seccion A");
}
