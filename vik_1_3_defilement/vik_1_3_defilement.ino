

int nxt = 3; // les 2 boutons
int prv = 4;

int i, cur=0; // leds sur 0, 1 et 2, valeurs prises par cur

void setup() {                
  pinMode(nxt, INPUT);
  pinMode(prv, INPUT);
  for (i=0; i<3; i++) // les 3 leds
    pinMode(i, OUTPUT); 
  digitalWrite(cur,HIGH); // on affiche cur
}

void loop() {
  // bouton suivant
  if (digitalRead(nxt)) {
    digitalWrite(cur, LOW);
    cur++;
    if (cur>2)
      cur=0;
    digitalWrite(cur, HIGH);
  }
  //bouton précédent
  if (digitalRead(prv)) {
    digitalWrite(cur, LOW);
    cur--;
    if (cur<0)
      cur=2;
    digitalWrite(cur, HIGH);
  }
  delay(100); //et un anti rebond  
}
