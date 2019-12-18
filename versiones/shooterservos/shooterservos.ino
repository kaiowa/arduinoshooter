#include <Servo.h> 
#include <PagonGameDev_GameLoop.h>


int GameFPS = 25;
void GameLoop();
GameInterval FPSInterval(1000 / GameFPS, GameLoop);
Servo myservo1,myservo2;
#define UP_STATE 90
#define DOWN_STATE 0
int randomNumber;
long haytirada;
int haysetup;
int inicioPartida;


void setup() {
  
  Serial.println("setup");
  Serial.begin(9600);
  haysetup=1;
  myservo1.attach(2);
  myservo2.attach(4);
  resetservos();
  
  haytirada=0;
}
void resetservos(){
  Serial.println("resetservos");
  myservo1.write(45);   
  myservo2.write(90);
  delay(2000);

}

void mostrardiana(int position){

     Serial.println("mostrar dianaa");
}


void loop() {
  FPSInterval.update();
}

void GameLoop()
{
  if(haytirada!=1)
  {
     Serial.println(haytirada);
    if(haysetup!=1)
    {
      setup();
      randomNumber = random(2000,3000);
      delay(4000); // delay para el inicio de partida
      haytirada=1;
       
    }
  //Esperamos 1 segundo para repetir
      
  }else{
     Serial.println(haytirada);
    haytirada=0;
    mostrardiana(1);
    delay(2000);
  }
  
delay(1000);
}
