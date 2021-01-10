// TODO: split HW, algo and debug

#include "xmasSettings.h"
#include "compatibility.h"

/*
   Constants
*/
/*
  s_pin channel_00 = 9;
  s_pin channel_01 = 10;
*/

const uint8_t stateOn = 0x01;
const uint8_t stateOff = 0x00;

/*
   Variables
*/
uint8_t automaticMode = 1;
uint8_t programNumber = 0;

uint8_t action = 0;
uint8_t groupRepeatCounter = 0xff;

uint8_t patternIndex = 0;
uint8_t globalIndex = 0;
uint8_t groupStartIndex = 0;
uint8_t queueLength = 0;

int8_t incrementBrightness = 0;
int incrementDelay = 0;
uint8_t currentBrightness = 0;
uint8_t currentBrightnessChannel2 = 0;
int currentDelay;

uint8_t startBrightnessPrevious = 0;
uint8_t startBrightness = 0;
int groupDelay = 0;

bool notFirstStart = false; // first start from zero position
bool isPatternStep = true; // pattern step otherwise just delay/brightness step

/*
   Sequences
   [start brightness group 0] [time to raise]
   [start brightness group 1] [time to raise]
   ...
   [start brightness group n] [0b 1XXX XXXX] group repeat]

   [0x00-0xA0] [0b0000 0000 - 0b0001 0000]

   [time to raise]
 	[0b 1XXX XXXX] - reset to 0 brightness
  	[0b 0XXX XXXX] - use next group to 0 brightness
*/

uint8_t programId = 0x00;

const uint8_t queue_00[] = {
	0x00, 0x05,
	0x20, 0x05,
	0x00, 0x05,
	0x40, 0x45,
	0x00, 0x05,
	0x60, 0x05,
	0x00, 0x05,
	0x80, 0x45,
	0x00, 0x05,
	0x90, 0x05,
	0x00, 0x05,
	0xA0, 0x05,
	0x00, 0x05,
	0x90, 0x05,
	0x00, 0x05,
	0x80, 0x45,
	0x00, 0x05,
	0x70, 0x05,
	0x00, 0x05,
	0x60, 0x05,
	0x00, 0x05,
	0x50, 0x05,
	0x00, 0x05,
	0x40, 0x45,
	0x00, 0x05,
	0x30, 0x05,
	0x00, 0x05,
	0x20, 0x05,
	0x00, 0x05,
	0x10, 0x05,

	0x00, 0x81};

const uint8_t queue_01[] = {
	0xFF, 0xFF,
	0xFF, 0x81};

const uint8_t queue_02[] = {
	0x30, 0xFF,
	0x30, 0x81};

const uint8_t queue_03[] = {
	0xFF, 0x4A,
	0xFF, 0x81};

const uint8_t *queue = queue_00;

const uint8_t channelsCount = 8;
uint8_t states[channelsCount] = {
	0x01, // [0] turn on by default
	0x01, // [1] turn on lk#01
	0x01, // [2] turn on lk#02
	0x00, // [3] blink mode enabled
	0x10, // [4] time scale fine
	0x02, // [5] time scale rough
	0x63, // [6] light scale
	0x00  // [7] program id
};

/*
   Z-Wave
*/
BYTE getState(const uint8_t channelIndex)
{
	return states[channelIndex];
}

void setState(const uint8_t channelIndex, const BYTE state)
{
	states[channelIndex] = state;
#ifndef DEBUG_CONSOLE
	zunoSendReport(channelIndex + 1);
#endif
}

void setter_03(BYTE value)
{
	if (states[3] != value)
	{
		setState(3, value);
	}
}

void setter_05(BYTE value)
{
	if (states[5] != value)
	{
		setState(5, value);
	}
}

void setter_06(BYTE value)
{
	if ((states[6] != value) && (0x00 <= value) && (value <= 0x63))
	{
		setState(6, value + 0x01);
	}
}

void setter_07(BYTE value)
{
	if ((states[7] != value) && (0 <= value) && (value < 4))
	{
		switch (value)
		{
		case 0x00:
		{
			break;
		}
		default:
		{
			// enable blink mode for fixed programs
			setter_03(stateOn);
		}
		}
		setState(7, value);
	}
}

void setter_01(BYTE value)
{
	if (states[1] != value)
	{
		switch (value)
		{
		case (uint8_t)0x00:
		{
#ifndef DEBUG_CONSOLE
			analogWrite(PWM4, 0x00);
#endif
			break;
		}
		}
		setState(01, value);
	}
}

void setter_02(BYTE value)
{
	if (states[2] != value)
	{
		switch (value)
		{
		case (uint8_t)0x00:
		{
#ifndef DEBUG_CONSOLE
			analogWrite(PWM2, 0x00);
#endif
			break;
		}
		}
		setState(02, value);
	}
}

void setter_04(BYTE value)
{
	if ((states[4] != value) && (0x00 <= value) && (value <= 0x63))
	{
		setState(4, value + 0x01);
	}
}

void setter_00(BYTE value)
{
	if (states[0] != value)
	{
		switch (value)
		{
		case (uint8_t)0x00:
		{
			BYTE oldState = getState(1);
			setter_01(stateOff);
			setter_01(oldState);
			oldState = getState(2);
			setter_02(stateOff);
			setter_02(oldState);
			break;
		}
		default:
		{
			if (!(states[1] || states[2]))
			{
				setter_01(stateOn);
				setter_02(stateOn);
			}
		}
		}
		setState(0, value);
	}
}

BYTE getter_00()
{
	return getState(0);
}

BYTE getter_01()
{
	return getState(1);
}

BYTE getter_02()
{
	return getState(2);
}

BYTE getter_03()
{
	return getState(3);
}

BYTE getter_04()
{
	return (states[4] - 0x01);
}

BYTE getter_05()
{
	return getState(5);
}

BYTE getter_06()
{
	return (states[6] - 0x01);
}

BYTE getter_07()
{
	return getState(7);
}

/*
   Setup configuration
*/
void setupPinMode()
{
#ifndef DEBUG_CONSOLE
	pinMode(16, OUTPUT);
	pinMode(14, OUTPUT);
#endif
}

void sendReport()
{
#ifndef DEBUG_CONSOLE
	for (uint8_t i = 0; i < channelsCount; i++)
	{
		zunoSendReport(i + 1);
	}
#endif
}

void setup()
{

	setupPinMode();

	//automaticMode = 1;
	//programNumber = 0;

	//patternIndex = 0;
	//globalIndex = 0;
	//groupStartIndex = 0;

	//incrementBrightness = 0;
	//incrementDelay = 0;

	//startBrightness = 0;

	//groupDelay = 0;
	//groupRepeatCounter = (uint8_t)0xff;
	queueLength = sizeof(queue);
	//notFirstStart = false;
	//isPatternStep = true;

	sendReport();
}

uint8_t scaleBrightness(const uint8_t paramBrightness)
{
	return (states[6] * paramBrightness) / 0x64;
}

void resetSegment()
{
	if (++patternIndex >= queueLength)
	{
		// reset to begin of sketch
		patternIndex = (uint8_t)0x0;
		notFirstStart = false;
	}
	groupStartIndex = patternIndex;
	groupRepeatCounter = (uint8_t)0xFF;
}

/*
  return true if reset to 0 (blinking mode) needed
*/
bool switchToNextGroup(uint8_t paramAction)
{
	if (paramAction >> 7)
	{
		switch (groupRepeatCounter)
		{
		case (uint8_t)0xff:
		{
			groupRepeatCounter = (paramAction & (uint8_t)0x7f) - 1;
			if (groupRepeatCounter == 0x0)
			{
				resetSegment();
			}
			else
			{
				patternIndex = groupStartIndex;
			}
			break;
		};
		case (uint8_t)0x00:
		{
			resetSegment();
			break;
		};
		default:
		{
			patternIndex = groupStartIndex;
			groupRepeatCounter--;
			break;
		}
		}
		return false;
	}
	else
	{
		uint8_t multiplier = (states[5] == 0 ? 1 : states[5] * 5);
		groupDelay = (paramAction & 0b00111111) * states[4] * multiplier;
		patternIndex++;
		return (paramAction & 0b01000000);
	}

#ifndef DEBUG_CONSOLE
	delay(1000);
#endif
}

void runSegment()
{
	isPatternStep = false;

	currentBrightness = startBrightnessPrevious;
	currentBrightnessChannel2 = startBrightness;
	currentDelay = groupDelay;

	if (startBrightness == startBrightnessPrevious)
	{
		incrementDelay = groupDelay;
	}
	else
	{
		incrementDelay = (groupDelay / (startBrightness - startBrightnessPrevious));
	}

	if (incrementDelay < 0)
	{
		incrementDelay = -incrementDelay;
		incrementBrightness = -1;
	}
	else
	{
		incrementBrightness = 1;
	}
}

void processSegment()
{
	if ((currentDelay <= 0) || (currentBrightness == startBrightness))
	{
		isPatternStep = true;
	}

	if (states[1] != stateOff)
	{
#ifndef DEBUG_CONSOLE
		analogWrite(PWM4, currentBrightness);
#endif
	}
	if (states[2] != stateOff)
	{
#ifndef DEBUG_CONSOLE
		analogWrite(PWM2, currentBrightnessChannel2);
#endif
	}
#ifndef DEBUG_CONSOLE
	delay(incrementDelay);
#else

	/*
    printf(
    	"| Channel 0: %02x | Channel 1: %02x | Delay: %04d | Increment: %02d | \n",
        paramCurrentBrightness,
        currentBrightnessChannel2,
        currentDelay,
        incrementDelay);
  */

	char displayProgress_0[0xFF];
	char displayProgress_1[0xFF];
	uint8_t toFill_0 = currentBrightness < 0xFE ? currentBrightness : 0xFE;
	uint8_t toFill_1 = currentBrightnessChannel2 < 0xFE ? currentBrightnessChannel2 : 0xFE;

	memset(displayProgress_0, '\0', 0xFF);
	memset(displayProgress_1, ' ', 0xFE);

	displayProgress_1[0xFE] = '\0';
	// printf("\r%s", displayProgress);
	//fflush(stdout);

	memset(displayProgress_0, '*', toFill_0);
	memset((displayProgress_1 + (0xFE - toFill_1)), '#', toFill_1);
	// printf("\r%s", displayProgress);
	// fflush(stdout);
	printf("%s\n", displayProgress_0);
	printf("%s\n", displayProgress_1);

	Sleep(incrementDelay);
#endif

	currentBrightnessChannel2 = currentBrightnessChannel2 - incrementBrightness;
	currentDelay = currentDelay - incrementDelay;
	currentBrightness = currentBrightness + incrementBrightness;
}

void loop()
{
	if (states[0] != stateOff)
	{

		if (programId != states[7])
		{
			switch (states[7])
			{
			case 0x00:
			{
				queue = queue_00;
				break;
			}
			case 0x01:
			{
				queue = queue_01;
				break;
			}
			case 0x02:
			{
				queue = queue_02;
				break;
			}
			case 0x03:
			{
				queue = queue_03;
				break;
			}
			}
			programId = states[7];
			resetSegment();
		}
		else
		{
			if (isPatternStep)
			{
				action = queue[patternIndex];

				if (patternIndex & 0x01)
				{
					if (switchToNextGroup(action) && (states[3] != stateOff)) //&&&&&
					{
						startBrightness = 0;
					}
				}
				else
				{
					if (notFirstStart)
					{
						startBrightnessPrevious = startBrightness;
						startBrightness = scaleBrightness(action);
						runSegment();
					}
					else
					{
						// first run, should initialize startBrightness
						notFirstStart = true;
						startBrightness = scaleBrightness(action);
					}
					patternIndex++;
				}
			}
			else
			{
				processSegment();
			}
		}
	}
	else
	{
#ifdef DEBUG_CONSOLE
		Sleep(5000);
#else
		delay(500);
#endif
	}
}
