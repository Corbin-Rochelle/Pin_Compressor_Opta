// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 2;     
const int buttonPin2 = 8;     
const int ledPin =  7;      

// variables will change:
int buttonState1 = 0;         
int buttonState2 = 0;         
int completeCheck = 1;
int ledHigh = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}

void loop() {
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  if (buttonState1 == HIGH && buttonState2 == HIGH && completeCheck == 1) {
    Serial.println("ON");
    digitalWrite(ledPin, HIGH);
    ledHigh = 1;
  }
  else if ((buttonState1 == LOW ^ buttonState2 == LOW) && ledHigh == 1) {
     completeCheck = 0;
     Serial.println("Only One");
     digitalWrite(ledPin, LOW);
     ledHigh = 0;
  }
  if (buttonState1 == LOW && buttonState2 == LOW) {
      completeCheck = 1;
      Serial.println("Reset");
      ledHigh = 0;
   }
}
