#define DEFAULT_SPEED 5

CRGB DEFAULT_COLOR_LEFT = CRGB::Red;
CRGB DEFAULT_COLOR_RIGHT = CRGB::Purple;

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
