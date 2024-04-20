// Visualizations
#define VIZ_DEFAULT 0
#define VIZ_TWINKLE 1
#define VIZ_LASERS 2
#define VIZ_WINDSHIELD 3

// Defaults
#define DEFAULT_SPEED 5
#define DEFAULT_COLOR_LEFT CRGB(255, 0, 0)    // Red
#define DEFAULT_COLOR_RIGHT CRGB(128, 0, 128) // Purple
#define DEFAULT_VIZ 0

// Knob Actions
#define ACTION_SET_COLOR_LEFT 1
#define ACTION_SET_COLOR_RIGHT 2
#define ACTION_SPEED 3

// Button Actions
#define ACTION_SET_BACKGROUND 5
#define ACTION_STROBE_OFF 6
#define ACTION_STROBE_ON 7

uint8_t receiverAddress1[] = {0xA8, 0x42, 0xE3, 0xAB, 0x80, 0xD4};
uint8_t receiverAddress2[] = {0xA0, 0xA3, 0xB3, 0x2D, 0x4E, 0x58};
uint8_t senderAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char receiverMacAddress1[] = "A8:42:E3:AB:80:D4";
char receiverMacAddress2[] = "A0:A3:B3:2D:4E:58";
char senderMacAddress[] = "";

typedef struct msg {
  uint8_t action;
  int value;
} msg;

void send(msg m) { esp_now_send(0, (uint8_t *)&m, sizeof(m)); }
