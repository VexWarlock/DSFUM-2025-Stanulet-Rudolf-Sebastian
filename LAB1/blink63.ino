void setup() {
 for(int i=8; i<=13; i++)
 pinMode(i, OUTPUT);
}

void afiseaza(int k)
{
  for(int i=0; i<=5; i++)
    digitalWrite(i+8, k & (1<<i));
}

void loop() {
         afiseaza(millis()/50);       
}
