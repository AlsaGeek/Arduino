int red = 13; // les 2 leds, puis le potentiomètre
int green = 12;
int pot = A5;

int val; // une variable temporaire

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(pot, INPUT);
}

void loop() {
  val = 2 * analogRead(pot); // on lit le temps désiré

  // et on alterne les leds sur ce rythme
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  delay(val);

  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  delay(val);
}
