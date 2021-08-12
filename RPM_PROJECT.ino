/*measure the flywheel speed of a spark-ignition engine 
 *by measuring pulses per unit time in secondary coil of the step up transformer (also added shift indicator) 
 *refer to the diagrams for connections.
 *Change some paramters according the vehicle's engine specs.
*/

#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

#define PIN         10       //Input for Neopixels
#define NUMPIXELS   4        //Number of neo pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  //Creating object to control Neopixels

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;                         //Defining required pins for LCD display
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);                                           //Creating object to control display

#define SIGNAL_PIN 6         //////////Pin to capture Signals from ignition coil////////////

int signal_count=0,previous_boxes=0,boxes=0,i,flag=0;
bool previous_signal,current_signal; 
int old_rate=0,new_rate=0;
long int t2=millis(),t1=1;
int del=5;
char box=char(255);


void setup() 

 {     
 
   pinMode(SIGNAL_PIN,INPUT);  
   pixels.begin();
   lcd.begin(16, 2);
   lcd.setCursor(0,1);
   lcd.print("hello, world!");
   digitalWrite(SIGNAL_PIN,HIGH);
   for(int i=0;i<16;i++)
    {
      lcd.setCursor(i,0);
      lcd.print(box);
      if(i/4==2||i/4==3)
        pixels.setPixelColor(i/4, pixels.Color(100,0,0));
      else
        pixels.setPixelColor(i/4, pixels.Color(0,100,0));
      pixels.show();  
      delay(200-i*10); 
    } 
   delay(300); 
   for(int i=15;i>=0;i--)
   {
    lcd.setCursor(i,0);
    lcd.print(" ");
     if(i/4==2||i/4==3)
        pixels.setPixelColor(i/4, pixels.Color(0,0,0));
      else
        pixels.setPixelColor(i/4, pixels.Color(0,0,0)); 
     pixels.show();    
     delay(50+i*10);   
   }
   lcd.clear();
   pixels.show();
   
 }

void loop() 
 {   
     previous_signal=digitalRead(SIGNAL_PIN);
     delay(del);
     current_signal=digitalRead(SIGNAL_PIN);
     if(previous_signal!=current_signal)
      { 
        if(++signal_count==28)
         {  
           t2=millis();
           new_rate=(float)8/(t2-t1)*1000*60;
           signal_count=0;
           if(old_rate!=new_rate)
             {        
              lcd.setCursor(0,1);
              if(new_rate>2000)
                 del=2;
              else if(new_rate<1000)
               {
                 lcd.print(" ");
                 del=5;
               }
              else
                del=5; 
              lcd.print(new_rate);               
              boxes=new_rate/400;    
              switch(boxes)
              {
               case 16 :  lcd.setCursor(15,0);
                          lcd.print(box);
                          break;                      
               case 15 :  lcd.setCursor(14,0);
                          lcd.print(box); 
                          break;                                                
               case 14 :  lcd.setCursor(13,0);
                          lcd.print(box); 
                          break;                
               case 13 :  lcd.setCursor(12,0);
                          lcd.print(box);
                          flag=1;
                          break;                                      
               case 12 :  lcd.setCursor(11,0);
                          lcd.print(box);
                          flag=1;
                          break;        
               case 11 :  lcd.setCursor(10,0);
                          lcd.print(box);
                          flag=1;
                          break;
               case 10 :  lcd.setCursor(9,0);
                          lcd.print(box); 
               case 9 :   lcd.setCursor(8,0);
                          lcd.print(box);
                          pixels.setPixelColor(3, pixels.Color(100,0,0));   
               case 8 :   lcd.setCursor(7,0);
                          lcd.print(box); 
                          pixels.setPixelColor(2, pixels.Color(100,0,0));            
               case 7 :   lcd.setCursor(6,0);
                          lcd.print(box); 
                          pixels.setPixelColor(1, pixels.Color(0,100,0)); 
               case 6 :   lcd.setCursor(5,0);
                          lcd.print(box);
                          pixels.setPixelColor(0, pixels.Color(0,100,0));          
               case 5 :   lcd.setCursor(4,0);
                          lcd.print(box);   
               case 4 :   lcd.setCursor(3,0);
                          lcd.print(box);                                                                                                                                     
               case 3 :   lcd.setCursor(2,0);
                          lcd.print(box); 
               case 2 :   lcd.setCursor(1,0);
                          lcd.print(box);
               case 1 :   lcd.setCursor(0,0);
                          lcd.print(box); 
                          flag=0;
                          pixels.show();                    
             }           
             if(previous_boxes-boxes>0)
               {
                 for(i=previous_boxes;i>=boxes;i--)                           
                { 
                 lcd.setCursor(i,0);                 
                 lcd.print(" ");    
                }
                if(flag!=1)
                 pixels.clear(); 
              }                                                   
            }    
          }
        else if (signal_count==1)
          {
             t1=millis();
             old_rate=new_rate;      //Updating old_rate
             previous_boxes=boxes;   //Updating previous_boxes        
             
          }

     }

}
