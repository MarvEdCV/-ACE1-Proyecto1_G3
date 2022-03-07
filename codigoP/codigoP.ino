#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Stepper.h>

int stepsPerRevolution = 40;
int motorSpeed = 2;
int portonAbierto = 22;//led porton abierto ROJO
int portonCerrado = 23;//led porton cerrado AMARRILLO



int contador = 0;
Stepper motor1(stepsPerRevolution, A12, A13, A14, A15);
Stepper motor2(stepsPerRevolution, A8, A9, A10, A11);

int periodo =5000;
unsigned long TiempoAhora=0;

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

// TOKEN
String toks[4];
String letrasToks[4] = {"A","B","C","D"};

// BUZZER
int pinBuzzer = 53;

void setup() {
  //leds de prueba de puertas
  pinMode(portonAbierto, OUTPUT);//PA
  pinMode(portonCerrado, OUTPUT);//PC

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
        delay(5000); //tiempo 5 segundos de espera cuando se conecta

        
        genToken();
        validarToken();
     }else{ //si no esta conectado a bluethoo que espere
         //timer1.update();
        //timer2.update();
        lcd.setCursor(0,0);
        lcd.print("  Esperando...   ");
    
        lcd.setCursor(0,1);
        lcd.print("                   ");
      }
  }
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

void genToken(){
  long randomNumber = random(10);  // Generate a random number between 0 and 10
  toks[0] = String(randomNumber);

  randomNumber = random(0, 10); // Generate a random number between 0 and 10
  toks[1] = String(randomNumber);

  for (int i = 0; i < 2; i++){
    randomNumber = random(0, 4);
    toks[i+2] = String(letrasToks[randomNumber]);
  }
  
  // Impresion TOKEN en LCD
  lcd.setCursor(5,1);
  lcd.print("TOKEN: ");
  lcd.setCursor(12,1);
  lcd.print(toks[0]);
  lcd.setCursor(13,1);
  lcd.print(toks[1]);
  lcd.setCursor(14,1);
  lcd.print(toks[2]);
  lcd.setCursor(15,1);
  lcd.print(toks[3]);

  //aca mando token a aplicacion
  delay(1000);
}

void validarToken(){
  int intentos = 0;
  String idToken = "";
  char key;
  
  lcd.clear();
  do{
    if (intentos < 3){
      lcd.setCursor(0, 0);
      lcd.print("DIGITE SU TOKEN:");
      lcd.setCursor(0, 1);
      lcd.print(idToken);
      
      
      while(true){
        key = keypad.getKey();
        if(key != NO_KEY){
          //Serial.println("KeyPad: " +String(code));
          if (key == '*'){
            if (comprobarToken(idToken)){
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(" TOKEN CORRECTO ");
              lcd.setCursor(0, 1);
              lcd.print("   "+idToken);
              delay(2000);
              //conectar entrada al parqueo
              lcd.clear();
              entradaParqueo();
            }else{
              tone(pinBuzzer, 500); // digitalWrite(buzzerP, HIGH);
              
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("TOKEN INCORRECTO ");
              lcd.setCursor(0, 1);
              lcd.print("   "+idToken);
              intentos++;
               idToken = "";
              delay(2000);
              lcd.clear();
              noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
              break;
            }
          }else{
            idToken += key;
            break;
          }
        }
      }
  
      if (idToken.length() > 4){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("EXCEDE SIZE");
        lcd.setCursor(0, 1);
        lcd.print("  DEL TOKEN   ");
        delay(2000);
        idToken = "";
        lcd.clear();
        
        intentos++;
      }
    }else{
      break;
    }
    
  }while (key != '#');

  if (intentos == 3){
    //suena la alrma de exceder 3 intentos
    tone(pinBuzzer, 500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TOKEN INVALIDO");
    lcd.setCursor(0,1);
    lcd.print("Excedio 3 intents");
    delay(2000);
    noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
  }else{
    //suena la alarma por presionar cancel
    tone(pinBuzzer, 500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("BOTON CANCEL");
    lcd.setCursor(0,1);
    lcd.print("SE ACTIVO");
    delay(2000);
    noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
  }
  
}

bool comprobarToken(String pass){

  if(pass.length() != 4){
    return false;
  }
  
  if (String(pass[0]) != toks[0]){
    return false;
  }else if (String(pass[1]) != toks[1]){
    return false;
  }else if (String(pass[2]) != toks[2]){
    return false;
  }else if (String(pass[3]) != toks[3]){
    return false;
  }
  
  return true;
}

void entradaParqueo(){
  while (true){
    lcd.setCursor(0,0);
    lcd.print("ENTRANDO PARQUEO");
    lcd.setCursor(0,1);
    controlPuertas();
    lcd.setCursor(0,0);
    lcd.print("Listo");
     lcd.setCursor(0,1);
    
  }
}

void controlPuertas(){
  if(contador<1){
  for (int i = 0; i < 1; i++) {
    TiempoAhora = millis();
     abrirPorton();
     digitalWrite(portonAbierto, LOW);
     lcd.setCursor(0,0);
     lcd.print("- Ingreso vehiculo -");
      lcd.setCursor(0,1);
     while(millis() < TiempoAhora+periodo){
    // espere [periodo] milisegundos
    }
    delay(5000);
    cerrarPorton();
    digitalWrite(portonCerrado, LOW);
    contador=contador+1;
  }
  }
}
void abrirPorton() {            // 2 vueltas derecha
  lcd.setCursor(0,0);
  lcd.print("- abriendo -");
   lcd.setCursor(0,1);
 digitalWrite(portonAbierto, HIGH);
  motor1.setSpeed(10); 
  motor1.step(stepsPerRevolution);
  delay(1000);
}

void cerrarPorton() {            // 2 vueltas derecha
  lcd.setCursor(0,0);
  lcd.print("- cerrando -");
  lcd.setCursor(0,1);
 digitalWrite(portonCerrado, HIGH);
  motor2.setSpeed(10); 
  motor2.step(-stepsPerRevolution);
  delay(1000);
}
