void setup() {
 for(int i=8; i<=10; i++)
 pinMode(i, OUTPUT);
}


void loop() {
  digitalWrite(8, millis()/2000 & 1);
   digitalWrite(9, millis()/3000 & 1); 
    digitalWrite(10, millis()/4000 & 1);                   
}
