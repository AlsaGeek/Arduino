#include <FastSPI_LED.h>
#include <SD.h>

#define NUM_LEDS 60

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char g; unsigned char r; unsigned char b; };
// struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

File root;

unsigned int sd_haut;
unsigned int sd_larg;

#define LED_PIN 4
#define SD_CS 10

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  FastSPI_LED.setLeds(NUM_LEDS);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_SM16716);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_HL1606);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_595);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2811);

  FastSPI_LED.setPin(LED_PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  
  memset(leds, 255, NUM_LEDS * 3);
  FastSPI_LED.show();
  delay(10);
  
  pinMode(SD_CS, OUTPUT);
  if (!SD.begin(SD_CS)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  File dataFile = SD.open("data.hex");
  // if the file is available, write to it:
  if (dataFile) {
    sd_haut = dataFile.read();
    sd_haut = (256*sd_haut)+dataFile.read();
    
    sd_larg = dataFile.read();
    sd_larg = (256*sd_larg)+dataFile.read();

  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening data.hex");
  } 

  Serial.print("File : haut=");
  Serial.print(sd_haut);
  Serial.print(" larg=");
  Serial.println(sd_larg);

  for( int x=0 ; x<sd_larg ; x++ ) {
    for( int y=0 ; y<min(sd_haut,NUM_LEDS) ; y++ ) {
      leds[y].r = 255-(dataFile.read());
      leds[y].g = 255-(dataFile.read());
      leds[y].b = 255-(dataFile.read());
    }
    
    FastSPI_LED.show();
    delay(10);
    Serial.println(x);

  }

  dataFile.close();
}

