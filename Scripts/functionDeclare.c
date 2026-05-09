// GUI related cap
void drawDesktop();
void drawAnimation();
void drawChar_THIN(int x, int y, unsigned char c, int color);
void drawChar(int x, int y, unsigned char c, int color);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void draw_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void draw_RectTRANS(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, uint16_t multer);
void drawMouse(int x_offset, int y_offset);
void drawButton(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid);
void drawbuttonPRO(uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid, uint8_t offXT, uint8_t offYT);
void drawString(char *text, uint16_t x, uint16_t y, uint16_t color);
void drawString1(char *text, uint16_t x, uint16_t y, uint16_t color);
void swapBuffers();

//cap
void wait(uint32_t seconds);
void loop();
void mouse_wait_data();
void init_mouse();

//kb/input
void updateMouse();
void getKey();
void handle_ps2_input();
void ManageTheKB();

// window system
void add_window(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, char *title);
void drawAllWindows();
void handleWindowDragging();
void bring_to_front(int idx);
void closeTopWindow();

// draw to window
void drawCharToWindow(uint8_t id, const uint8_t glyph[8], uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindowTitle(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindow(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawStringToWindowBIG(uint8_t id, char *text, uint16_t x, uint16_t y, uint16_t color);
void drawPixelToWindow(uint8_t id, uint16_t x, uint16_t y, uint16_t color);
void drawRectToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color);
void drawButtonToWindow(uint8_t id, uint16_t t, uint16_t l, uint16_t w, uint16_t h, uint16_t color, char *text, uint16_t Buttid);

// console
void ClearConsole();
void ConsoleLoop();
void ConsoleNewLine();
void printToConsole(int len, char *text);
void clearInputBuf();
void openConsole();

// random cap
void updateRTC();
void WelcomeLoop();
void FamilyMart();
void beep();
void thewayyouunderstandme();
void PianoLoop();
static void play_sound(uint32_t nFrequence);
static void nosound();

// disk stuff
uint8_t disk_readSector(uint32_t target_address, uint32_t LBA);
uint8_t disk_writeSector(uint16_t *ptr,uint32_t LBA);
uint8_t disk_detectIDE();
void jiouFS_readFileTo(uint32_t LBAFO, uint32_t* address, uint32_t lenB);
void jiouFS_readFile(int len, char *targetName, uint32_t* address);
void jiouFS_list();

