/*
 * 2 buttons are used for DigitalIns
 * the first and second switches patterns
 */

#include "mbed.h"
#include "NeoStrip.h"
#include "gt.h"
#include "string"
#include "stdio.h"
#include "MAX9814.h"
Serial pc (USBTX, USBRX);

#define N 144
#define PATTERNS 7



int hueToRGB(float h);
void pattern0();
void pattern1();
void pattern2();
void pattern3();
void pattern4();
void pattern5();
void pattern6();

// array of function pointers to the various patterns
void (*patterns[])(void) = {&pattern0, &pattern1, &pattern2, &pattern3, &pattern4, &pattern5, &pattern6};
int gt_img3[144] = {};
int gt_img4[144] = {};
float sound_vol;
int how_loud = 0;

NeoStrip strip(p18, N);
DigitalIn b1(p25); // prev pattern
DigitalIn b2(p24); // next pattern

MAX9814 mic(p20); //microphone

// timer used for debugging
Timer timer;

//this funcrion writes pattern 3 based on how loud it is
//pattern 3 is a double amplification representation
//based on how loud it is light up between 1 and 7 lights
//more loud = all lights on
//not that loud theres 1 light on
void write_three(int n){
  //n measures loudness
  if(n==1){
    for(int i = 0; i<144; i++){
      gt_img3[i] = write_three_one[i%12]; 
    } 
  }
  if(n==2){
    for(int i = 0; i<144; i++){
      gt_img3[i] = write_three_two[i%12]; 
    }
  }
  if(n==3){
    for(int i = 0; i<144; i++){
      gt_img3[i] = write_three_three[i%12]; 
    }   
  } 
  if(n==4){
    for(int i = 0; i<144; i++){
      gt_img3[i] = write_three_four[i%12]; 
    }   
  } 
}
//this funcrion writes pattern 4 based on how loud it is
//pattern 4 is a amplification representation
//based on how loud it is light up between 1 and 4 lights
//more loud = all lights on
//not that loud theres 1 light on
void write_four(int n){
  //n measures loudness
  if(n==1){
    for(int i = 0; i<144; i++){
      gt_img4[i] = write_four_one[i%24]; 
    } 
  }
  if(n==2){
    for(int i = 0; i<144; i++){
      gt_img4[i] = write_four_two[i%24]; 
    }
  }
  if(n==3){
    for(int i = 0; i<144; i++){
      gt_img4[i] = write_four_three[i%24]; 
    }   
  } 
  if(n==4){
    for(int i = 0; i<144; i++){
      gt_img4[i] = write_four_four[i%24]; 
    }   
  } 
}

int main()
{
  //this is where push buttons previously used pullDown which worked fine
  b1.mode(PullDown);
  b2.mode(PullDown);

  
  int pattern = 0;
  float bright = 0.2; // 20% is plenty for indoor use
  bool b1o = b1;    // old copy of button 1 to poll for changes
  bool b2o = b2;    // old copy of button 2 to poll for changes

  strip.setBrightness(bright);  // set default brightness
  
  while (true)
  {

    timer.reset(); // use a timer to measure loop execution time for debugging purposes
    timer.start(); // for this application, the main loop takes approximately 3ms to run

    
    sound_vol = mic.sound_level();
    //using the mic library I measured loudness
    //I declared 5 levels of how loud it is
    //the levels go from 0 being quiet throuh 4 being loud
    //variable how loud measures how loud it is
    //how loud variable is then used to draw the patterns
    if(sound_vol < 0.05){
      how_loud = 0;
      //how loud the strip is effects how bright the strip is
      //inc in loudness results in inc in brightness
      bright = 0.2;
      strip.setBrightness(bright);  
    }
    if(0.05<sound_vol && sound_vol< 0.5){
      how_loud = 1;
      write_three(1);
      write_four(1);
      //how loud the strip is effects how bright the strip is
      //inc in loudness results in inc in brightness
      bright = 0.4;
      strip.setBrightness(bright);  
    }
    if(0.5<sound_vol && sound_vol< 1.0){
      how_loud = 2;
      write_three(2);
      write_four(2);
      //how loud the strip is effects how bright the strip is
      //inc in loudness results in inc in brightness
      bright = 0.6;
      strip.setBrightness(bright);  
    } 
    if(1.0<sound_vol && sound_vol< 1.2){
      how_loud = 3;
      write_three(3);
      write_four(3);
      //how loud the strip is effects how bright the strip is
      //inc in loudness results in inc in brightness
      bright = 0.8;
      strip.setBrightness(bright);  
    }
    if(1.2<sound_vol && sound_vol< 4.0){
      how_loud = 4;
      write_three(4);
      write_four(4);
      //how loud the strip is effects how bright the strip is
      //inc in loudness results in inc in brightness
      bright = 1.0;
      strip.setBrightness(bright);  
    }   
      
    
    // button 2 changes to the next pattern, only do stuff when its state has changed
    if (b2 != b2o)
    {
      //increment patterns if equals last pattern reset to 0
      if (b2 && ++pattern == PATTERNS)
        pattern = 0;
      //use this function to preserve state
      b2o = b2;
    }
    // button 1 changes to the prev pattern, only do stuff when its state has changed
    if (b1 != b1o)
    {
      //decrement patterns if equals first pattern then switch to last pattern
      if (b1 && --pattern == -1)
        pattern = (PATTERNS-1);
      //use this function to preserve state
      b1o = b1;
    }
    
 
    
    // run the pattern update function which sets the strip's pixels
    patterns[pattern]();
    strip.write();

    timer.stop();
    
    
    wait_ms(10);
  }
}

// pattern0 displays a static blue and yellow image
void pattern0()
{
  strip.setPixels(0, N, gt_img);
}

// display a shifting rainbow, all colors have maximum
// saturation and value, with evenly spaced hue
//this pattern was taken from Allen Wild
//https://os.mbed.com/users/aswild/code/NeoPixels/file/f38492690f0e/main.cpp/
void pattern1()
{
  static float dh = 360.0 / N;
  static float x = 0;

  for (int i = 0; i < N; i++)
    strip.setPixel(i, hueToRGB((dh * i) - x));
  
  x += 1;
  if (x > 360)
    x = 0;
}

// display a shifting gradient between red and blue
//this pattern was taken from Allen Wild
//https://os.mbed.com/users/aswild/code/NeoPixels/file/f38492690f0e/main.cpp/
void pattern2()
{
  // offset for each pixel to allow the pattern to move
  static float x = 0;

  float r, b, y;

  for (int i = 0; i < N; i++)
  {
    // y is a scaled position between 0 (red) and 1.0 (blue)
    y = 1.0 * i / (N - 1) + x;
    if (y > 1)
      y -= 1;

    // if on the left half, red is decreasing and blue is increasng
    if (y < 0.5)
    {
      b = 2 * y;
      r = 1 - b;
    }

    // else red is increasing and blue is decreasing
    else
    {
      r = 2 * (y - 0.5);
      b = 1 - r;
    }

    // scale to integers and set the pixel
    strip.setPixel(i, (uint8_t)(r * 255), 0, (uint8_t)(b * 200));
  }

  x += 0.003;
  if (x > 1)
    x = 0;
}
//princess bubblegum
void pattern3()
{
  strip.setPixels(0, N, gt_img2);
}

//amplitude
void pattern4()
{
  strip.setPixels(0, N, gt_img3);
}

//double amplitude
void pattern5()
{
  strip.setPixels(0, N, gt_img4);
}

//ant crawl
//if its loud makes ants crawl faster by lowering the wait time
//if its not that loud the ants crawl slow hightening the wait time
void pattern6()
{
  
  for (int i = 0;i < 12; i++){
    strip.setPixels(0, N, clear);
    strip.write();
    wait_ms(2);
    if(i != 0){
      strip.setPixel(i, rainbow[i%12]);
      strip.setPixel(i-1, rainbow[i%12]);
      
      strip.setPixel(12+i, rainbow[i]);
      strip.setPixel((12+i)-1, rainbow[i]);
      
      strip.setPixel(24+i, rainbow[i]);
      strip.setPixel((24+i)-1, rainbow[i]);
      
      strip.setPixel(36+i, rainbow[i]);
      strip.setPixel((36+i)-1, rainbow[i]);
      
      strip.setPixel(48+i, rainbow[i]);
      strip.setPixel((48+i)-1, rainbow[i]);
      
      strip.setPixel(60+i, rainbow[i]);
      strip.setPixel((60+i)-1, rainbow[i]);
      
      strip.setPixel(72+i, rainbow[i]);
      strip.setPixel((72+i)-1, rainbow[i]);
      
      strip.setPixel(84+i, rainbow[i]);
      strip.setPixel((84+i)-1, rainbow[i]);
      
      strip.setPixel(96+i, rainbow[i]);
      strip.setPixel((96+i)-1, rainbow[i]);
      
      strip.setPixel(108+i, rainbow[i]);
      strip.setPixel((108+i)-1, rainbow[i]);
      
      strip.setPixel(120+i, rainbow[i]);
      strip.setPixel((120+i)-1, rainbow[i]);
      
      strip.setPixel(132+i, rainbow[i]);
      strip.setPixel((132+i)-1, rainbow[i]);
    }
    else{
      strip.setPixel(i, rainbow[i%12]);
      strip.setPixel(11, rainbow[i%12]);
      
      strip.setPixel(12+i, rainbow[i]);
      strip.setPixel(23, rainbow[i]);
      
      strip.setPixel(24+i, rainbow[i]);
      strip.setPixel(35, rainbow[i]);
      
      strip.setPixel(36+i, rainbow[i]);
      strip.setPixel(47, rainbow[i]);
      
      strip.setPixel(48+i, rainbow[i]);
      strip.setPixel(59, rainbow[i]);
      
      strip.setPixel(60+i, rainbow[i]);
      strip.setPixel(71, rainbow[i]);
      
      strip.setPixel(72+i, rainbow[i]);
      strip.setPixel(83, rainbow[i]);
      
      strip.setPixel(84+i, rainbow[i]);
      strip.setPixel(95, rainbow[i]);
      
      strip.setPixel(96+i, rainbow[i]);
      strip.setPixel(107, rainbow[i]);
      
      strip.setPixel(108+i, rainbow[i]);
      strip.setPixel(119, rainbow[i]);
      
      strip.setPixel(120+i, rainbow[i]);
      strip.setPixel(131, rainbow[i]);
      
      strip.setPixel(132+i, rainbow[i]);
      
      
    }
    
    strip.write();
    if(i != 11){
      if(how_loud == 0){
        wait_ms(280);
      }
      if(how_loud == 1){
        wait_ms(220);
      }
      if(how_loud == 2){
        wait_ms(160);
      }
      if(how_loud == 3){
        wait_ms(100);
      }
      if(how_loud == 4){
        wait_ms(50);
      }
      
    }
    else{
      if(i != 11){
        if(how_loud == 0){
          wait_ms(245);
        }
        if(how_loud == 1){
          wait_ms(185);
        }
        if(how_loud == 2){
          wait_ms(125);
        }
        if(how_loud == 3){
          wait_ms(65);
        }
        if(how_loud == 4){
          wait_ms(15);
        }
      }
    }
  }

//this function was taken from Allen Wild
//https://os.mbed.com/users/aswild/code/NeoPixels/file/f38492690f0e/main.cpp/
// Converts HSV to RGB with the given hue, assuming
// maximum saturation and value
int hueToRGB(float h)
{
  // lots of floating point magic from the internet and scratching my head
  float r, g, b;
  if (h > 360)
    h -= 360;
  if (h < 0)
    h += 360;
  int i = (int)(h / 60.0);
  float f = (h / 60.0) - i;
  float q = 1 - f;
  
  switch (i % 6)
  {
    case 0: r = 1; g = f; b = 0; break;
    case 1: r = q; g = 1; b = 0; break;
    case 2: r = 0; g = 1; b = f; break;
    case 3: r = 0; g = q; b = 1; break;
    case 4: r = f; g = 0; b = 1; break;
    case 5: r = 1; g = 0; b = q; break;
    default: r = 0; g = 0; b = 0; break;
  }
  
  // scale to integers and return the packed value
  uint8_t R = (uint8_t)(r * 255);
  uint8_t G = (uint8_t)(g * 255);
  uint8_t B = (uint8_t)(b * 255);

  return (R << 16) | (G << 8) | B;
}

