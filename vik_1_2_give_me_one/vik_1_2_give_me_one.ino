#include <Servo.h> // la biblio de gestion du servo

Servo ser;
int light = A5; // le capteur de lumière
int but = 0; // le bouton pour commander un réglage

int val;
int lim = 800; // seuil de déclenchement initial (inspirée du Serial)

void setup() {
  Serial.begin(9600); // initialise la comm avec le moniteur
  while (!Serial) {;} // leonardo doit attendre la connection
  pinMode(light, INPUT);
  pinMode(but, INPUT);
  ser.attach(9); // connection du servo
  ser.write(0); // servo sur l'angle 0
}

void loop() {
  val = analogRead(light); // on lit la lumistance
  
  // on teste le bouton
  if (digitalRead(but)) {
    Serial.print("lim=");
    lim=val;
  }
  Serial.println(val); // et on affiche sur le moniteur
  
  // puis on teste la lumistance
  if (val<lim) {
    ser.write(90);// aller
    delay(200); 
    ser.write(0);// puis retour
    delay(1000); // mais pas trop vite entre 2 passages
  }
  delay(100);
}
