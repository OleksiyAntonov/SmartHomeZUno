// Sketch composition
#include "xmasSettings.h"
#include "xmasImpl.h"

/*
  Script configuration
    16: PWM4 - lichterkette #LK0
    14: PWM2 - lichterkette #LK1

  Z-Wave channels
    00: [SWITCH_BINARY Instance_1:_Switch_1] turn on/off both
    01: [SWITCH_BINARY Instance_2:_Switch_2] turn on/off LK#0
    02: [SWITCH_BINARY Instance_3:_Switch_3] turn on/off LK#1
    03: [SWITCH_BINARY Instance_4:_Switch_4] enable blink steps
    04: [SWITCH_MULTILEVEL Instance_1:_Level_1] time scale fine (%)
        - min 0x00: equal of XX ms [virtual delay (depends on hw performance), XX = param06 * 5 ms]
        - max 0x63: equal of (0x63 + 0x001) * XX
    05: [SWITCH_MULTILEVEL Instance_2:_Level_2] time scale rough (%)
    06: [SWITCH_MULTILEVEL Instance_3:_Level_3] light scale for program / max light (0..99%) (default 0x63)
        - min 0x00
        - max 0x63: 100% light, equal 0xFF in program
    07: [SWITCH_MULTILEVEL Instance_4:_Level_4] program id
        0: default scheme
        1: static light (full bright 0xff)
        2: static light (dim bright 0xff)
        3: pulse (time scale based)
*/

/*
   Z-Uno setup
   See file header for the detailed specifications
*/
#ifndef DEBUG_CONSOLE
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);
ZUNO_SETUP_CHANNELS(
  ZUNO_SWITCH_BINARY(getter_00, setter_00),
  ZUNO_SWITCH_BINARY(getter_01, setter_01),
  ZUNO_SWITCH_BINARY(getter_02, setter_02),
  ZUNO_SWITCH_BINARY(getter_03, setter_03),
  ZUNO_SWITCH_MULTILEVEL(getter_04, setter_04),
  ZUNO_SWITCH_MULTILEVEL(getter_05, setter_05),
  ZUNO_SWITCH_MULTILEVEL(getter_06, setter_06),
  ZUNO_SWITCH_MULTILEVEL(getter_07, setter_07));
#endif
