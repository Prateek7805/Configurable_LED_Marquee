#define COLSIZE 16
#define ROWSIZE 8
#define INVERT 0

#define BRIGHT_PERIOD 1050

const char NOARGS[] PROGMEM = "No Args";
const uint32_t NUM_DISP =  (2*(COLSIZE/16))+1;
const uint32_t COLCHUNK = COLSIZE/8;

const uint8_t INVERTMASK = (INVERT == 1) ? 0xFF : 0;

const uint32_t ROWMASK = (COLSIZE - 1);
const uint8_t COLMASK = 0xFF;

/*FILE*/
struct FileProperties {
  String HISTORY_DEFAULT = "Hello";
  String SELECTTEXT = "/t0.bin";
  String SELECTTEXT_DEFAULT = "Select";
  String CREDENTIALS = "/creds.bin";
  String CREDENTIAL_DEFAULT = "ConfigurableLED\n\n\n";
  String VARIABLES = "/var.bin";
  String VARIABLES_DEFAULT = "1\n1\nt\n";
  String SETTINGS = "/settings.bin";
  String SETTINGS_DEFAULT = "16\n100\n0\n0\nscroll\n";
  String PIXELS = "/pixels.bin";
  String PIXELS_DEFAULT = "00000000000000000000000000000000";
  String PIXEL_LEFT = "/pixell.bin";
  String PIXEL_RIGHT = "/pixelr.bin";
  String PIXEL_LEFT_DEFAULT = "0000";
  String PIXEL_RIGHT_DEFAULT = "0000";

} FP;
/*END FILE*/
/*Credentials*/
struct Credentials {
  String AP_SSID = "pocketMatrix";
  String AP_PASS = "";
  String STA_SSID = "";
  String STA_PASS = "";
} creds;
/*END Credentials*/
/*TIMING*/
struct TIMING {
  unsigned long cScroll = 0;
  unsigned long brightness = 0;
  unsigned long pixelSave = 0;
} timing;
/*END TIMING*/
/*LIMITS*/
struct TIMINGLIMITS {
  unsigned long SEC_PER_COL = 0;
  unsigned long BRIGHT_HIGH = 0;
  unsigned long BRIGHT_LOW = 0;
  unsigned long PIXEL_SAVE = 2e6;
} limit;
/*END LIMITS*/
/*VARS*/
struct vars {
  uint16_t NUM_TEXTS = 1;
  uint16_t CURR_TEXT = 1;
  String CURR_PRE = "t";
  bool skip = false;
  bool pixelUpdate = false;
} controls;
/*END VARS*/
/*SETTINGS*/
struct setting {
  uint16_t SPEED = 16; //num columns per second -> say 16
  uint8_t BRIGHT_DUTY_C = 100;
  uint8_t WIFIMODE = 0;
  uint8_t RESETMODE = 0;
  String ANIM = "scroll";
} settings;
/* END SETTINGS */
/*PIXELS*/
struct PIX_ART {
  uint8_t BUFFER[COLSIZE] = {0,};
  uint8_t * LEFT;
  uint16_t LEFT_LENGTH = 0;
  uint8_t * RIGHT;
  uint16_t RIGHT_LENGTH = 0;
} pix;

/*END PIXELS*/
