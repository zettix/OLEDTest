// OLED Performance Test
// Because why not.  Sean Brennan, May 2015
#include "U8glib.h" // https://code.google.com/p/u8glib/ for display info.

U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9 , 8);
// I2C: This is the _ONLY_ change:
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); 

int xpos, ypos;
int rcount = 0;
int delta_time = 0;

char label[100];
uint8_t bitmap[1024]; 
// Dimensions of display:
int xres = 128;
int yres = 64;

void clearbuf(void) {
  int i;
  for (i = 0; i < 1024; i++) {
    bitmap[i] = 0;
  }
  xpos = 0;
  ypos = 0;
}

void plot(int x, int y) {
    int bitno, bitplace, bitoff;
    bitno = y * 128 + x;
    bitplace = bitno / 8;
    bitoff = bitno % 8;
    bitmap[bitplace] = bitSet(bitmap[bitplace], 7 - bitoff);
}

void setup(void) {
    int i;
    Serial.begin(9600);
    u8g.setColorIndex(1);         // pixel on
    xpos = 0;
    ypos = 0;
    clearbuf();
    u8g.setFont(u8g_font_osr18);
    pinMode(A3, INPUT);
}

void draw1(void) {
  // graphic commands to redraw the complete screen should be placed here  
  int t = 0;
  int done = 0;
  int perf_now;
  if (((xpos * xpos + ypos * ypos ) % (rcount % 387)) == 1 ) {
    //plot(xpos, ypos);
  }
  xpos += 1; ypos = yres + 1;
  if (xpos >= xres) {
    xpos = 0;
    ypos += 1;
  }
  if (ypos >= yres) {
     // Image drawn, performance area.
     perf_now = millis();
     u8g.firstPage();  
     do {
       //u8g.drawBitmap(0, 0, 16, 64, bitmap);
       u8g.drawStr(0, yres - rcount % 30, label);
     } while( u8g.nextPage() ); 
     // rebuild the picture after some delay
     delta_time = millis() - perf_now;
     delay(50); 
     ypos = 0;
     rcount = analogRead(A3);
     sprintf(label, "%03d %04d", delta_time, rcount % 1200);
     Serial.println(label);
     clearbuf();
  }
}

void loop(void) {
    draw1();
}
