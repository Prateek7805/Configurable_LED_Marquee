#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<FS.h>
#include<WebSocketsServer.h>
#include<SPI.h>
#include "ASCII_PROGMEM_TABLE.h"
#include "index.h"
#include "styles.h"
#include "script.h"
#include "VARIABLES.h"
#include "tools.h"
uint8_t DATA = 13;
uint8_t CLOCK = 14;
uint8_t LATCH = 4;
uint32_t currentInd = 0;

String TEXT = "Hello  ";

uint8_t BUFFER[COLSIZE];
uint8_t SPIBUFFER[NUM_DISP];

IPAddress apIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 12, 7);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);



void returnOK() {
  server.send(200, "text/plain", "");
}
void returnNotFound(String msg) {
  server.send(400, "text/plain", msg);
}
void returnErr() {
  server.send_P(500, "text/plain", NOARGS);
}
void handleOtherFiles() {
  if (checkInFS(server.uri())) {
    return;
  }
  returnNotFound("Oops check the url");
}

bool checkInFS(String path) {
  String dataType = "text/plain";
  const char* data;
  if (path.endsWith("/") || path.endsWith(".html")) {
    dataType = "text/html";
    data = _index;
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
    data = _styles;
  } else if (path.endsWith(".js")) {
    dataType = "application/js";
    data = _script;
  } else {
    return false;
  }
  server.send_P(200, dataType.c_str(), data);
  return true;
}
void saveVariables() {
  File file = SPIFFS.open(FP.VARIABLES, "w");
  file.print(String(controls.NUM_TEXTS) + "\n" + String(controls.CURR_TEXT) + "\n" + controls.CURR_PRE + "\n");
  file.close();
}
void saveSettings() {
  File file = SPIFFS.open(FP.SETTINGS, "w");
  file.print(String(settings.SPEED) + "\n" + String(settings.BRIGHT_DUTY_C) + "\n" + String(settings.WIFIMODE) + "\n" + String(settings.RESETMODE) + "\n" + String(settings.ANIM) + "\n");
  file.close();
}
void saveCreds() {
  File file = SPIFFS.open(FP.CREDENTIALS, "w");
  file.print(creds.AP_SSID + "\n" + creds.AP_PASS  + "\n" + creds.STA_SSID + "\n" + creds.STA_PASS  + "\n");
  file.close();
}
void pushHISTORY(String &txt) {
  controls.NUM_TEXTS++;
  controls.CURR_TEXT = controls.NUM_TEXTS;
  controls.CURR_PRE = "t";
  saveVariables();
  File file = SPIFFS.open("/t" + String(controls.NUM_TEXTS) + ".bin", "w");
  file.print(txt);
  file.close();
}

void updateTEXT() {
  if (!server.hasArg("data")) {
    returnErr();
    return;
  }
  String txt = server.arg(0);
  pushHISTORY(txt);
  TEXT = server.arg(0) + "  ";
  controls.skip = true;
  returnOK();
}

void createFSFile(String &namae, String &content) {
  File file = SPIFFS.open(namae, "w");
  file.print(content);
  file.close();
}
void checkFilesFS() {
  String fileName = "/" + controls.CURR_PRE + String(controls.CURR_TEXT) + ".bin";
  if (controls.CURR_TEXT != 0) {
    if (!SPIFFS.exists(fileName)) createFSFile(fileName, FP.HISTORY_DEFAULT);
  }
  if (!SPIFFS.exists(FP.SELECTTEXT)) createFSFile(FP.SELECTTEXT, FP.SELECTTEXT_DEFAULT);
  if (!SPIFFS.exists(FP.CREDENTIALS)) createFSFile(FP.CREDENTIALS, FP.CREDENTIAL_DEFAULT);
  if (!SPIFFS.exists(FP.VARIABLES)) createFSFile(FP.VARIABLES, FP.VARIABLES_DEFAULT);
  if (!SPIFFS.exists(FP.SETTINGS)) createFSFile(FP.SETTINGS, FP.SETTINGS_DEFAULT);
  if (!SPIFFS.exists(FP.PIXELS)) createFSFile(FP.PIXELS, FP.PIXELS_DEFAULT);
  if (!SPIFFS.exists(FP.PIXEL_LEFT)) createFSFile(FP.PIXEL_LEFT, FP.PIXEL_LEFT_DEFAULT);
  if (!SPIFFS.exists(FP.PIXEL_RIGHT)) createFSFile(FP.PIXEL_RIGHT, FP.PIXEL_RIGHT_DEFAULT);
  getScrollText(fileName);
}

void loadVariables() {
  File file = SPIFFS.open(FP.VARIABLES, "r");
  if (!file) {
    return;
  }
  controls.NUM_TEXTS = file.readStringUntil('\n').toInt();
  controls.CURR_TEXT = file.readStringUntil('\n').toInt();
  controls.CURR_PRE = file.readStringUntil('\n');
  file.close();
}
void loadSettings() {
  File file = SPIFFS.open(FP.SETTINGS, "r");
  if (!file) {
    return;
  }

  settings.SPEED = file.readStringUntil('\n').toInt();
  settings.BRIGHT_DUTY_C = file.readStringUntil('\n').toInt();
  settings.WIFIMODE = file.readStringUntil('\n').toInt();
  settings.RESETMODE = file.readStringUntil('\n').toInt();
  settings.ANIM = file.readStringUntil('\n');
  file.close();
}

void loadCreds() {
  File file = SPIFFS.open(FP.CREDENTIALS, "r");
  if (!file) {
    return;
  }
  creds.AP_SSID = file.readStringUntil('\n');
  creds.AP_PASS = file.readStringUntil('\n');
  creds.STA_SSID = file.readStringUntil('\n');
  creds.STA_PASS = file.readStringUntil('\n');
  Serial.println(creds.AP_SSID);
  Serial.println(creds.AP_PASS);
  Serial.println(creds.STA_SSID);
  Serial.println(creds.STA_PASS);
  file.close();
}


void initSetup() {
  loadVariables();
  loadSettings();
  checkFilesFS();
  loadCreds();
  loadPixels();
  loadPixelL();
  loadPixelR();
}

String getText(String &Path) {
  File file = SPIFFS.open(Path, "r");
  String txt = file.readString();
  Serial.print(txt);
  Serial.println(txt.length());
  file.close();
  return txt;
}
void getScrollText(String &Path) {
  TEXT = getText(Path);
  if (controls.CURR_PRE == "t") {
    for (uint8_t i = 0; i < COLCHUNK; i++) {
      TEXT += " ";
    }
  } else if (controls.CURR_PRE == "p") {
    for (uint8_t i = 0; i < COLCHUNK; i++) {
      TEXT += "0000000000000000";
    }
  }
}
void dispText() {
  if (!server.hasArg("ind")) {
    returnErr();
    return;
  }
  String ARG = server.arg(0);
  controls.CURR_TEXT = ARG.substring(1).toInt();
  controls.CURR_PRE = ARG[0];
  String Path = "/" + ARG + ".bin";
  getScrollText(Path);
  saveVariables();
  controls.skip = true;
  returnOK();
}

void deleteTEXT() {
  if (!server.hasArg("ind")) {
    returnErr();
    return;
  }
  String delPos = server.arg(0);
  uint16_t pos = delPos.substring(1).toInt();
  if (pos == controls.CURR_TEXT) {
    controls.CURR_TEXT = 0;
    controls.CURR_PRE = "t";
    TEXT = getText(FP.SELECTTEXT) + "  ";
    saveVariables();
    controls.skip = true;
  }
  if (!SPIFFS.remove("/" + delPos + ".bin")) {
    returnErr();
    return;
  }
  returnOK();
}


void sendHistory() {
  Dir dir = SPIFFS.openDir("/");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  server.sendContent("[");

  while (dir.next()) {
    String fileName = dir.fileName();
    if (fileName != FP.VARIABLES && fileName != FP.CREDENTIALS && fileName != FP.SELECTTEXT && fileName != FP.SETTINGS && fileName != FP.PIXELS && fileName != FP.PIXEL_RIGHT && fileName != FP.PIXEL_LEFT) {
      String Position = fileName.substring(1, fileName.lastIndexOf('.'));
      char type = Position[0];

      server.sendContent("{\"p\":\"" + Position + "\",");
      if (type == 't') {
        server.sendContent("\"t\":\"" + getText(fileName)); // remove the \n
      } else {
        server.sendContent("\"t\":\"ok"); // remove the \n
      }

      server.sendContent("\"},");
    }
  }
  server.sendContent("]");
  server.sendContent("");
}
void sendVariables() {
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  server.sendContent("{");
  String data = "\"S\":" + String(settings.SPEED) + ",";
  data += "\"B\":" + String(settings.BRIGHT_DUTY_C) + ",";
  data += "\"W\":" + String(settings.WIFIMODE) + ",";
  data += "\"R\":" + String(ROWSIZE) + ",";
  data += "\"C\":" + String(COLSIZE) + ",";
  data += "\"A\":\"" + settings.ANIM + "\"}";
  server.sendContent(data);
  server.sendContent("");
}


void savePixelsFS() {
  controls.NUM_TEXTS++;
  File file = SPIFFS.open("/p" + String(controls.NUM_TEXTS) + ".bin", "w");
  String txt = "";
  char MSB, LSB;
  if (pix.LEFT_LENGTH > 0) {
    for (uint16_t i = 0; i < pix.LEFT_LENGTH; i++) {
      getCharHex(pix.LEFT[i], &MSB, &LSB);
      txt += MSB;
      txt += LSB;
    }
  }
  for (uint16_t i = 0 ; i < COLSIZE; i++) {
    getCharHex(pix.BUFFER[i], &MSB, &LSB);
    txt += MSB;
    txt += LSB;
  }
  if (pix.RIGHT_LENGTH > 0) {
    for (int32_t i = pix.RIGHT_LENGTH - 1; i >= 0; i--) {
      getCharHex(pix.RIGHT[i], &MSB, &LSB);
      txt += MSB;
      txt += LSB;
    }
  }
  file.print(txt);
  file.close();
}

void sendPixels() {
  String PIXELS = "[[";
  for (uint16_t i = 0; i < pix.LEFT_LENGTH; i++) {
    PIXELS += String(pix.LEFT[i]) + ",";
  }
  if (pix.LEFT_LENGTH != 0) {
    PIXELS = PIXELS.substring(0, PIXELS.length() - 1);
  }
  PIXELS += "],[";

  for (uint16_t i = 0; i < COLSIZE; i++) {
    PIXELS += String(pix.BUFFER[i]) + ",";
  }
  PIXELS = PIXELS.substring(0, PIXELS.length() - 1) + "],[";

  for (uint16_t i = 0; i < pix.RIGHT_LENGTH; i++) {
    PIXELS += String(pix.RIGHT[i]) + ",";
  }

  if (pix.RIGHT_LENGTH != 0) {
    PIXELS = PIXELS.substring(0, PIXELS.length() - 1);
  }
  PIXELS += "]]";

  server.send(200, "text/json", PIXELS);
}

void renameTEXT() {
  if (!(server.hasArg("ind") && server.hasArg("data"))) {
    returnErr();
    return;
  }
  String ind = server.arg(0);
  String fileName = "/" + ind + ".bin";
  uint16_t pos = ind.substring(1).toInt();
  String content = server.arg(1);
  createFSFile(fileName, content);
  if (controls.CURR_TEXT == pos) {
    TEXT = server.arg(1) + "  ";
    controls.skip = true;
  }
  returnOK();
}

void updateSettings() {
  if (!(server.hasArg("type") && server.hasArg("val"))) {
    returnErr();
    return;
  }
  String type = server.arg(0);
  if (type == "anim") {
    settings.ANIM = server.arg(1);
    controls.skip = true;
  } else if (type == "speed") {
    uint8_t val = server.arg(1).toInt();
    if (val > 100) {
      returnErr();
      return;
    }
    settings.SPEED = server.arg(1).toInt();
  } else if (type == "bright") {
    uint8_t val = server.arg(1).toInt();
    if (val > 100) {
      returnErr();
      return;
    }
    settings.BRIGHT_DUTY_C = server.arg(1).toInt();
  } else if (type == "WMODE") {
    settings.WIFIMODE = server.arg(1).toInt();
  }
  else if (type == "RESET") {
    settings.RESETMODE = server.arg(1).toInt();
    switch (settings.RESETMODE) {
      case 0:
        ESP.restart();
        break;
      case 1:
        controls.CURR_TEXT = 0;
        controls.CURR_PRE = "t";
        saveVariables();
        checkFilesFS();
        controls.skip = true;
        break;
      case 2:
        Dir dir = SPIFFS.openDir("/");
        while (dir.next()) {
          SPIFFS.remove(dir.fileName());
        }
        ESP.restart();
        break;
    }
  } else {
    returnErr();
    return;
  }

  saveSettings();
  returnOK();
}

void updateCreds() {
  if (!(server.hasArg("type") && server.hasArg("ssid") && server.hasArg("pass"))) {
    returnErr();
    return;
  }
  String type = server.arg(0);
  String ssid = server.arg(1);
  String pass = server.arg(2);
  Serial.println(type);
  Serial.println(ssid);
  Serial.println(pass);
  if (type == "STA") {
    creds.STA_SSID = ssid;
    creds.STA_PASS = pass;
  } else if (type == "AP") {
    creds.AP_SSID = ssid;
    creds.AP_PASS = pass;
  }
  saveCreds();
  returnOK();
}


void updatePixels() {
  if (!(server.hasArg("px") && server.hasArg("val"))) {
    returnErr();
    return;
  }
  uint16_t px = server.arg(0).toInt();
  uint8_t val = server.arg(1).toInt();
  pix.BUFFER[px] = val;
  returnOK();
}
void savePixels() {
  File file = SPIFFS.open(FP.PIXELS, "w");
  for (uint16_t i = 0; i < COLSIZE; i++) {
    char MSB, LSB;
    getCharHex(pix.BUFFER[i], &MSB, &LSB);
    file.write(MSB);
    file.write(LSB);
  }
  file.close();
}
void savePixelL() {
  File file = SPIFFS.open(FP.PIXEL_LEFT, "w");
  char N1 = ((pix.LEFT_LENGTH >> 12) & 0x0f) + '0';
  char N2 = ((pix.LEFT_LENGTH >> 8) & 0x0f) + '0';
  char N3 = ((pix.LEFT_LENGTH >> 4) & 0x0f) + '0';
  char N4 = ((pix.LEFT_LENGTH) & 0x0f) + '0';
  file.write(N1);
  file.write(N2);
  file.write(N3);
  file.write(N4);
  char MSB, LSB;
  for (uint16_t i = 0; i < pix.LEFT_LENGTH; i++) {
    getCharHex(pix.LEFT[i], &MSB, &LSB);
    file.write(MSB);
    file.write(LSB);
  }
  file.close();
}
void savePixelR() {
  File file = SPIFFS.open(FP.PIXEL_RIGHT, "w");
  char N1 = ((pix.RIGHT_LENGTH >> 12) & 0x0f) + '0';
  char N2 = ((pix.RIGHT_LENGTH >> 8) & 0x0f) + '0';
  char N3 = ((pix.RIGHT_LENGTH >> 4) & 0x0f) + '0';
  char N4 = ((pix.RIGHT_LENGTH) & 0x0f) + '0';
  file.write(N1);
  file.write(N2);
  file.write(N3);
  file.write(N4);
  for (uint16_t i = 0; i < pix.RIGHT_LENGTH; i++) {
    char MSB, LSB;
    getCharHex(pix.RIGHT[i], &MSB, &LSB);
    file.write(MSB);
    file.write(LSB);
  }
  file.close();
}
void loadPixels() {
  File file = SPIFFS.open(FP.PIXELS, "r");
  uint8_t MSB, LSB;
  for (uint8_t i = 0; i < COLSIZE; i++) {
    MSB = file.read() - 48;
    LSB = file.read() - 48;
    pix.BUFFER[i] = (MSB << 4 | LSB);
  }
  Serial.println("pixels loaded");
  file.close();
}
void loadPixelL() {
  File file = SPIFFS.open(FP.PIXEL_LEFT, "r");
  uint8_t N1, N2, N3, N4;
  N1 = file.read() - '0';
  N2 = file.read() - '0';
  N3 = file.read() - '0';
  N4 = file.read() - '0';
  pix.LEFT_LENGTH = (N1 << 12) | (N2 << 8) | (N3 << 4) | (N4);
  Serial.println(pix.LEFT_LENGTH);
  if (pix.LEFT_LENGTH == 0) {
    file.close();
    return;
  }
  pix.LEFT = (uint8_t * )malloc(pix.LEFT_LENGTH);
  for (uint8_t i = 0; i < pix.LEFT_LENGTH; i++) {
    uint8_t MSB, LSB;
    MSB = file.read() - 48;
    LSB = file.read() - 48;
    pix.LEFT[i] = (MSB << 4 | LSB);
  }
  Serial.println("pixel left loaded");
  file.close();
}
void loadPixelR() {
  File file = SPIFFS.open(FP.PIXEL_RIGHT, "r");
  uint8_t N1, N2, N3, N4;
  N1 = file.read() - '0';
  N2 = file.read() - '0';
  N3 = file.read() - '0';
  N4 = file.read() - '0';
  pix.RIGHT_LENGTH = (N1 << 12) | (N2 << 8) | (N3 << 4) | (N4);
  Serial.println(pix.RIGHT_LENGTH);
  if (pix.RIGHT_LENGTH == 0) {
    file.close();
    return;
  }
  pix.RIGHT = (uint8_t * )malloc(pix.RIGHT_LENGTH);
  for (uint8_t i = 0; i < pix.RIGHT_LENGTH; i++) {
    uint8_t MSB, LSB;
    MSB = file.read() - 48;
    LSB = file.read() - 48;
    pix.RIGHT[i] = (MSB << 4 | LSB);
  }
  Serial.println("pixel right loaded");
  file.close();
}

void clearPixels() {
  clearBuffer(pix.BUFFER);
  pix.LEFT_LENGTH = 0;
  pix.RIGHT_LENGTH = 0;
  free(pix.LEFT);
  free(pix.RIGHT);
}
void setup() {
  Serial.begin(115200);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(LATCH, OUTPUT);

  SPI.begin();
  SPIFFS.begin();
  clearDisplay();
  initSetup();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("pocketMatrix");
  Serial.println("Connected");

  server.on("/history", HTTP_GET, sendHistory);
  server.on("/status", HTTP_GET, sendVariables);
  server.on("/text", HTTP_POST, updateTEXT);
  server.on("/disp", HTTP_GET, dispText);
  server.on("/delete", HTTP_GET, deleteTEXT);
  server.on("/rename", HTTP_POST, renameTEXT);
  server.on("/pixelupdate", HTTP_GET, updatePixels);
  server.on("/setting", HTTP_GET, updateSettings);
  server.on("/creds", HTTP_POST, updateCreds);
  server.on("/pixels", HTTP_GET, sendPixels);
  server.on("/favicon.ico", HTTP_GET, []() {
    server.send(200, "image/png", "");
  });
  server.onNotFound(handleOtherFiles);

  server.begin();
  webSocket.begin();
  webSocket.onEvent(socketHandle);

}

void loop() {
  if (settings.ANIM == "scroll") {
    scroll(TEXT);
  } else if (settings.ANIM == "ledone") {
    ledone(TEXT);
  } else if (settings.ANIM == "ledoff") {

  } else if (settings.ANIM == "pixel") {
    pixelArt();
  }

  server.handleClient();
  webSocket.loop();
}

void latch() {
  delayMicroseconds(10);
  digitalWrite(LATCH, HIGH);
  delayMicroseconds(10);
  digitalWrite(LATCH, LOW);
}

void updateFrame(uint8_t * FRAME) {
  limit.BRIGHT_HIGH = (uint32_t)((float)(BRIGHT_PERIOD * settings.BRIGHT_DUTY_C / 100));
  limit.BRIGHT_LOW = (uint32_t)((float)(BRIGHT_PERIOD * (100 - settings.BRIGHT_DUTY_C) / 100));
  for (int row = 0; row < ROWSIZE; ++row) {
    server.handleClient();
    webSocket.loop();
    if (limit.BRIGHT_HIGH != 0) {
      uint32_t col = (1 << (ROWSIZE - (row + 1)));
      uint32_t CurrentCol = 0;
      for (uint16_t k = 0; k < COLSIZE; k++) {
        CurrentCol |= (((((FRAME[COLSIZE - k - 1] << row) & 0x80)) >> 7) << k);
      }
      makeSPIBuffer(CurrentCol, ~col);
      mprint();
    }
    timing.brightness = micros();
    while (micros() - timing.brightness < limit.BRIGHT_HIGH) {
      webSocket.loop();
    }
    timing.brightness = micros();
    if (limit.BRIGHT_LOW != 0) {
      clearDisplay();
    }
    while (micros() - timing.brightness < limit.BRIGHT_LOW) {
      webSocket.loop();
    }
  }// frame refresh
}

uint8_t getNextCol(String &txt, uint32_t * len) {
  uint8_t col;
  uint32_t LIMIT = COLSIZE * 2;
  for (uint32_t i = 0; i < LIMIT ; i++) {
    uint16_t row = *(len) >> 3; // /8
    col = pgm_read_byte(&(ASCIIArray[(uint16_t)txt[row] - 32][*len % 8]));

    if (txt[row] != ' ') {
      if (currentInd != row) {
        if (col != 0) {
          --(*len);
          currentInd = row;
          return 0;
        } else {
          ++(*len);
        }
      } else {
        if (col == 0) {
          ++(*len);
        } else {
          return col;
        }
      }
    } else {
      return col;
    }
  }
  return 0;
}
void updateFrameWithSpeed(uint8_t * BUFFER) {
  timing.cScroll = micros();
  while (1) {
    if (settings.SPEED != 0) {
      limit.SEC_PER_COL = (uint32_t)((float)(1e6 / settings.SPEED));
    } else {
      limit.SEC_PER_COL = 1e7;
    }

    updateFrame(BUFFER);
    server.handleClient();
    webSocket.loop();
    wdt_reset();
    if (micros() - timing.cScroll > limit.SEC_PER_COL) {
      break;
    }
    if (controls.skip) {
      wdt_reset();
      return;
    }
  }
  wdt_reset();
}
void scroll(String &txt) {
  currentInd = 0;
  clearBuffer(BUFFER);
  if (controls.skip) {
    controls.skip = false;
    return;
  }

  uint32_t txtSize;
  if (controls.CURR_PRE == "t") {
    txtSize = txt.length() * 8;
  } else if (controls.CURR_PRE == "p") {
    txtSize = txt.length() / 2;
  } else {
    return;
  }


  for (uint32_t len = 0; len < txtSize; ++len) {

    for (uint16_t i = 0; i < COLSIZE - 1; ++i) {
      BUFFER[i] = BUFFER[i + 1];
    }


    if (controls.CURR_PRE == "t") {
      BUFFER[COLSIZE - 1] =  getNextCol(txt, &len);
    } else if (controls.CURR_PRE == "p") {
      uint8_t MSN = txt[2 * len] - '0';
      uint8_t LSN = txt[2 * len + 1] - '0';
      BUFFER[COLSIZE - 1] = (MSN << 4) | (LSN);
    }
    updateFrameWithSpeed(BUFFER);

    if (controls.skip) {
      controls.skip = false;
      return;
    }
  }
}

void ledoneForLetters(String &txt, uint32_t totalFrames){
 
  for (uint32_t i = 0; i < totalFrames; i++) {
    for (uint16_t j = 0; j < COLCHUNK; j++) {
      uint32_t letterIndex = (COLCHUNK * i) + j;

      for (uint8_t k = 0; k < 8; k++) {

        uint16_t bufferIndex = (8 * j) + k;
        BUFFER[bufferIndex] = pgm_read_byte(&(ASCIIArray[(uint16_t)txt[letterIndex] - 32][k]));
      }
      
    }
    if (totalFrames == 1) {
      updateFrame(BUFFER);
      return;
    }
    updateFrameWithSpeed(BUFFER);
    if (controls.skip) {
        controls.skip = false;
        return;
    }
  }
}
void ledoneForPixels(String &txt, uint32_t totalFrames){
 uint32_t pixelUnitSize = COLSIZE*2;
  for (uint32_t i = 0; i < totalFrames; i++) {
    for (uint16_t j = 0; j < COLSIZE; j++) {
      uint32_t txtInd = (i*pixelUnitSize) + (2*j);
      uint8_t val = getHexFromChars(txt[txtInd], txt[txtInd + 1]);
      BUFFER[j] = val;
      
    }
    if (totalFrames == 1) {
      updateFrame(BUFFER);
      return;
    }
    updateFrameWithSpeed(BUFFER);
    if (controls.skip) {
        controls.skip = false;
        return;
    }
  }
}
void ledone(String txt) {
  clearBuffer(BUFFER);
  uint32_t txtSize = txt.length();
  uint32_t pixelUnitSize = 2 * COLSIZE;
  Serial.println(txt);
  Serial.println(txtSize);


  if (controls.CURR_PRE == "t") {
    
    txtSize -= COLCHUNK;
    txt = txt.substring(0, txtSize);
    uint16_t extraBytes = txtSize % COLCHUNK;
    if (extraBytes) {
      uint16_t offset = COLCHUNK - extraBytes;
      txtSize += offset;
      
      for (uint16_t i = 0; i < offset; i++) {
        txt += " ";
      }

    }
    txtSize /= COLCHUNK;
    ledoneForLetters(txt, txtSize);
    return;
  }
  
  if(controls.CURR_PRE == "p"){
    txtSize -= pixelUnitSize;
    txt = txt.substring(0, txtSize);
    uint16_t extraNibbles = txtSize % pixelUnitSize;
    if(extraNibbles){
      uint16_t offset = pixelUnitSize - extraNibbles;
      txtSize += offset;
      
      for (uint16_t i = 0; i < offset; i++) {
        txt += "0";
      }

    }
    txtSize/=pixelUnitSize;
    Serial.println(txtSize);
    ledoneForPixels(txt, txtSize);
  }
}



void clearDisplay() {
  for (int8_t i = 0; i < NUM_DISP - 1; i++) {
    SPIBUFFER[i] = 0x00;
  }
  SPIBUFFER[NUM_DISP - 1] = 0xFF;
  mprint();
}
void makeSPIBuffer(uint32_t col, uint8_t row) {
  for (uint8_t i = 0; i < COLCHUNK; ++i) {
    SPIBUFFER[i] = ((col >> (8 * (COLCHUNK - 1 - i))) & COLMASK)^INVERTMASK;
  }
  SPIBUFFER[NUM_DISP - 1] = (row & COLMASK)^INVERTMASK;
}

void mprint() {
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  for (uint8_t i = 0; i < NUM_DISP; i++)
    SPI.transfer(SPIBUFFER[i]);
  latch();
  SPI.endTransaction();
}

void clearBuffer(uint8_t * FRAME) {
  for (uint16_t i = 0; i < COLSIZE; ++i) FRAME[i] = 0x0;
}
uint8_t pixToHex(char number) {
  if (number >= '0' && number <= '9') return number - '0';
  else if (number >= 'a' && number <= 'f') return number - 'a' + 0x0a;
  else if (number >= 'A' && number <= 'F') return number - 'A' + 0x0a;
  else return 0;
}

void socketHandle(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: {
        break;
      }
    case WStype_CONNECTED: {
        break;
      }
    case WStype_TEXT: {
        //do all stuff here
        switch (payload[0]) {
          case 'p': {
              String vals = String((char*)&payload[1]);
              uint16_t px = vals.substring(0, vals.indexOf(',')).toInt();
              uint8_t val = vals.substring(vals.indexOf(',') + 1).toInt();
              pix.BUFFER[px] = val;
              controls.pixelUpdate = true;
              break;
            }
          case 'c': {
              clearPixels();
              controls.pixelUpdate = true;
              break;
            }
          case 'i': {

              for (uint16_t i = 0; i < COLSIZE; i++) {
                pix.BUFFER[i] ^= 0xff;
              }
              controls.pixelUpdate = true;
              break;
            }
          case 'l': {

              if (pix.LEFT_LENGTH == 0) {
                pix.LEFT_LENGTH++;
                pix.LEFT = (uint8_t *)malloc(pix.LEFT_LENGTH);
              } else {
                pix.LEFT_LENGTH++;
                pix.LEFT = (uint8_t *)realloc(pix.LEFT, pix.LEFT_LENGTH);
              }
              pix.LEFT[pix.LEFT_LENGTH - 1] = pix.BUFFER[0];
              for (uint16_t i = 0; i < COLSIZE - 1; i++) {
                pix.BUFFER[i] = pix.BUFFER[i + 1];
              }
              if (pix.RIGHT_LENGTH == 0) {
                pix.BUFFER[COLSIZE - 1] = 0;
              } else {
                pix.RIGHT_LENGTH--;
                pix.BUFFER[COLSIZE - 1] = pix.RIGHT[pix.RIGHT_LENGTH];
              }
              controls.pixelUpdate = true;
              break;
            }
          case 'r': {

              if (pix.RIGHT_LENGTH == 0) {
                pix.RIGHT_LENGTH++;
                pix.RIGHT = (uint8_t *)malloc(pix.RIGHT_LENGTH);
              } else {
                pix.RIGHT_LENGTH++;
                pix.RIGHT = (uint8_t *)realloc(pix.RIGHT, pix.RIGHT_LENGTH);
              }
              pix.RIGHT[pix.RIGHT_LENGTH - 1] = pix.BUFFER[COLSIZE - 1];
              for (int32_t i = COLSIZE - 1; i > 0; i--) {
                pix.BUFFER[i] = pix.BUFFER[i - 1];
              }
              if (pix.LEFT_LENGTH == 0) {
                pix.BUFFER[0] = 0;
              } else {
                pix.LEFT_LENGTH--;
                pix.BUFFER[0] = pix.LEFT[pix.LEFT_LENGTH];
              }
              controls.pixelUpdate = true;
              break;
            }
          case 's': {
              savePixelsFS();
              clearPixels();
              controls.pixelUpdate = true;
            }
        }
      }
  }
}



void pixelArt() {
  updateFrame(pix.BUFFER);

  if ((micros() - timing.pixelSave > limit.PIXEL_SAVE) && controls.pixelUpdate) {
    savePixels();
    savePixelL();
    savePixelR();
    controls.pixelUpdate = false;
    timing.pixelSave = micros();
  }
  wdt_reset();

}
