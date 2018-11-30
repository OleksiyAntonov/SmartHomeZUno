/*
 * Setup global configuration
*/
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);
ZUNO_SETUP_CHANNELS(
  ZUNO_SWITCH_BINARY(getter_00, setter_00),
  ZUNO_SWITCH_BINARY(getter_01, setter_01),
  ZUNO_SWITCH_BINARY(getter_02, setter_02),
  ZUNO_SWITCH_BINARY(getter_03, setter_03)
);

/*
 * Constants
*/
s_pin channel_00 = 9;
s_pin channel_01 = 10;
s_pin channel_02 = 11;
s_pin channel_03 = 12;

const uint8_t channelMaxIndex = 3; // channel count is relayMaxIndex + 1
// s_pin channels[channelMaxIndex + 1] = {9, 10, 11, 12};

/*
 * Variables
*/
uint8_t states[channelMaxIndex + 1] = {0, 0, 0, 0};

/*
 * Callbacks
*/
BYTE getState(const uint8_t channelIndex) {
  BYTE result = -1;
  for (uint8_t i = 0; i <= channelMaxIndex; i++) {
    if (i == channelIndex) {
      result = states[channelIndex];
      break;
    }
  }

  return result;
}

void setState(const uint8_t channelIndex, const BYTE state) {
  for (uint8_t i = 0; i <= channelMaxIndex; i++) {
    if (i == channelIndex) {
      if (state == 0) {
        switch (channelIndex) {
          case 0: {
            digitalWrite(channel_00, LOW);      
            break;
          };
          case 1: {
            digitalWrite(channel_01, LOW);      
            break;
          };
          case 2: {
            digitalWrite(channel_02, LOW);      
            break;
          };
          case 3: {
            digitalWrite(channel_03, LOW);      
            break;
          }
        }
      } else {
        switch (channelIndex) {
          case 0: {
            digitalWrite(channel_00, HIGH);      
            break;
          };
          case 1: {
            digitalWrite(channel_01, HIGH);      
            break;
          };
          case 2: {
            digitalWrite(channel_02, HIGH);      
            break;
          };
          case 3: {
            digitalWrite(channel_03, HIGH);      
            break;
          }
        }
      }

      states[channelIndex] = state;
      break;
    }
  }
}

/*
 * Setup configuration
*/
void setupPinMode() {
  pinMode(channel_00, OUTPUT);
  pinMode(channel_01, OUTPUT);
  pinMode(channel_02, OUTPUT);
  pinMode(channel_03, OUTPUT);
}

/*
 * Callbacks stubs
*/
void setter_00(BYTE value) {
  setState(0, value);
}

void setter_01(BYTE value) {
  setState(1, value);
}

void setter_02(BYTE value) {
  setState(2, value);
}

void setter_03(BYTE value) {
  setState(3, value);
}

BYTE getter_00() {
  return getState(00);
}

BYTE getter_01() {
  return getState(01);
}

BYTE getter_02() {
  return getState(02);
}

BYTE getter_03() {
  return getState(03);
}

/*
 * Run
*/
void setup() {
  setupPinMode();
}

void loop() {
}
