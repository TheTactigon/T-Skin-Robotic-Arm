
#include <Braccio.h>
#include <Servo.h>

#define BUFF_LENGTH 32

#define pump  2
#define valve 4

//Servo Declarations
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

//String for received data buffer
String inBuff = "";

int gFirstTime;

int ledPin = 13;   //led 3
int btnPin = 8;    //button 1





//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(pump, OUTPUT);
  pinMode(valve, OUTPUT);

  //other init
  gFirstTime = 0;
  Serial.begin(9600);

  //init pin
  pinMode(btnPin, INPUT);           //button
  pinMode(ledPin, OUTPUT);          //led

  
    
  
  //For each step motor this set up the initial degree
  Braccio.begin();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{

  //first time clear rx serial buffer
  if(gFirstTime == 0)
  {
    int avlb, bb, i;
    
    gFirstTime = 1;
    avlb = Serial.available();
    for(i=0; i<avlb; i++)
       bb = Serial.read();
  }


  //loop on serial
  while (Serial.available()) 
  {
    char c;
    //Fills buffer
    c = char(Serial.read());
     

    if(inBuff.length() > 31)
      inBuff = "";
      
    //end of message, start parsing
    else if(c == '\n')
    {
      int a1, a2, a3, yaw, relYaw_3, cks;
      int match;
      int a_shoulder, a_elbow, a_rot, a_wrist, a_nipper;
      char buff[BUFF_LENGTH];


      //sets buff elements to 0
      memset(buff, 0, sizeof(buff));
      //populate buff with inBuff elements
      inBuff.toCharArray(buff, sizeof(buff));
      //sscanf to get parameters from T-Skin's message
      match = sscanf(buff, "%d,%d,%d,%d,%d %d", &a1, &a2, &a3, &relYaw_3, &yaw, &cks);
      if(match == 6)      
      {
        if(1)
        { 
  		    a_rot = yaw;          //data sent is already processed from T-Skin:  -yaw + 90;		
          a_shoulder = -a1;
          a_elbow = 90 - a2; 
          a_wrist = 90 - a3;        
          a_nipper = -relYaw_3 + 90;
                     
          //Checks if limits are reached
  		    angle_Limits(&a_shoulder, &a_elbow, &a_wrist, &a_nipper);		                         
          angle_combinedCheck(&a_shoulder, &a_elbow);
          //Move the arm
  			  Braccio.ServoMovement(20, a_rot, a_shoulder, a_elbow, a_wrist, a_nipper, 90);  
        }             
      }
      inBuff = "";
    }
    else
      inBuff += c;
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void angle_Limits(int *l_shoulder, int *l_elbow, int *l_wrist, int *l_nipper)
{
//  45 < a_shoulder < 125
//  65 < a_elbow    < 140
//   0 < a_wrist    < 165
//  30 < a_nipper   < 120
//
//  a_shoulder + a_elbow > 50 + 65

	if(*l_shoulder < 45)
	  *l_shoulder = 45;
	if(*l_shoulder > 110)
	  *l_shoulder = 110;

	if(*l_elbow < 65)
	  *l_elbow = 65;
	if(*l_elbow > 150)
	  *l_elbow = 150;
  
	if(*l_wrist < 30)
	  *l_wrist = 30;
	if(*l_wrist > 135)
	  *l_wrist = 135;
  
	if(*l_nipper < (90-40))      //centered on 90
	  *l_nipper = (90-40);
	if(*l_nipper > (90+40))
	  *l_nipper = (90+40);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void angle_combinedCheck(int *p_shoulder, int *p_elbow)
{
	if(*p_shoulder + *p_elbow > (50 + 65))
  {    
  }
  else
  {
    *p_shoulder = 50;
    *p_elbow = 65; 
  }
  
}





