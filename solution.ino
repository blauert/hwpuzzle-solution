// https://www.arduino.cc/reference/
#include <HardwareSerial.h>

#define RESET PIN_PA2
#define ACK PIN_PA3
#define INPUT0 PIN_PA4
#define INPUT1 PIN_PA5
#define INPUT2 PIN_PA6
#define INPUT3 PIN_PA7
#define LED0 PIN_PC0
#define LED1 PIN_PC1
#define LED2 PIN_PC2
#define LED3 PIN_PC3

#define PUSH_TIME 20
#define SLEEP_TIME 20


void setup() {
  // Set pin modes
  // https://docs.arduino.cc/learn/microcontrollers/digital-pins
  // https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
  // https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
  pinMode(RESET, OUTPUT);
  pinMode(ACK, OUTPUT);
  pinMode(INPUT0, OUTPUT);
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(INPUT3, OUTPUT);
  // LEDs: NOT OUTPUT!
  pinMode(LED0, INPUT);
  pinMode(LED1, INPUT);
  pinMode(LED2, INPUT);
  pinMode(LED3, INPUT);

  // Init Serial
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  delay(500);
  Serial.write("Up and running!\n");

  // Reset all buttons to their initial state
  reset();
  ack();
  press(0);
  press(1);
  press(2);
  press(3);
  reset();
}


void loop() {
  //test_game();
  //part1();
  part2();
}


void part1() {
  // Game setup
  int sequence[100];
  size_t seq_len = 0;
  int res = 0;

  while (res == 0) {
    ack();
    // Wining animation will only show after initial ACK
    res = read_result();
    if (res == 1) {
      break;
    }
    
    // Track the sequence
    for (int i = 0; i < seq_len; i++) {
      ack();
    }
    int next_led = read();
    sequence[seq_len] = next_led;
    ack();
  
    // Repeat the sequence
    for (int i = 0; i <= seq_len; i++) {
      Serial.print(sequence[i]);
      press(sequence[i]);
    }
    Serial.print(" -> ");Serial.print(seq_len+1);Serial.print("\n");

    // Detect losing condition, or continue if no LEDs are on
    res = read_result();
    seq_len ++;
  }

  // Print result
  if (res == 1) {
    Serial.print("Length of the sequence: ");Serial.print(seq_len);Serial.print("\n");
    while (true) {
      ack();
      delay(150);
    }
  }
  else if (res == -1) {
    Serial.print("Something went wrong.\n");
    delay(100000);
  }
}


void part2() {
  // Game setup
  int sequence[100];
  size_t seq_len = 0;
  int res = 0;

  // Encoded keyword setup
  int qubits = 0;
  char letter = 0;

  while (res == 0) {
    ack();
    // Wining animation will only show after initial ACK
    res = read_result();
    if (res == 1) {
      break;
    }
    
    // Track the sequence
    for (int i = 0; i < seq_len; i++) {
      ack();
    }
    int next_led = read();
    sequence[seq_len] = next_led;
    ack();

    // Decode the keyword
    letter |= next_led << 2*qubits;
    if (qubits == 3) {
      Serial.print(letter);
      qubits = 0;
      letter = 0;
    }
    else {
      qubits++;
    }
  
    // Repeat the sequence
    for (int i = 0; i <= seq_len; i++) {
      press(sequence[i]);
    }

    // Detect losing condition, or continue if no LEDs are on
    res = read_result();
    seq_len ++;
  }
  
  // Play winning animation
  while (true) {
    ack();
    delay(150);
  }
}


void reset() {
  digitalWrite(RESET, LOW);
  delay(PUSH_TIME);
  digitalWrite(RESET, HIGH);
}


void ack() {
  digitalWrite(ACK, LOW);
  delay(PUSH_TIME);
  digitalWrite(ACK, HIGH);
  delay(SLEEP_TIME);
}


void press(int button) {
  if (button == 0) {
  digitalWrite(INPUT0, LOW);
  delay(PUSH_TIME);
  digitalWrite(INPUT0, HIGH);
  }
  else if (button == 1) {
  digitalWrite(INPUT1, LOW);
  delay(PUSH_TIME);
  digitalWrite(INPUT1, HIGH);
  }
  else if (button == 2) {
  digitalWrite(INPUT2, LOW);
  delay(PUSH_TIME);
  digitalWrite(INPUT2, HIGH);
  }
  else if (button == 3) {
  digitalWrite(INPUT3, LOW);
  delay(PUSH_TIME);
  digitalWrite(INPUT3, HIGH);
  }
  delay(SLEEP_TIME);
}


int read() {
  delay(SLEEP_TIME);
  if (digitalRead(LED0) == LOW) {
    return 0;
  }
  if (digitalRead(LED1) == LOW) {
    return 1;
  }
  if (digitalRead(LED2) == LOW) {
    return 2;
  }
  if (digitalRead(LED3) == LOW) {
    return 3;
  }
  return -1;  // no led is on
}


int read_result() {
  if ((digitalRead(LED0) == LOW && digitalRead(LED2) == LOW && digitalRead(LED1) == HIGH && digitalRead(LED3) == HIGH) || 
      (digitalRead(LED1) == LOW && digitalRead(LED3) == LOW && digitalRead(LED0) == HIGH && digitalRead(LED2) == HIGH)) {
      return 1;  // win
  }
  if (digitalRead(LED0) == LOW && digitalRead(LED1) == LOW && digitalRead(LED2) == LOW && digitalRead(LED3) == LOW) {
      return -1;  // lose
  }
  return 0;  // game continues
}


void test_game() {
  ack();
  Serial.print(read());Serial.print("\n");
  ack();
  press(3);
  
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());Serial.print("\n");
  ack();
  press(3);
  press(2);
  
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());Serial.print("\n");
  ack();
  press(3);
  press(2);
  press(0);
  
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());
  ack();
  Serial.print(read());Serial.print("\n");
  ack();
  press(1);
  Serial.print(read_result());Serial.print("\n");
  
  Serial.print("Test completed.\n");
}
