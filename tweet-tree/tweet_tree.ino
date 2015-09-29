#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
// Ethernet Shield header here

// Ethernet Shield network configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE1};
IPAddress ip(192,168,1,53);

EthernetServer server(80);

//Start the clock
unsigned long time;

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      26

#define PIXELSPERGROUP    4
#define LEADPIXELS      2

#define EXPIRETIME  30000  //300000 is five minutes

//make pixel groups - GROUPS being (NUMPIXELS - LEADPIXELS) / PIXELSPERGROUP
const int GROUPS = (NUMPIXELS - LEADPIXELS)/PIXELSPERGROUP;
int PIXELGROUP[GROUPS][PIXELSPERGROUP];

//keep track of group status. Group = GroupID, red, green, blue, timer
long GROUPSTATUS[GROUPS][4];  

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

int preset = 1;

String buffer;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  // populate pixelgroups with pixels
  int pixelcount = 0;
  int groupcount = 0;
  int currentpixel = (0+LEADPIXELS);
  for(currentpixel; currentpixel < NUMPIXELS; currentpixel++){
      if (pixelcount == PIXELSPERGROUP)
      {
        groupcount++;
        pixelcount = 0;
      }
      PIXELGROUP[groupcount][pixelcount] = currentpixel;
      pixelcount++;
  }

// set pixel group initial data
  groupcount = 0;
  for(groupcount; groupcount < GROUPS; groupcount++){
    GROUPSTATUS[groupcount][0] = 0;
    GROUPSTATUS[groupcount][1] = 0;
    GROUPSTATUS[groupcount][2] = 0;
    GROUPSTATUS[groupcount][3] = time;
  }
  
  Serial.begin(9600);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
 
  pixels.show();
  
}


void loop() {
  time = millis();
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    buffer = "";
    //Expect "GET /tweettree/".
    //Read characters into buffer until len == 13.
    while (client.available() && buffer.length() <15) {
      buffer = buffer + char(client.read());
    }
    //Error check that request starts with "GET /tweettree/" and 
    //that the client hasn't dropped.
    if (buffer != "GET /tweettree/" || !client.available()) {
      //return error
      client.stop();
      return;
    }
    //Reset buffer to store next in URL request
    //expecting format "?red=123&green=123&blue=123&group=001" 35char
    buffer = "";
    while (client.available() && buffer.length() <37) {
      buffer = buffer + char(client.read());
    }
    //Parse buffer for color values we want
    String search_array[] = {"red=", "green=", "blue=", "group="};
    String group_st;
    String red_st;
    String green_st;
    String blue_st;
    
    size_t position;
    //Positional-fu for string parsing   
    position = buffer.indexOf(search_array[0]);
    Serial.println(position);
    position = position + 4;
    Serial.println(position);
    red_st = buffer.substring(position, position + 3);
    Serial.println(red_st);
    position = buffer.indexOf(search_array[1]);
    position = position + 6;
    green_st = buffer.substring(position, position + 3);
    position = buffer.indexOf(search_array[2]);
    position = position + 5;
    blue_st = buffer.substring(position, position + 3);
    position = buffer.indexOf(search_array[3]);
    position = position + 6;
    Serial.println(position);
    group_st = buffer.substring(position, position + 3);
    
    int red = red_st.toInt();
    int green = green_st.toInt();
    int blue = blue_st.toInt();
    int group = group_st.toInt();
    turnup(group, red, green, blue);
    //ENDED HERE. 
    
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
      //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin

          client.print("Red: ");
          client.print(red_st);
          client.println("<br />");
          client.print("Green: ");
          client.print(green_st);
          client.println("<br />");
          client.print("Blue: ");
          client.print(blue_st);
          client.println("<br />");
          client.print("Group: ");
          client.print(group_st);
          client.println("<br />");
              
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

int timecheck(void){
   int groupcount = 0;
   for(groupcount; groupcount < GROUPS; groupcount++){
     
     long age = time - GROUPSTATUS[groupcount][3];
     if(age>EXPIRETIME){
       turndown(groupcount);
     }
   }
}


void turnup(int group, int newred, int newgreen, int newblue){
 turndown(group);
 int red = GROUPSTATUS[group][0];
 int green = GROUPSTATUS[group][1];
 int blue = GROUPSTATUS[group][2];

     Serial.print("Group:");
     Serial.println(group);
     Serial.print("Red:");
     Serial.println(red);
     Serial.print("Green:");
     Serial.println(green);
     Serial.print("Blue:");
     Serial.println(blue); 
 if(red != newred || green != newgreen || blue != newblue){
   while (red != newred || green != newgreen || blue != newblue){
     if(red>newred){
         red--;
     }
     if(red<newred){
      red++;
     } 
     if(green>newgreen){
       green--;
      }
     if(green<newgreen){
        green++;
      }
     if(blue>newblue){
        blue--;
      }
      if(blue<newblue){
        blue++;
      }

     for(int pixel=0; pixel<PIXELSPERGROUP; pixel++){

       pixels.setPixelColor(PIXELGROUP[group][pixel], pixels.Color(red, green, blue));
       pixels.show();
       }

   }
/* Serial.println("done. values to be saved.");
 Serial.println(group);
 Serial.print("Red:");
 Serial.println(red);
 Serial.print("Green:");
 Serial.println(green);
 Serial.print("Blue:");
 Serial.println(blue);*/
 GROUPSTATUS[group][0] = red;
 GROUPSTATUS[group][1] = green;
 GROUPSTATUS[group][2] = blue;
 GROUPSTATUS[group][3] = time;
 Serial.print("Turned up:");
 Serial.print(group);
 Serial.print(", ");
 Serial.println(time);
 }
 
}

void turndown(int group){
 //FOR WHAT?!
 //turn down all values by one until all black
 int red = GROUPSTATUS[group][0];
 int green = GROUPSTATUS[group][1];
 int blue = GROUPSTATUS[group][2];
 /*Serial.print("Group:");
 Serial.println(group);
 Serial.print("Red:");
 Serial.println(red);
 Serial.print("Green:");
 Serial.println(green);
 Serial.print("Blue:");
 Serial.println(blue);*/
 
 while (red>0 || green>0 || blue>0){
   if(red>0){
     red--;
   }
    if(green>0){
      green--;
    }
    if(blue>0){
      blue--;
    }
    
    for(int pixel=0; pixel<PIXELSPERGROUP; pixel++){
 /*      Serial.println("Blue is greater than zero");
      Serial.print(red);
      Serial.print(", ");
      Serial.print(green);
      Serial.print(", ");
     Serial.println(blue); */
       pixels.setPixelColor(PIXELGROUP[group][pixel], pixels.Color(red, green, blue));
       pixels.show();
     }
    }
   
   GROUPSTATUS[group][0] = 0;
   GROUPSTATUS[group][1] = 0;
   GROUPSTATUS[group][2] = 0;
   GROUPSTATUS[group][3] = 0;
 Serial.print("Turned down:");
 Serial.print(group);
 Serial.print(", ");
 Serial.println(time);
 }
 

void do_preset(void){
  Serial.println(preset);
  if(preset!=0){
    turnup(1, 255, 100, 150);
    turnup(2, 255, 100, 150);
    turnup(2, 255, 100, 150);
    turnup(3, 255, 100, 150);
    turnup(4, 255, 100, 150);
    turnup(5, 255, 100, 150);
    preset = 0;
  }
}
