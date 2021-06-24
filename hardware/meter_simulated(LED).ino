int pin=13;
int rndl;
int rndh;
unsigned long StartTime = millis();


void setup() {
  pinMode(pin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
unsigned long CurrentTime = millis();
unsigned long ElapsedTime = CurrentTime - StartTime;

Serial.println(CurrentTime);
if(ElapsedTime>5000){
    StartTime=CurrentTime;
    Serial.println(StartTime);
}
}