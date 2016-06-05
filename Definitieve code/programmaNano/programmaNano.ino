#include <Servo.h>
int reader;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;              
bool stippenlijn = false;
bool boven= false;
bool beneden = true;
const long interval = 15;           

#define echoPin 5          // Echo Pin
#define trigPin 4          // Trigger Pin
#define timeOutPin A5       //De pin om de sensor aan en uit te doen

int coila1 = 6;
int coila2 = 7;
int coilb1 = 8;
int coilb2 = 9;
int enable = 10;

int draainaarbeneden = 0;
int middenpuntpositie = 0;
int draainaarboven =0;
int positie = 0;

int maximumRange = 60;                   // Maximum range needed
int minimumRange = 0;                     // Minimum range needed

int rechtseAfstand  = -1;
int linkseAfstand = -1;
bool schuinKijken = false;

bool rechtsDraaien = false;


Servo myservo;

void setup(){
  myservo.attach(3);  // attaches the servo on pin 11 to the servo object
  Serial.begin(9600);
  reader = 0;

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(timeOutPin, OUTPUT);
  pinMode(coila1, OUTPUT);
  pinMode(coila2, OUTPUT);
  pinMode(coilb1, OUTPUT);
  pinMode(coilb2, OUTPUT);
  pinMode(enable, OUTPUT);
  myservo.write(80);
}

void loop(){

  unsigned long currentMillis = millis();
  if (Serial.available() > 1) {
      reader = Serial.parseInt();
      delay(2);
  }
  
  if (reader == 0) { myservo.write(80);  delay(500); reader = -1; }
  else if (reader == 1) { myservo.write(60); delay(500); reader = -1;}
  else if (reader == 2) { myservo.write(100); delay(500); reader = -1; }
  else if (reader == 3) { myservo.write(0); delay(1000); reader = -1; }
  else if (reader == 4) { myservo.write(180); delay(1000); reader = -1; }
  else if (reader == 6) {  beneden = true; stippenlijn = false; reader = -1; }
  else if (reader == 7) {stippenlijn = true ; beneden = false; reader = -1; }
  else{

  if(beneden){

    draainaarbeneden = 10 - middenpuntpositie;
    beneden=false;

 }

  if(stippenlijn && !beneden){
    if(middenpuntpositie<=0){
       draainaarbeneden = 10 - middenpuntpositie;
    }
    if(middenpuntpositie>=10){
      draainaarboven = middenpuntpositie;
    }
  }

  
    
     if (currentMillis - previousMillis >= interval) {

        Distance();

       
            if(draainaarboven>0){
              digitalWrite(enable, HIGH);
        
              switch(positie){
                case 0:
                  a1Enb2();
                  positie++;
                  break;
                case 1:
                  b2();
                  positie++;
                  break;
                case 2:
                  b2Ena2();
                  positie++;
                  break;
                case 3:
                  a2();
                  positie++;
                  break;
                case 4:
                  a2Enb1();
                  positie++;
                  break;
                case 5:
                  b1();
                  positie++;
                  break;
                case 6:
                  b1Ena1();
                  positie++;
                  break;
                case 7:
                  a1();
                  positie = 0;
                  draainaarboven--;
                  middenpuntpositie--;
                  break;
              }
              
            }else if(draainaarbeneden>0){
              digitalWrite(enable, HIGH);
        
              switch(positie){
                case 0:
                  a1();
                  positie ++;
                  break;
                case 1:
                  b1Ena1();
                  positie++;
                  break;
                case 2:
                  b1();
                  positie++;
                  break;
                case 3:
                  a2Enb1();
                  positie++;
                  break;
                case 4:
                  a2();
                  positie++;
                  break;
                case 5:
                  b2Ena2();
                  positie++;
                  break;
                case 6:
                  b2();
                  positie++;
                  break;
                case 7:
                  a1Enb2();
                  positie = 0;
                  draainaarbeneden--;
                  middenpuntpositie++;
                  break;
              }
              
            }
            else{
              digitalWrite(enable, LOW);
            }
        previousMillis = currentMillis;
     
    }
  }

    

    
  
   
}


void automatischrijdenZigZag(){
  afstand = filterDistance();
  if (afstand == 1){
    
    Serial.println(11); //Verstuurd stop
     if (rechtsDraaien == true) {
      while(rechtseAfstand <1){
        rechtseAfstand = RechtsKijken90();
      }    
      if(rechtseAfstand >1){
         Serial.println(3); // Verstuurd om rechts te draaien

          schuinKijken = false;
          myservo.write(80);
          afstand=0;
         delay(4000);
         beneden = true; 
         stippenlijn = false;
         rechtsDraaien = false;
        }  
      }
      else if(rechtsDraaien == false){
       while(linkseAfstand <1){
        linkseAfstand = LinksKijken90();
       }
        if(linkseAfstand >1){
          Serial.println(4); // Verstuurd om links te draaien

          schuinKijken = false;
          myservo.write(80);
          afstand=0;
          delay(4000);
          stippenlijn = true ;
          beneden = false;
          rechtsDraaien = true;

        }  
      }
          rechtseAfstand = -1;
          linkseAfstand = -1;

         
    }  
    else if(afstand ==2){
        
        if (schuinKijken == false){ 
          Serial.println(11); //Verstuurd stop
          schuinKijken = true; 
          
          rechtseAfstand = RechtsKijken20();                
          linkseAfstand = LinksKijken20(); 
          
          if( rechtseAfstand <=2 &&  rechtseAfstand > 0|| linkseAfstand <= 2 &&  linkseAfstand > 0){
            if(rechtseAfstand < linkseAfstand){
              myservo.write(60);

            } 
              
          }
          else{
            myservo.write(80);
          }
          rechtseAfstand = -1;
          linkseAfstand = -1;
        }  
      Serial.println(2); //Verstuurd om traag te rijden
    }  
    else if(afstand ==3) {

      Serial.println(5);//Verstuurd om snel te rijden
    }else{
       Serial.println(11); //verstuurd stop
    }
    
}

int RechtsKijken90() {

  myservo.write(0);
  delay(1000);
  return kijken();

}
int LinksKijken90() {
  myservo.write(180);
  delay(1000);
  return kijken();
}
int RechtsKijken20() {
  myservo.write(60);
  delay(500);
  return kijken();
}
int LinksKijken20() {
  myservo.write(100);
  delay(500);
  return kijken();
}
int kijken(){
  int tempafstand = 0;
  while(tempafstand == 0){
    tempafstand = filterDistance();
    delay(10);
  }
  return tempafstand;
}



void Distance(){ // meet de afstand van de sensor
 
  long duration, distance;                  // Duration used to calculate distance
  digitalWrite(timeOutPin, HIGH);
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;
 
  if (distance >= maximumRange || distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON 
    to indicate "out of range" */
    distance = 201;
    digitalWrite(timeOutPin, LOW);
  }

  else { 
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    //Serial.println(distance);
  }
  Serial.println(distance);
}

int filterDistance() {

  afstandwaarde = Distance();
  if(count <= 5 && afstandwaarde == 201 || afstandwaarde == 0) {
    count++;
    afstandwaarde = Distance();
    return 0;
  }else{
     count = 0;
    if(afstandwaarde < 15 && afstandwaarde > 0){
       return 1;
    }
     else if(afstandwaarde < 35 && afstandwaarde > 0){
      return 2;
    }
    else{
      return 3;
    }
  } 
}



void a1Enb2(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}

void b2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}
void b2Ena2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, HIGH);
}
void a2(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
}
void a2Enb1(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, HIGH);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void b1(){
    digitalWrite(coila1, LOW);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void b1Ena1(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, HIGH);
    digitalWrite(coilb2, LOW);
}
void a1(){
    digitalWrite(coila1, HIGH);
    digitalWrite(coila2, LOW);
    digitalWrite(coilb1, LOW);
    digitalWrite(coilb2, LOW);
}



