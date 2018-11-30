#define RELAY_PIN_00 9
#define RELAY_PIN_01 10
#define RELAY_PIN_02 11
#define RELAY_PIN_03 12

byte switch00State;
byte switch01State;
byte switch02State;
byte switch03State;

ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);
ZUNO_SETUP_CHANNELS(
  ZUNO_SWITCH_BINARY(getter_00, setter_00),
  ZUNO_SWITCH_BINARY(getter_01, setter_01),
  ZUNO_SWITCH_BINARY(getter_02, setter_02),
  ZUNO_SWITCH_BINARY(getter_03, setter_03),
  ZUNO_SWITCH_BINARY(getter_03, setter_03)
  );

// the setup routine runs once when you press reset:
void setup() {
  pinMode(RELAY_PIN_00 , OUTPUT);
  pinMode(RELAY_PIN_01 , OUTPUT);
  pinMode(RELAY_PIN_02 , OUTPUT);
  pinMode(RELAY_PIN_03 , OUTPUT);
}

void loop() {
}

void setter_00(byte value) {
  switch (value) {
    case 0: {
      digitalWrite(RELAY_PIN_00, LOW);      
      break;
      };
    default: {
      digitalWrite(RELAY_PIN_00, HIGH);
      }
    }

  switch00State = value;
}

void setter_01(byte value) {
  switch (value) {
    case 0: {
      digitalWrite(RELAY_PIN_01, LOW);      
      break;
      };
    default: {
      digitalWrite(RELAY_PIN_01, HIGH);
      }
    }

  switch01State = value;
}

void setter_02(byte value) {
  switch (value) {
    case 0: {
      digitalWrite(RELAY_PIN_02, LOW);      
      break;
      };
    default: {
      digitalWrite(RELAY_PIN_02, HIGH);
      }
    }

  switch02State = value;
}

void setter_03(byte value) {
  switch (value) {
    case 0: {
      digitalWrite(RELAY_PIN_03, LOW);      
      break;
      };
    default: {
      digitalWrite(RELAY_PIN_03, HIGH);
      }
    }

  switch03State = value;
}

byte getter_00() {
  return switch00State;
}

byte getter_01() {
  return switch01State;
}

byte getter_02() {
  return switch02State;
}

byte getter_03() {
  return switch03State;
}
