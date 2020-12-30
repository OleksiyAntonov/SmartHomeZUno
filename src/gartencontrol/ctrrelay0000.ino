/*
   Setup global configuration
*/
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);
ZUNO_SETUP_CHANNELS(
  ZUNO_SWITCH_BINARY(getter_00, setter_00),
  ZUNO_SWITCH_BINARY(getter_01, setter_01),
  ZUNO_SWITCH_BINARY(getter_02, setter_02)
);

/*
   Constants
*/

/*
   States
*/
const uint8_t state_off = 0;
const uint8_t state_on = 1;

const uint8_t channelsCount = 3;

/*
   Variables
*/
const uint8_t channels[channelsCount] = {9, 10, 11};
uint8_t states[channelsCount] = {0, 0, 0};

/*
   Callbacks stubs
*/
BYTE getState(const uint8_t channelIndex) {
  return states[channelIndex];
}

void setState(const uint8_t channelIndex, const BYTE state) {
  if (state == 0) {
    digitalWrite(channels[channelIndex], LOW);  
  } else {
    digitalWrite(channels[channelIndex], HIGH);  
  }

  states[channelIndex] = state;
  zunoSendReport(channelIndex + 1);
}

void setter_00(BYTE value) {
  setState(0, value);
}

void setter_01(BYTE value) {
  setState(1, value);
}

void setter_02(BYTE value) {
  setState(2, value);
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

/*
   Run
*/
void setup() {
  for (uint8_t i = 0; i < channelsCount; i++) {
    pinMode(channels[i], OUTPUT);
    zunoSendReport(i + 1);
  }
}

void loop() {
}
