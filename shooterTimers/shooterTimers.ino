
#include <LedControl.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include  <LiquidCrystal_I2C.h>

#include <SeqTimer.h>
SeqTimer timer1,timer2,timer3,timer4,timerTiradas;
SeqTimer timerTirada;
int posicionTirada=-1;

Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver(0x40);
LiquidCrystal_I2C lcd(0x3F,16,2);  

unsigned int pos0=172; // ancho de pulso en cuentas para pocicion 0°
unsigned int pos180=565; // ancho de pulso en cuentas para la pocicion 180°



const uint64_t IMAGES[] = {
  0x3c66666e76663c00,
  0x7e1818181c181800,
  0x7e060c3060663c00,
  0x3c66603860663c00,
  0x30307e3234383000,
  0x3c6660603e067e00,
  0x3c66663e06663c00,
  0x1818183030667e00,
  0x3c66663c66663c00,
  0x3c66607c66663c00,
  0xff44444455664400,
  0xff14146485867400,
  0xffc4c474c5c67400,
  0xff8484f495969400,
  0xff8484f41516f400,
  0xff9494f41516f400,
  0xff1414244586f400,
  0xff9494f49596f400,
  0xff8484f49596f400,
  0xff9191979494f700,
  0xff81818794a4c700,
  0xff1111f78484f700,
  0xff8181f78484f700,
  0xff8181f794949700,
  0xff8181f71414f700,
  0xff9191f71414f700,
  0xff1111274484f700,
  0xff9191f79494f700,
  0xff8181f79494f700
};
const int IMAGES_LEN = sizeof(IMAGES)/8;
LedControl display= LedControl(11,12,13);

// the timer object


int contador = 0;
int puntuacion=0;
int jugando=0;
unsigned long tiempo0 = 0;
unsigned long tiempo1 = 0;
bool primeraVez = false;

int NumberArray[4]={1,2,3,4};
int enviadoPuntuacion[4]={0,0,0,0};
int target1,target2,target3,target4;
int luzservo1=3;
int luzservo2=4;
int luzservo3=5;
int luzservo4=6;
int timerReset=2000;

int haytirada=0;

int inicio=0;
int estadoAC=0;
int estadoAN=0;
int sluz1=A5;
int pinSonido=10;
int value=1;
int value1=1;
int value2=1;
int value3=1;
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

void displayImage(uint64_t image){
  for(int i=0;i<8;i++){
    byte row=(image >> i*8) & 0xff;
    for(int j=0;j<8;j++){
      display.setLed(0,i,j,bitRead(row,j));
    }
  }
}
void setup() { 
  
  Serial.begin(9600);

  servos.begin();  
  servos.setPWMFreq(60); //Frecuecia PWM de 60Hz o T=16,66ms
  setServo(0,60); 
  setServo(1,60); 
  setServo(2,60); 
  setServo(3,60); 

       
  pinMode(luzservo1,INPUT);
  pinMode(luzservo2,INPUT);
  pinMode(luzservo3,INPUT);
  pinMode(luzservo4,INPUT);
  pinMode(9,OUTPUT);
   digitalWrite(9, LOW); // Turn the LED
  lcd.init();
  //Encender la luz de fondo.
  lcd.backlight();
  // Escribimos el Mensaje en el LCD.bn
  lcd.print("!! Ready !!");
 display.clearDisplay(0);
  display.shutdown(0,false);
  display.setIntensity(0,10); 
  displayImage(IMAGES[0]);
}
void setServo(uint8_t n_servo, int angulo) {
  int duty;
  duty=map(angulo,0,180,pos0, pos180);
  servos.setPWM(n_servo, 0, duty);  
}
void tirar(){
  Serial.println("-------------------------------");
 Serial.println("           TIRADA            ");
  shuffleArray(NumberArray,3);
  animaServos();
 sonidoAcierto();

}
void animaServos(){
  
  posicionTirada=0;
  timerTirada.init(1200);   
}
void sonidoAcierto(){
  digitalWrite(pinSonido, HIGH);
  tone(pinSonido, 988, 250);
  delay(100);
  digitalWrite(pinSonido, LOW);
   digitalWrite(9, LOW);
}
void moverServo(int nservo,int grados){
Serial.println("--------------------");
Serial.println("Mover servo");
Serial.println(nservo);
 digitalWrite(9, LOW); // Turn the LED
  switch(nservo){
    case 1:
     
      target1=1;
      
      setServo(0,grados);
      timer1.init(timerReset);
     break;
    case 2:     
      target2=1;
      setServo(1,grados);
      timer2.init(timerReset);
      break;
    case 3: 
      target3=1;
      setServo(2,grados);
      timer3.init(timerReset);
      break;
    case 4: 
      target4=1;
      setServo(3,grados);
      timer4.init(timerReset);
      break;
  }
  
  delay(200);
}
void resetTirada(){

  if(timerTirada.getTimer()==true && posicionTirada!=-1){
     
       if(posicionTirada==4){   
        posicionTirada=-1;  
       }
       else{
          moverServo(NumberArray[posicionTirada],180);
          posicionTirada++;
       }
  }
  
  if(timerTiradas.getTimer()==true){
    jugando=0;
    lcd.clear();
    lcd.print("Nueva Tirada");
    tirar();
    
  }
  
  if (timer1.getTimer() == true)
  {
      enviadoPuntuacion[0]==0;
      setServo(0,80);
      value=1;
      target1=0;
  }else{
    if(target1==1){
       value = digitalRead(luzservo1);  
      if(value==0 && enviadoPuntuacion[0]==0){
        puntuacion++;
        setServo(0,80);
        writePuntuacion();
        target1=0;
        enviadoPuntuacion[0]=1;
      }
      
    }
   
  }
 
  if (timer2.getTimer() == true)
  {   
     enviadoPuntuacion[1]=0;
      setServo(1,80);
      target2=0;
  }else{
     if(target2==1){
        int value2 = digitalRead(luzservo2);  
        if(value2==0  && enviadoPuntuacion[1]==0){
          puntuacion++;
          setServo(1,80);
          writePuntuacion();
          target2=0;
           enviadoPuntuacion[1]=1;
        }
     }
  }
  
  if (timer3.getTimer() == true)
  {
      setServo(2,80);
      target3=0;
       enviadoPuntuacion[2]=0;
  }else{
     if(target3==1){
        int value3 = digitalRead(luzservo3);  
        if(value3==0  && enviadoPuntuacion[2]==0){
          puntuacion++;
          setServo(2,80);
          writePuntuacion();
          target3==0;
           enviadoPuntuacion[2]=1;
        }
     }    
   
    
  }
  if (timer4.getTimer() == true)
  {
      setServo(3,80);
      target4=0;
      enviadoPuntuacion[3]=0;
  }else{
    if(target4==1){
      int value4 = digitalRead(luzservo4);  
      if(value4==0 && enviadoPuntuacion[3]==0){
        puntuacion++;
        setServo(3,80);
        writePuntuacion();
        target4=0;
        enviadoPuntuacion[3]=1;
      }
      
    }
   
  }
   
}
void writePuntuacion(){
  
  Serial.println("Puntuacion");
  Serial.println(puntuacion);
 displayImage(IMAGES[puntuacion]);
  digitalWrite(9, HIGH); // Turn the LED
}
void loop() {

  //Leer el estado de la entrada digital 7
  if(jugando==0)
  {
    writePuntuacion();
    setServo(15,45);
    timerTiradas.init(12000);
    jugando=1;
  }
  int estadoBoton = digitalRead(12); 
  //Serial.println(estadoBoton);
  resetTirada();

  
  
  
} 
