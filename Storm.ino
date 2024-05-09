// Rules for the Logic of the Device: 
//  1) Click the start button and it will run throughout (needs to be let off before next)
//  2) Estop will reverse back to start conditions
//  3) Door switch will reset everything 
//  4) After start piston will have to compress spring siwtch 
//  5) Lower into place, hit switch
//  6) Move back to top and switch click
//  7) Retract P1 back to start


// Prototype: (8 total) 
// P1 and P2 min and Max (4)
// Door Switch (1)
// Start and Estop (2)
// Slide Closed (1)


// constants won't change. They're used here to set pin numbers:
const int start_button = A0;    
const int estop_button = A1;    
const int piston_1_min = A2;
const int piston_1_max = A3;
const int piston_2_min = A4;
const int piston_2_max = A5;
const int door_switch = A6;
const int holder_slide = A7;
const int forward_running_state_led = LED_D0;  
const int reverse_running_state_led = LED_D1;    
const int estop_state_led = LED_D2;  
const int ready_to_start = LED_D3;  
const int hydraulics_forward = LED_BUILTIN;  
const int hydraulics_reverse = LEDR;  

 
// variables will change:
int start_button_state = 0;
int estop_button_state = 0;
int piston_1_min_state = 0;
int piston_1_max_state = 0;
int piston_2_min_state = 0;
int piston_2_max_state = 0;
int door_switch_state = 0;
int holder_slide_state = 0;
      
int ready_to_start_bool = 0;
int estop_bool = 0;
int ledHigh = 0;
int operation_index = 0;
int forward_or_reverse = 0;

void reading_all_sensors() {
  start_button_state = analogRead(start_button);
  estop_button_state = analogRead(estop_button);
  piston_1_min_state = analogRead(piston_1_min);
  piston_1_max_state = analogRead(piston_1_max);
  piston_2_min_state = analogRead(piston_2_min);
  piston_2_max_state = analogRead(piston_2_max);
  door_switch_state = analogRead(door_switch);
  holder_slide_state = analogRead(holder_slide);

  if (start_button_state > 100) {
    start_button_state = HIGH;
  }
  else {
    start_button_state = LOW;
  }

  if (estop_button_state > 100) {
    estop_button_state = HIGH;
  }
  else {
    estop_button_state = LOW;
  }

  if (piston_1_min_state > 100) {
    piston_1_min_state = HIGH;
  }
  else {
    piston_1_min_state = LOW;
  }

  if (piston_1_max_state > 100) {
    piston_1_max_state = HIGH;
  }
  else {
    piston_1_max_state = LOW;
  }

  if (piston_2_min_state > 100) {
    piston_2_min_state = HIGH;
  }
  else {
    piston_2_min_state = LOW;
  }

  if (piston_2_max_state > 100) {
    piston_2_max_state = HIGH;
  }
  else {
    piston_2_max_state = LOW;
  }

  if (door_switch_state > 100) {
    door_switch_state = HIGH;
  }
  else {
    door_switch_state = LOW;
  }

  if (holder_slide_state > 100) {
    holder_slide_state = HIGH;
  }
  else {
    holder_slide_state = LOW;
  }
}
 
void setup() {
  Serial.begin(9600);
 
  pinMode(start_button, INPUT); // A0
  pinMode(estop_button, INPUT); // A1
  pinMode(piston_1_min, INPUT); // A2
  pinMode(piston_1_max, INPUT); // A3
  pinMode(piston_2_min, INPUT); // A4
  pinMode(piston_2_max, INPUT); // A5
  pinMode(door_switch, INPUT);  // A6
  pinMode(holder_slide, INPUT); // A7

  pinMode(forward_running_state_led, OUTPUT);
  pinMode(reverse_running_state_led, OUTPUT);
  pinMode(estop_state_led, OUTPUT);
  pinMode(ready_to_start, OUTPUT);
  pinMode(hydraulics_forward, OUTPUT);
  pinMode(hydraulics_reverse, OUTPUT);

  digitalWrite(LED_USER, HIGH);
}
 
void loop() {
  reading_all_sensors();
  // Serial.println(operation_index);

  // resetting the estop
  if (estop_button_state == HIGH) {
    digitalWrite(forward_running_state_led, LOW);
    digitalWrite(reverse_running_state_led, LOW);
    digitalWrite(estop_state_led, LOW);
    digitalWrite(ready_to_start, LOW);
    ready_to_start_bool = 0;

    while (estop_button_state == HIGH) {
      digitalWrite(forward_running_state_led, HIGH);
      delay(1000);
      reading_all_sensors();
      if (estop_button_state == LOW) { break; }
      digitalWrite(reverse_running_state_led, HIGH);
      delay(1000);
      reading_all_sensors();
      if (estop_button_state == LOW) { break; }
      digitalWrite(estop_state_led, HIGH);
      delay(1000);
      digitalWrite(ready_to_start, HIGH);
      break;
    }


    reading_all_sensors();
    if (estop_button_state == HIGH) {
      while (estop_button_state == HIGH) {
        reading_all_sensors();
        delay(10);
      }
      estop_bool = 0;
    }
    digitalWrite(forward_running_state_led, LOW);
    digitalWrite(reverse_running_state_led, LOW);
    digitalWrite(ready_to_start, LOW);
    if (estop_bool == 1) {
      digitalWrite(estop_state_led, HIGH);
    }
    else {
      digitalWrite(estop_state_led, LOW);
    }
  }

  // Seeing if the system should be ready to start A2 A4 A6 A7
  if (operation_index == 0 && piston_1_min_state == HIGH && piston_1_max_state == LOW && piston_2_min_state == HIGH && piston_2_max_state == LOW && holder_slide_state == HIGH && door_switch_state == HIGH && estop_bool == 0) {
    digitalWrite(ready_to_start, HIGH);
    ready_to_start_bool = 1;
    forward_or_reverse = 0;
  }
  else {
    digitalWrite(ready_to_start, LOW);
    ready_to_start_bool = 0;
  }

  // running through the procedure 
  if (ready_to_start_bool == 1 && start_button_state == HIGH) {
    digitalWrite(ready_to_start, LOW);

    while (1) {
      while (estop_button_state == LOW && piston_1_min_state == HIGH && piston_1_max_state == LOW && piston_2_min_state == HIGH && piston_2_max_state == LOW && (operation_index == 0 || operation_index == 1)) {
        reading_all_sensors();
        if (operation_index == 0 && forward_or_reverse == 0) { 
          operation_index++; 
          Serial.println("Powering Piston 1");
          digitalWrite(forward_running_state_led, HIGH);
        }
        if (operation_index == 1 && forward_or_reverse == 1) { operation_index--; break; }
      }
      delay(50);

      while (estop_button_state == LOW && piston_1_min_state == LOW && piston_1_max_state == LOW && piston_2_min_state == HIGH && piston_2_max_state == LOW && (operation_index == 1 || operation_index == 2)) {
        reading_all_sensors();
        if (operation_index == 1 && forward_or_reverse == 0) { 
          operation_index++; 
          Serial.println("Piston 1 On Piston 2 Off");
        }
        if (operation_index == 2 && forward_or_reverse == 1) { operation_index--; }
      }
      delay(50);

      while (estop_button_state == LOW && piston_1_min_state == LOW && piston_1_max_state == HIGH && piston_2_min_state == HIGH && piston_2_max_state == LOW && (operation_index == 2 || operation_index == 3)) {
        reading_all_sensors();
        if (operation_index == 2 && forward_or_reverse == 0) { 
          operation_index++; 
          Serial.println("Powering Piston 2");
        }
        if (operation_index == 3 && forward_or_reverse == 1) { operation_index--; }
      }
      delay(50);

      while (estop_button_state == LOW && piston_1_min_state == LOW && piston_1_max_state == HIGH && piston_2_min_state == LOW && piston_2_max_state == LOW && (operation_index == 3 || operation_index == 4)) {
        reading_all_sensors();
        if (operation_index == 3 && forward_or_reverse == 0) { 
          operation_index++; 
          Serial.println("Piston 1 On Piston 2 On");
        }
        if (operation_index == 4 && forward_or_reverse == 1) { operation_index--; }
      }
      delay(50);

      while (estop_button_state == LOW && piston_1_min_state == LOW && piston_1_max_state == HIGH && piston_2_min_state == LOW && piston_2_max_state == HIGH && (operation_index == 4 || operation_index == 5)) {
        reading_all_sensors();
        if (operation_index == 4 && forward_or_reverse == 0) { 
          operation_index++; 
          Serial.println("Press Complete! Reversing!");
        }
        if (operation_index == 5 && forward_or_reverse == 1) { operation_index--; }
      }
      delay(50);

      while (estop_button_state == LOW && piston_1_min_state == LOW && piston_1_max_state == HIGH && piston_2_min_state == LOW && piston_2_max_state == HIGH && (operation_index == 5 || operation_index == 6)) {
        reading_all_sensors();
        if (operation_index == 5 && forward_or_reverse == 0) { 
          operation_index++; 
          forward_or_reverse = 1; 
          Serial.println("Piston 1 Off Piston 2 On Reverse");
          digitalWrite(forward_running_state_led, LOW);
          digitalWrite(reverse_running_state_led, HIGH);
        }
        if (operation_index == 6 && forward_or_reverse == 1) { operation_index--; }
      }
      delay(50);

      if (estop_button_state == HIGH) {
        Serial.println("EStop!");
        estop_bool = 1;
        digitalWrite(forward_running_state_led, LOW);
        digitalWrite(reverse_running_state_led, LOW);
        digitalWrite(estop_state_led, HIGH);
        delay(5000);
      }
    }
    

  }

}



