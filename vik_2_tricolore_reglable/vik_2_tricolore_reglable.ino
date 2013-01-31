
int nxt = 0; // 3 boutons de controle
int sub = 8;
int add = 12;
int ctrl[3] = {1,2,4}; // leds vertes de controle
int mix[3] = {3,5,6}; // composantes led blanche
int sing[3] = {9,10,11}; // leds rouges d'intensité

int cur = 0, i; // composante courante
int mem[3]; // valeurs des 3 composantes

void setup() {
  // boutons en input, tout le reste en output
  pinMode(nxt, INPUT);
  pinMode(add, INPUT);
  pinMode(sub, INPUT);
  for (i=0; i<3; i++) {
    pinMode(ctrl[i], OUTPUT);
    pinMode(mix[i], OUTPUT);
    pinMode(sing[i], OUTPUT);
    mem[i] = 50; // valeurs par défaut des composantes à 50
    light(i); // qu'on affiche
  }
  digitalWrite(ctrl[cur], HIGH); // et la led de controle courant
}

// gestion de l'affichage d'une composante donnée
void light(int cur) {
  analogWrite(sing[cur], mem[cur]); // la valeur stoquée va à la led rouge
  analogWrite(mix[cur], 255-mem[cur]); // et son inverse à la blanche (montage sur le +)
}

void loop() {
  // si changement de composante on augmente cur
  if (digitalRead(nxt)) {
    digitalWrite(ctrl[cur], LOW); // sans oublier de mettre à jour les controles
    cur++;
    if (cur>2)
      cur=0;
    digitalWrite(ctrl[cur], HIGH);
    delay(100); // anti rebond
  }
  // si demandé on augmente et met a jour la valeurs courante
  if (digitalRead(add)) {
    if (mem[cur]<255) {
      mem[cur]++; // en mémoire
      light(cur); // et à l'affichage
    }
  }
  // et pareil dans l'autre sens
  if (digitalRead(sub)) {
    if (mem[cur]>0) {
      mem[cur]--;
      light(cur);
    }
  }
  delay(20);
}
