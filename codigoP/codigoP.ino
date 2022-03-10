#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Separador.h>
#include <EEPROM.h>
#include <LedControl.h>
#include "matrices.h"

int stepsPerRevolution = 40;
int motorSpeed = 2;
int portonAbierto = 22;//led porton abierto ROJO
int portonCerrado = 23;//led porton cerrado AMARRILLO
String tokenM = "";
int contador = 0;
Stepper motor1(stepsPerRevolution, A12, A13, A14, A15);
Stepper motor2(stepsPerRevolution, A8, A9, A10, A11);

int periodo = 5000;
unsigned long TiempoAhora = 0;

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
/* CODIGO PRUEBA EDUARDO */
int led = 13;
String entrada;
String User = "admin"; //Variable que recibe el nombre de usuario para iniciar sesión.
String UserPass = "1234";//Variable que recibe la contraseña del usuario para iniciar sesión.
String User2 = "admin2"; //Variable que recibe el nombre de usuario para iniciar sesión.
String UserPass2 = "12342";
char RegistrarUsuario;
String Usuarios[2] = {"admin-1234", "sara-sarita"};
String Vartmp = "Eduardo-1234,REGISTRAR";
Separador s;//variable para separar usuario de funcion a realizar
String Token = "1234XZ";
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
String letrasToks[4] = {"A", "B", "C", "D"};

// BUZZER
int pinBuzzer = 53;

// Led 8x8
LedControl lc = LedControl(16, 18, 17, 1);
const int pinMatriz[16] = {24, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 52};

void setup() {


  //INICIO EDUARDO
  pinMode(13, OUTPUT); //Declaramos el pin como salida
  //FIN EDUARDO
  //leds de prueba de puertas
  pinMode(portonAbierto, OUTPUT);//PA
  pinMode(portonCerrado, OUTPUT);//PC
  pinMode(A7, INPUT); 

  //BUZZER
  pinMode(pinBuzzer, OUTPUT);
  noTone(pinBuzzer);

  //INICIO LCD
  lcd.begin(16, 2);
  lcd.clear();

  //EMOJIS
  lcd.createChar(0, lock);
  lcd.createChar(1, smile);

  tiempo1 = millis();

  // NUM
    randomSeed(analogRead(A6));
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial3.begin(9600);

 
// No funciona con lo de NUM
  // LED 8X8
  /*
  pinesMatriz();
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  */
}

void loop() {
  
  //INICIO EDUARDO

  /*if(Serial.available()>0){
    entrada = Serial.readString();//Serial.readString();
    String UsuarioContra = s.separa(entrada,',',0);
    String Funcion = s.separa(entrada,',',1);
    if(Funcion == "REGISTRAR"){
      Serial.println("Estoy en registrar");
      Serial1.println("Estoy en registrar");
      //BtnRegistrar(UsuarioContra);
    }else if(Funcion == "LOGIN"){
     // Serial1.println("Estoy en login");
      //Serial.print("Estoy en login");
      BtnLogin(UsuarioContra);
    }else if(Funcion=="TOKEN"){
      Serial.println(Token);
    }

    }



    if(Serial.available()>0){
      entrada = Serial.readString(); //ASI SE LEEN/RECIBEN DATOS DE LA APP
      lcd.setCursor(0,0);
      lcd.print(String(entrada));
    }
    if(entrada=="HOLA"){;
      digitalWrite(led,HIGH);
    }
    if(entrada=="A"){
      digitalWrite(led,LOW);
    }*/

  //FIN EDUARDO

  bool conection = true;
  // put your main code here, to run repeatedly:
  tiempo2 = millis();

  if (tiempo2 < (tiempo1 + 1000)) { //hay que subirlo a 10000
    showScreen();
  } else if (tiempo2 > (tiempo1 + 1000)) { //hay que subirlo a 10000
    char code = keypad.getKey();

    //conection es una bandera que supone que se inicio sesion de manera correcta
    if (conection) { //Si se inicia sesion correctamente entra a las validaciones
      String userEntrada = "*USUARIO*";
      lcd.setCursor(0, 0);
      lcd.print(" "+userEntrada+"     ");

      lcd.setCursor(0, 1);
      lcd.print("   Bienvenido     ");
      delay(5000); //tiempo 5 segundos de espera cuando se conecta


      genToken();
      String validacion = validarToken();
      if (validacion == "cancel") {
        //solicitar si desea generar token otra vez
        bool solicitud = true;
        if (solicitud) {
          genToken();
        }

      } else if (validacion == "fallido") {
        //aca hay que mandarle a la aplicacion que ejecute y que pida si el quiere pedir otro token o salirse
        bool solicitud = true;
        if (solicitud) {
          genToken();
        }

      } else if (validacion == "correcto") {
        seleccionParqueo();
        
      }

    } else { //si no a iniciado sesion que espere
      //timer1.update();
      //timer2.update();
      lcd.setCursor(0, 0);
      lcd.print("  Esperando...   ");

      lcd.setCursor(0, 1);
      lcd.print("                   ");
    }
  }

  /*
  llenarMatriz(m_Ocupados);
  convertir4_8(m_Ocupados);
  vizualizarLed();
  */
}

void pinesMatriz() {
  for (int i = 0 ; i < 16; i++) {
    pinMode(pinMatriz[i], INPUT);
  }
}

void llenarMatriz(int matriz[4][4]) {
  int index = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matriz[i][j] = digitalRead(pinMatriz[index]);
      index ++;
    }
  }
}

void initMatriz(int matriz[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      matriz[i][j] = 0;
    }
  }
}

void convertir4_8(int matriz[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      int f = i * 2;
      int c = j * 2;
      parqueoLed[f][c] = matriz[i][j];
      parqueoLed[f + 1][c] = matriz[i][j];
      parqueoLed[f][c + 1] = matriz[i][j];
      parqueoLed[f + 1][c + 1] = matriz[i][j];
    }
  }
}

void vizualizarLed() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (parqueoLed[i][j] == 1) {
        lc.setLed(0, i, j, true);
      } else {
        lc.setLed(0, i, j, false);
      }
    }
  }
}

// Estos se pueden quitar
void mostrarLed() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      Serial.print(parqueoLed[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}

void imprimir4x4(int matriz[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      Serial.print(matriz[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}

//METODOS EDUARDO
void BtnRegistrar(String entrada) {
  bool bandera = false;
  for (int i = 0; i < 2; i++) {
    if (Usuarios[i] == entrada) {
      bandera = true;
    }
  }
  if (bandera) {
    Serial.print(true);
    bandera = false;
  } else if (!bandera) {
    Serial.print(false);
    //Aqui ingresar usuario a la EPROM
  }
}
void BtnLogin(String entrada) {
  bool bandera = false;

  for (int i = 0; i < 2; i++) {
    if (Usuarios[i] == entrada) {
      bandera = true;//Hace match el usuario que quiere entrar con el almacenado.
    }
  }
  if (bandera) {
    Serial.print(true);//Aqui retornamos que ya esta registrado el usuario
    bandera = false;
  } else if (!bandera) {
    Serial.print(false);
  }
}

//FIN METODOS EDUARDO


void showScreen() {
  lcd.setCursor(3, 0);
  lcd.print("Bienvenido");

  //lock
  lcd.setCursor(1, 0); //columna y fila
  lcd.write(byte(0));

  //lock
  lcd.setCursor(14, 0); //columna y fila
  lcd.write(byte(0));

  lcd.setCursor(0, 1);
  lcd.print("Grupo3-Seccion A");
}

void genToken() {
  tokenM = "";
  long randomNumber = random(10);  // Generate a random number between 0 and 10
  toks[0] = String(randomNumber);

  randomNumber = random(0, 10); // Generate a random number between 0 and 10
  toks[1] = String(randomNumber);

  for (int i = 0; i < 2; i++) {
    randomNumber = random(0, 4);
    toks[i + 2] = String(letrasToks[randomNumber]);
  }

  // Impresion TOKEN en LCD
  lcd.setCursor(5, 1);
  lcd.print("TOKEN: "); //esto borrar cuando se junte el codigo

  //aca mando token a aplicacion y muestra pantalla
  tokenM = toks[0] + toks[1] + toks[2] + toks[3]; // tokenM es el que se manda para aplication
  lcd.setCursor(12, 1);
  lcd.print(tokenM);
  delay(1000);
}

String validarToken() {
  int intentos = 0;
  String idToken = "";
  char key;
  bool correct;

  lcd.clear();
  do {

    if (intentos < 3) {
      correct = false;
      lcd.setCursor(0, 0);
      lcd.print("DIGITE SU TOKEN:");
      lcd.setCursor(0, 1);
      lcd.print(idToken);


      while (true) {
        key = keypad.getKey();
        if (key != NO_KEY) {
          //Serial.println("KeyPad: " +String(code));
          if (key == '*') {
            if (comprobarToken(idToken)) {
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(" TOKEN CORRECTO ");
              lcd.setCursor(0, 1);
              lcd.print("   " + idToken);
              delay(2000);

              lcd.clear();
              correct = true;
              entradaParqueo();
              break;
            } else {
              tone(pinBuzzer, 500); // digitalWrite(buzzerP, HIGH);

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("TOKEN INCORRECTO ");
              lcd.setCursor(0, 1);
              lcd.print("   " + idToken);
              intentos++;
              idToken = "";
              delay(2000);
              lcd.clear();
              noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
              break;
            }
          } else {
            idToken += key;
            break;
          }
        }
      }

      if (idToken.length() > 4) {
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
      if (correct) {
        break;
      }
    } else {
      break;
    }

  } while (key != '#');

  if (intentos == 3) {
    //suena la alrma de exceder 3 intentos
    tone(pinBuzzer, 500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TOKEN INVALIDO");
    lcd.setCursor(0, 1);
    lcd.print("Excedio 3 intents");
    delay(2000);
    noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
    return "fallido";
  }
  if (key == '#') {
    //suena la alarma por presionar cancel
    tone(pinBuzzer, 500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BOTON CANCEL");
    lcd.setCursor(0, 1);
    lcd.print("SE ACTIVO");
    delay(2000);
    noTone(pinBuzzer); // digitalWrite(buzzerP, LOW);
    lcd.clear();
    return "cancel";
  }

  return "correcto";
}

bool comprobarToken(String pass) {

  if (pass.length() != 4) {
    return false;
  }

  if (String(pass[0]) != toks[0]) {
    return false;
  } else if (String(pass[1]) != toks[1]) {
    return false;
  } else if (String(pass[2]) != toks[2]) {
    return false;
  } else if (String(pass[3]) != toks[3]) {
    return false;
  }

  return true;
}

void entradaParqueo() {
  lcd.setCursor(0, 0);
  lcd.print("ENTRANDO PARQUEO");
  delay(1000);
  controlPuertas();
  lcd.setCursor(0, 0);
  lcd.print("Listo              ");
  lcd.clear();
}

void controlPuertas() {
  if (contador < 1) {
    for (int i = 0; i < 1; i++) {
      TiempoAhora = millis();
      abrirPorton();
      digitalWrite(portonAbierto, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Ingreso vehiculo      ");
      lcd.setCursor(0, 1);
      while (millis() < TiempoAhora + periodo) {
        // espere [periodo] milisegundos
      }
      delay(5000);
      cerrarPorton();
      digitalWrite(portonCerrado, LOW);
      contador = contador + 1;
    }
  }
}

void salidaPuertas() {
  if (contador < 1) {
    for (int i = 0; i < 1; i++) {
      TiempoAhora = millis();
      abrirPorton();
      digitalWrite(portonAbierto, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Salida vehiculo      ");
      lcd.setCursor(0, 1);
      while (millis() < TiempoAhora + periodo) {
        // espere [periodo] milisegundos
      }
      delay(5000);
      cerrarPorton();
      digitalWrite(portonCerrado, LOW);
      contador = contador + 1;
    }
  }
}



void abrirPorton() {            // 2 vueltas derecha
  lcd.setCursor(0, 0);
  lcd.print("- abriendo -    ");
  lcd.setCursor(0, 1);
  digitalWrite(portonAbierto, HIGH);
  motor1.setSpeed(10);
  motor1.step(stepsPerRevolution);
  delay(1000);
}

void cerrarPorton() {            // 2 vueltas derecha
  lcd.setCursor(0, 0);
  lcd.print("- cerrando -     ");
  lcd.setCursor(0, 1);
  digitalWrite(portonCerrado, HIGH);
  motor2.setSpeed(10);
  motor2.step(-stepsPerRevolution);
  delay(1000);
}

void seleccionParqueo() {
 //int n =digitalRead(11) ;
  Serial3.println(digitalRead(A7));
  if(digitalRead(A7) == 1){
    salidaPuertas();
  }else{
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("selecciona parqueo");
  delay(2000);
  lcd.clear();
  }
   
 
}
