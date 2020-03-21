/*************************************************** 
  This is an example for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>
#include <RCSwitch.h>
#include <SoftwareSerial.h>


// define the pins used
//#define CLK 11       // SPI Clock, shared with SD card
//#define MISO 10      // Input data, from VS1053/SD card
//#define MOSI 9      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  6      // VS1053 reset pin (output)
#define BREAKOUT_CS     7     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    5      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  //Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);


RCSwitch mySwitch = RCSwitch();
SoftwareSerial mySerial(-1, 8); // RX, TX

boolean bLED = false;
long lTime1=0;
int iNote;
#define LED 13
bool bSDPlayerReady=false;
    

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit VS1053");
  bSDPlayerReady=true;
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     //while (1);
     bSDPlayerReady=false;
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    //while (1);  // don't do anything more
    bSDPlayerReady=false;
  }

  if(bSDPlayerReady){ 

    
    // list files
    printDirectory(SD.open("/"), 0);
    
    // Set volume for left, right channels. lower numbers == louder volume!
    musicPlayer.setVolume(20,20);
  
    // Timer interrupts are not suggested, better to use DREQ interrupt!
    //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
  
    // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
    
    // Play one file, don't return until complete
    //  Serial.println(F("Playing track 001"));
    //musicPlayer.playFullFile("/smp001.mp3");
    // Play another file in the background, REQUIRES interrupts!
    //Serial.println(F("Playing track 002"));
    //musicPlayer.startPlayingFile("/t002.mp3");
  
    
    musicPlayer.startPlayingFile("/start.mp3");   
    
  }


  mySwitch.enableReceive(0);  // Empfänger ist an Interrupt-Pin "0" - Das ist am UNO der Pin2
  mySerial.begin(31250);
  pinMode(LED, OUTPUT); 

/*
   //----Test
    delay(1000);
    
    //for(int i=0; i<10; i++){
      
      //char *tmp = malloc(5);        
      //sprintf(tmp, "%03d", 13);        
      
      String sFilename = "/smp002.mp3";
      //sFilename.concat(tmp);
      //sFilename.concat(".mp3");
      char copy[11];
      sFilename.toCharArray(copy, 12);
      
      //Serial.print("*");Serial.print(copy);Serial.println("*");
      //musicPlayer.startPlayingFile( copy );
      playFile( copy );
      delay(200);
    //}
*/
}


bool playFile(String pFilename){
    
  bool bReturn = false;
  if(bSDPlayerReady){

    char copy[pFilename.length()];
    pFilename.toCharArray(copy, pFilename.length()+1);
        
    Serial.print("playFile:*");Serial.print(copy);Serial.println("*");
    musicPlayer.stopPlaying();
    musicPlayer.startPlayingFile( copy );
    
    //musicPlayer.startPlayingFile("/start.mp3");
    bReturn = true;
  }else{
    Serial.println("playFile(): SDPlayer not ready");
  }
  return bReturn;
}

void loop() {
  // File is playing in the background
  if (musicPlayer.stopped()) {
    //1Serial.println("Done playing music");
    //while (1) {
      delay(10);  // we're done! do nothing...
    //}
  }

/*  
  if (Serial.available()) {
    char c = Serial.read();
    
    // if we get an 's' on the serial console, stop!
    if (c == 's') {
      musicPlayer.stopPlaying();
    }
    
    // if we get an 'p' on the serial console, pause/unpause!
    if (c == 'p') {
      if (! musicPlayer.paused()) {
        Serial.println("Paused");
        musicPlayer.pausePlaying(true);
      } else { 
        Serial.println("Resumed");
        musicPlayer.pausePlaying(false);
      }
    }

    if (c == '1') {
      musicPlayer.stopPlaying();
      musicPlayer.startPlayingFile("/smp001.mp3");
    }

    if (c == '2') {
      musicPlayer.stopPlaying();
      musicPlayer.startPlayingFile("/smp002.mp3");
    }

    if (c == '3') {
      musicPlayer.stopPlaying();
      musicPlayer.startPlayingFile("/smp003.mp3");
    }

    if (c == '4') {
      musicPlayer.startPlayingFile("/smp004.mp3");
    }

    if (c == '5') {
      musicPlayer.startPlayingFile("/smp005.mp3");
    }

    if (c == '6') {
      musicPlayer.startPlayingFile("/smp006.mp3");
    }

    
  }

  delay(100);
*/  
  if (mySwitch.available()){
  
    if(lTime1+200<millis()){
      //Neuer Anschlag
      bLED = !bLED;
       digitalWrite(LED, true);
      
      lTime1=millis();
      iNote = mySwitch.getReceivedValue()-1;
      if(iNote<99){
        mySerial.write(144);
        mySerial.write(iNote);
        mySerial.write(127);   
        //musicPlayer.stopPlaying();
        //musicPlayer.startPlayingFile("/smp001.mp3");

        
        musicPlayer.stopPlaying();
        
        
        char *tmp = malloc(5);        
        sprintf(tmp, "%03d", iNote);        
        
        String sFilename = "/smp";
        sFilename.concat(tmp);
        sFilename.concat(".mp3");
        char copy[11];
        sFilename.toCharArray(copy, 12);
        
        //Serial.print("*");Serial.print(copy);Serial.println("*");
        //musicPlayer.startPlayingFile( copy );
        playFile( copy );

        
      }else{
        //----Das Poti fuer die Lautstaerke
        int i=0;
        mySerial.write(176); // MIdiCC auf Channel 1
        mySerial.write(i);
        mySerial.write(iNote-99);
        
        int iVol = iNote-99;
        iVol = map(iVol,0,127,100,2);
        musicPlayer.setVolume(iVol,iVol);
      }

      //Serial.println(iNote, DEC);
      digitalWrite(LED, false);
    }else{
      //----Wiederholung
      delay(50);
    }
    
    mySwitch.resetAvailable(); // Hier wird der Empfänger "resettet"
    
  }
}


/// File listing helper
void printDirectory(File dir, int numTabs) {
  
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }

     
     Serial.print(entry.name());
     
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }  
}
