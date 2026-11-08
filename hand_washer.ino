/*
  Automatic Touchless Hand Washer
  Board: Arduino Uno or compatible

  HC-SR04:
    TRIG -> D9
    ECHO -> D10

  Relay module:
    IN -> D7

  This sketch assumes an ACTIVE-HIGH relay:
    HIGH = pump ON
    LOW  = pump OFF

  If your relay module is active-LOW, invert the output levels
  in setPump() and verify the pump stays OFF during startup.
*/

#define TRIG_PIN 9
#define ECHO_PIN 10
#define RELAY_PIN 7

const unsigned int DETECTION_DISTANCE_CM = 10;
const unsigned long PUMP_RUNTIME_MS = 5000;
const unsigned long COOLDOWN_MS = 2000;
const unsigned long ECHO_TIMEOUT_US = 30000;

unsigned long durationUs = 0;
float distanceCm = 0.0f;

void setPump(bool on) {
  digitalWrite(RELAY_PIN, on ? HIGH : LOW);
}

float measureDistanceCm() {
  // Ensure a clean trigger pulse.
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // A zero result means no echo was received before the timeout.
  durationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (durationUs == 0) {
    return -1.0f;
  }

  // Sound travels to the target and back, so divide by two.
  return (durationUs * 0.0343f) / 2.0f;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Keep the pump switched off at startup.
  setPump(false);

  Serial.begin(9600);
  Serial.println(F("Automatic Touchless Hand Washer ready."));
}

void loop() {
  distanceCm = measureDistanceCm();

  Serial.print(F("Distance: "));
  if (distanceCm < 0.0f) {
    Serial.println(F("No echo"));
  } else {
    Serial.print(distanceCm, 1);
    Serial.println(F(" cm"));
  }

  // Activate only for a valid reading within the detection range.
  if (distanceCm > 0.0f &&
      distanceCm <= DETECTION_DISTANCE_CM) {
    Serial.println(F("Object detected - pump ON"));

    setPump(true);
    delay(PUMP_RUNTIME_MS);

    setPump(false);
    Serial.println(F("Pump OFF - cooldown"));

    delay(COOLDOWN_MS);
  }

  delay(200);
}
