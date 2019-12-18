

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include  <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <SeqTimer.h>
SeqTimer timer1,timer2,timer3,timer4;
const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);
LiquidCrystal_I2C lcd(0x3F,16,2);  

unsigned int pos0=172; // ancho de pulso en cuentas para pocicion 0°
unsigned int pos180=565; // ancho de pulso en cuentas para la pocicion 180°

// the timer object

int Pulsador=12;
int contador = 0;
unsigned long tiempo0 = 0;
unsigned long tiempo1 = 0;
bool primeraVez = false;

int NumberArray[4]={1,2,3,4};

int target1,targe2,target3,target4;
int luzservo1=6;
int luzservo2=5;
int luzservo3=4;
int luzservo4=3;


int haytirada=0;


int inicio=0;
int estadoAC=0;
int estadoAN=0;
int sluz1=A5;

unsigned long interval=1000; // the time we need to wait
unsigned long previousMillis=0; // millis() returns an unsigned long.
unsigned long currentMillis=0;
 
void shuffleArray(int * array, int size)
{
  int last = 0;
  int temp = array[last];
  for (int i=0; i<size; i++)
  {
    int index = random(size);
    array[last] = array[index];
    last = index;
  }
  array[last] = temp;
}

void setup() { 
  
  Serial.begin(9600);
  pinMode(Pulsador,INPUT);
  servos.begin();  
  servos.setPWMFreq(60); //Frecuecia PWM de 60Hz o T=16,66ms
  setServo(0,60); 
  setServo(1,60); 
  setServo(2,60); 
  setServo(3,60);          
 // pinMode(sluz1,INPUT);
  lcd.init();

  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("Hola Mundo");
 irrecv.enableIRIn();
  irrecv.blink13(true);
  
}
void setServo(uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,pos0, pos180);
  servos.setPWM(n_servo, 0, duty);  
}
void tirar(){
   setServo(0,80); 
  setServo(1,80); 
  setServo(2,80); 
  setServo(3,80); 
  delay(1000);
  shuffleArray(NumberArray,3);
  for (int i=0; i<=4; i++)
  {
    if(NumberArray[i]==1 ||NumberArray[i]==2 ||NumberArray[i]==3 ||NumberArray[i]==4){
      
    //Serial.println(NumberArray[i]);
 
      moverServo(NumberArray[i],180);
     //delay(600);
      
     
    }
    
  }

}
void resetServo1(){
   setServo(0,80);
}
void resetServo2(){
  setServo(1,80);
}
void resetServo3(){
  setServo(2,80);
}
void resetServo4(){
  setServo(3,80);
}
void moverServo(int nservo,int grados){
Serial.println("--------------------");
Serial.println("Mover servo");
Serial.println(nservo);
Serial.println("--------------------");

  switch(nservo){
    case 1:
      lcd.clear();
      lcd.print("servo 1");
        setServo(0,grados);
      timer1.init(1000);
     break;
    case 2:
      lcd.clear();
     lcd.print("servo 2");
     setServo(1,grados);
timer2.init(1000);
      break;

    case 3:
      lcd.clear();
      lcd.print("servo 3");
       setServo(2,grados);
 timer3.init(1000);
      break;

    case 4:
      lcd.clear();
      lcd.print("servo 4");
      setServo(3,grados);
      timer4.init(1000);
      break;
  }
  
  delay(100);
}
void resetTirada(){
if (timer1.getTimer() == true)
  {
      setServo(0,80);
  }
  if (timer2.getTimer() == true)
  {
      setServo(1,80);
  }
  if (timer3.getTimer() == true)
  {
      setServo(2,80);
  }
  if (timer4.getTimer() == true)
  {
      setServo(3,80);
  }
  
}
void loop() {
  /*myservo1.write(180,127,false);        // move the servo to 180, fast speed, run background
                                        // write(degrees 0-180, speed 1-255, wait to complete true-false)
  myservo2.write(180,127,true);         // move the servo to 180, fast speed, wait until done
  
  myservo1.write(0,30,false);           // move the servo to 180, slow speed, run in background
  myservo2.write(0,30,true);            // move the servo to 180, slow speed, wait until done
*/
  //Leer el estado de la entrada digital 7
  int estadoBoton = digitalRead(12); 
  //Serial.println(estadoBoton);
  resetTirada();
  if(estadoBoton == 0){
    //El boton esta activado
    if(!primeraVez){ primeraVez = true; tiempo0 = millis(); }  //Se detecta la primera activacion y se guarda ese tiempo
    else{
      tiempo1 = millis();     //En los rebotes se verifica el tiempo, aqui se podria llamar a otra funcion para aprovechar el tiempo del procesador
      if((tiempo1-tiempo0)>1000) {  //Si el tiempo supera los 200ms entonces activamos las instrucciones correspondientes
          //Se activo el boton y pasaron 200ms
          primeraVez = false;
          contador++;
          Serial.print("Boton activado: ");
          Serial.println(contador);
      }
    }
    
    
  }
  else{
    tirar();
    contador=0;
    //El boton esta desactivado
   // Serial.println("Boton desactivado");
  }

  if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFFA25D:
          Serial.println("CH-");
          break;
          case 0xFF629D:
          Serial.println("CH");
          break;
          case 0xFFE21D:
          Serial.println("CH+");
          break;
          case 0xFF22DD:
          Serial.println("|<<");
          break;
          case 0xFF02FD:
          Serial.println(">>|");
          break ;  
          case 0xFFC23D:
          Serial.println(">|");
          break ;               
          case 0xFFE01F:
          Serial.println("-");
          break ;  
          case 0xFFA857:
          Serial.println("+");
          break ;  
          case 0xFF906F:
          Serial.println("EQ");
          break ;  
          case 0xFF6897:
          Serial.println("0");
          break ;  
          case 0xFF9867:
          Serial.println("100+");
          break ;
          case 0xFFB04F:
          Serial.println("200+");
          break ;
          case 0xFF30CF:
          Serial.println("1");
          break ;
          case 0xFF18E7:
          Serial.println("2");
          break ;
          case 0xFF7A85:
          Serial.println("3");
          break ;
          case 0xFF10EF:
          Serial.println("4");
          break ;
          case 0xFF38C7:
          Serial.println("5");
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          break ;
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          break ;
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }
  
} 
