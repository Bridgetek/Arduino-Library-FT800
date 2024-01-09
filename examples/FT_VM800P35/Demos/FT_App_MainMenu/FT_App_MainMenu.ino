/*****************************************************************************
* Copyright (c) Future Technology Devices International 2014
* propriety of Future Technology devices International.
*
* Software License Agreement
*
* This code is provided as an example only and is not guaranteed by FTDI. 
* FTDI accept no responsibility for any issues resulting from its use. 
* The developer of the final application incorporating any parts of this 
* sample project is responsible for ensuring its safe and correct operation 
* and for any consequences resulting from its use.
*****************************************************************************/
/**
* @file                           FT_App_MainMenu.ino
* @brief                          Sample application to demonstrat FT800 primitives, widgets and customized screen shots. 
* @version                        0.1.0
* @date                           2014/03/21
*
*/




#include "SPI.h"
#include "Wire.h"
#include "FT_VM800P35.h"

/* Macro for different types of scrolling methods. These are Mutually exclusive macros.*/
//#define WIN8_METHOD
//#define ANDROID_METHOD
#define LOOPBACK_METHOD






#define MAX_MENUS 12
#define THUMBNAIL_ADDRESS (125*1024L)
#define MENU_POINTSIZE  5 // 16bit prec 



/* SD card object*/
FT_SD FtSd(FT_SD_CSPIN);

/* sd_present holds error values during initialization.  0 means no error and all other errors are non zero value */
FT_SDStatus sd_present; 

/* Global object for transport */
FT800IMPL_SPI FTImpl(FT_CS_PIN,FT_PDN_PIN,FT_INT_PIN);


/*****************************************************************************/
/* Example code to display few points at various offsets with various colors */

// deflated  icon
PROGMEM prog_uint8_t home_star_icon[] = {0x78,0x9C,0xE5,0x94,0xBF,0x4E,0xC2,0x40,0x1C,0xC7,0x7F,0x2D,0x04,0x8B,0x20,0x45,0x76,0x14,0x67,0xA3,0xF1,0x0D,0x64,0x75,0xD2,0xD5,0x09,0x27,0x17,0x13,0xE1,0x0D,0xE4,0x0D,0x78,0x04,0x98,0x5D,0x30,0x26,0x0E,0x4A,0xA2,0x3E,0x82,0x0E,0x8E,0x82,0xC1,0x38,0x62,0x51,0x0C,0x0A,0x42,0x7F,0xDE,0xB5,0x77,0xB4,0x77,0x17,0x28,0x21,0x26,0x46,0xFD,0x26,0xCD,0xE5,0xD3,0x7C,0xFB,0xBB,0xFB,0xFD,0xB9,0x02,0xCC,0xA4,0xE8,0x99,0x80,0x61,0xC4,0x8A,0x9F,0xCB,0x6F,0x31,0x3B,0xE3,0x61,0x7A,0x98,0x84,0x7C,0x37,0xF6,0xFC,0xC8,0xDD,0x45,0x00,0xDD,0xBA,0xC4,0x77,0xE6,0xEE,0x40,0xEC,0x0E,0xE6,0x91,0xF1,0xD2,0x00,0x42,0x34,0x5E,0xCE,0xE5,0x08,0x16,0xA0,0x84,0x68,0x67,0xB4,0x86,0xC3,0xD5,0x26,0x2C,0x20,0x51,0x17,0xA2,0xB8,0x03,0xB0,0xFE,0x49,0xDD,0x54,0x15,0xD8,0xEE,0x73,0x37,0x95,0x9D,0xD4,0x1A,0xB7,0xA5,0x26,0xC4,0x91,0xA9,0x0B,0x06,0xEE,0x72,0xB7,0xFB,0xC5,0x16,0x80,0xE9,0xF1,0x07,0x8D,0x3F,0x15,0x5F,0x1C,0x0B,0xFC,0x0A,0x90,0xF0,0xF3,0x09,0xA9,0x90,0xC4,0xC6,0x37,0xB0,0x93,0xBF,0xE1,0x71,0xDB,0xA9,0xD7,0x41,0xAD,0x46,0xEA,0x19,0xA9,0xD5,0xCE,0x93,0xB3,0x35,0x73,0x0A,0x69,0x59,0x91,0xC3,0x0F,0x22,0x1B,0x1D,0x91,0x13,0x3D,0x91,0x73,0x43,0xF1,0x6C,0x55,0xDA,0x3A,0x4F,0xBA,0x25,0xCE,0x4F,0x04,0xF1,0xC5,0xCF,0x71,0xDA,0x3C,0xD7,0xB9,0xB2,0x48,0xB4,0x89,0x38,0x20,0x4B,0x2A,0x95,0x0C,0xD5,0xEF,0x5B,0xAD,0x96,0x45,0x8A,0x41,0x96,0x7A,0x1F,0x60,0x0D,0x7D,0x22,0x75,0x82,0x2B,0x0F,0xFB,0xCE,0x51,0x3D,0x2E,0x3A,0x21,0xF3,0x1C,0xD9,0x38,0x86,0x2C,0xC6,0x05,0xB6,0x7B,0x9A,0x8F,0x0F,0x97,0x1B,0x72,0x6F,0x1C,0xEB,0xAE,0xFF,0xDA,0x97,0x0D,0xBA,0x43,0x32,0xCA,0x66,0x34,0x3D,0x54,0xCB,0x24,0x9B,0x43,0xF2,0x70,0x3E,0x42,0xBB,0xA0,0x95,0x11,0x37,0x46,0xE1,0x4F,0x49,0xC5,0x1B,0xFC,0x3C,0x3A,0x3E,0xD1,0x65,0x0E,0x6F,0x58,0xF8,0x9E,0x5B,0xDB,0x55,0xB6,0x41,0x34,0xCB,0xBE,0xDB,0x87,0x5F,0xA9,0xD1,0x85,0x6B,0xB3,0x17,0x9C,0x61,0x0C,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xA2,0x5D,0x61,0x10,0xED,0x2A,0x9B,0xED,0xC9,0xFC,0xDF,0x14,0x54,0x8F,0x80,0x7A,0x06,0xF5,0x23,0xA0,0x9F,0x41,0xF3,0x10,0x30,0x4F,0x41,0xF3,0x18,0x30,0xCF,0xCA,0xFC,0xFF,0x35,0xC9,0x79,0xC9,0x89,0xFA,0x33,0xD7,0x1D,0xF6,0x5E,0x84,0x5C,0x56,0x6E,0xA7,0xDA,0x1E,0xF9,0xFA,0xAB,0xF5,0x97,0xFF,0x2F,0xED,0x89,0x7E,0x29,0x9E,0xB4,0x9F,0x74,0x1E,0x69,0xDA,0xA4,0x9F,0x81,0x94,0xEF,0x4F,0xF6,0xF9,0x0B,0xF4,0x65,0x51,0x08};


PROGMEM char *info[] = { 
                         "FT800 MainMenu Application",
                         "APP to demonstrate interactive menus,",
                         "using Jpeg decode,",
                         "Rectangle & Points."

                       }; 

                             
                             
static int32_t fsize = 0;



// API used to play the KEYsound
void Ft_Play_Sound(uint8_t sound,uint8_t vol,uint8_t midi)
{
  uint16_t val = (midi << 8) | sound; 
  FTImpl.Write(REG_SOUND,val);
  FTImpl.Write(REG_PLAY,1); 
}

static struct {
  signed short dragprev;
  int vel;      // velocity
  long base;    // screen x coordinate, in 1/16ths pixel
  long limit;
} scroller;

/* Api to Intialise the scroller*/
static void scroller_init(uint32_t limit)
{
  scroller.dragprev = -32768;
  scroller.vel = 0;      // velocity
  scroller.base = 0;     // screen x coordinate, in 1/16ths pixel
  scroller.limit = limit;
}
/* Api used to scroll the screen horizontally*/
static void scroller_run()
{
	sTagXY sTagxy;
	FTImpl.GetTagXY(sTagxy);
	int16_t sx = sTagxy.x;
  if ((sx != -32768) & (scroller.dragprev != -32768)) {
    scroller.vel = (scroller.dragprev - sx) << 4;
  } else {
    int change = max(1, abs(scroller.vel) >> 5);
    if (scroller.vel < 0)
      scroller.vel += change;
    if (scroller.vel > 0)
      scroller.vel -= change;
  }
  scroller.dragprev = sx;
  scroller.base += scroller.vel;
  scroller.base = max(0, min(scroller.base, scroller.limit));
}
/********API to return the assigned TAG value when penup,for the primitives/widgets******/

static uint8_t sk=0;
uint8_t Read_Keys()
{
  static uint8_t Read_tag=0,ret_tag=0,temp_tag=0;	
  Read_tag = FTImpl.Read(REG_TOUCH_TAG);
  ret_tag = NULL;
  if(Read_tag!=NULL)								// Allow if the Key is released
  {
    if(temp_tag!=Read_tag)
    {
      temp_tag = Read_tag;	
      sk = Read_tag;										// Load the Read tag to temp variable	
    }  
  }
  else
  {
    if(temp_tag!=0)
    {
      ret_tag = temp_tag;
      temp_tag = 0;
    }  
    sk = 0;
  }
  return ret_tag;
}

/* Load the icons of the menu to GRAM*/ 
void Load_Thumbnails()
{
FT_SDFile imfile;
  char *apps[] = {"1.jpg","2.jpg","3.jpg","4.jpg","5.jpg","6.jpg","7.jpg","8.jpg","9.jpg","10.jpg","11.jpg","12.jpg"};
  uint8_t imn,fnf=0;

  int16_t		
		fsize=0,
		blklen = 10*1024L,
		wsize;

  uint8_t temp[512];
  imn = 0;
  
  
  if(sd_present)
  {
    for(imn=0;imn<12;imn++)
    {
       FTImpl.DLStart(); 
      FTImpl.Cmd_Memset(THUMBNAIL_ADDRESS+(imn*blklen),random(150),50*2*100);
      FTImpl.Finish();
    }  
    return;
  }
  
  
  do
  {
    if(FtSd.OpenFile(imfile,apps[imn]))
    {
      FTImpl.WriteCmd(CMD_MEMSET);
      FTImpl.WriteCmd(imn*blklen);
      FTImpl.WriteCmd(random(150));
      FTImpl.WriteCmd(random(10*1024L));
    }
    else
    {
		FTImpl.WriteCmd(CMD_LOADIMAGE);
		FTImpl.WriteCmd(THUMBNAIL_ADDRESS+(imn*blklen));
		FTImpl.WriteCmd(0);
    
		while (imfile.Offset < imfile.Size){
			uint16_t n = min(512, imfile.Size - imfile.Offset);
			n = (n + 3) & ~3;   // force 32-bit alignment
			imfile.ReadSector(temp);
			FTImpl.WriteCmd(temp, n);//alignment is already taken care by this api
		}
	}
    imn++;
  }while(imn<MAX_MENUS);
}




/***********************API used to SET the ICON******************************************/
/*Refer the code flow in the flowchart availble in the Application Note */


void home_setup()
{
  /*Icon  file is deflated use J1 Command to inflate the file and write into the GRAM*/
  FTImpl.Cmd_Inflate(250*1024L);
  FTImpl.WriteCmdfromflash(home_star_icon,sizeof(home_star_icon));
  /*Set the Bitmap properties for the ICONS*/
  FTImpl.DLStart();        // start
  FTImpl.Clear(1,1,1);
  FTImpl.ColorRGB(255, 255, 255);
  FTImpl.BitmapHandle(13);    // handle for background stars
  FTImpl.BitmapSource(250*1024L);      // Starting address in gram
  FTImpl.BitmapLayout(FT_L4, 16, 32);  // format 
  FTImpl.BitmapSize(FT_NEAREST, FT_REPEAT, FT_REPEAT, 512, 512  );
  FTImpl.BitmapHandle(14);    // handle for background stars
  FTImpl.BitmapSource(250*1024L);      // Starting address in gram
  FTImpl.BitmapLayout(FT_L4, 16, 32);  // format 
  FTImpl.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 32, 32  );
  FTImpl.DLEnd();
  FTImpl.Finish();
}
//==============================================Information Screen =======================================
void Info()
{
  uint16_t dloffset = 0,z;
// Touch Screen Calibration
  FTImpl.DLStart(); 
  FTImpl.Clear(1,1,1);
  FTImpl.ColorRGB(255,255,255);
  FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,FT_DISPLAYHEIGHT/2,26,FT_OPT_CENTERX|FT_OPT_CENTERY,"Please tap on a dot");
  FTImpl.Cmd_Calibrate(0);
  FTImpl.DLEnd();
  FTImpl.Finish();
// Ftdi Logo animation 
  FTImpl.Cmd_Logo();
	while(FTImpl.CheckLogo() != FT_GE_FINISHED); 
  dloffset = FTImpl.Read16(REG_CMD_DL);
  dloffset -=4;
  //Copy the Displaylist from DL RAM to GRAM
  FTImpl.Cmd_Memcpy(100000L,FT_RAM_DL,dloffset);
  do
  {
    FTImpl.DLStart();   
    FTImpl.Cmd_Append(100000L,dloffset);
    //Reset the BITMAP properties used during Logo animation
    FTImpl.BitmapTransformA(256);
    FTImpl.BitmapTransformB(0);
    FTImpl.BitmapTransformC(0);
    FTImpl.BitmapTransformD(0);
    FTImpl.BitmapTransformE(256);
    FTImpl.BitmapTransformF(0);  
    FTImpl.SaveContext();	
    //Display the information with transparent Logo using Edge Strip 
    FTImpl.ColorRGB(219,180,150);
    FTImpl.ColorA(220);
    FTImpl.Begin(FT_EDGE_STRIP_A);
    FTImpl.Vertex2f(0,FT_DISPLAYHEIGHT*16);
    FTImpl.Vertex2f(FT_DISPLAYWIDTH*16,FT_DISPLAYHEIGHT*16);
    FTImpl.ColorA(255);
    FTImpl.RestoreContext();	
    FTImpl.ColorRGB(0,0,0);
   // INFORMATION 
    FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,20,28,FT_OPT_CENTERX|FT_OPT_CENTERY,(char*)pgm_read_word(&info[0]));
    FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,60,26,FT_OPT_CENTERX|FT_OPT_CENTERY,(char*)pgm_read_word(&info[1]));
    FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,90,26,FT_OPT_CENTERX|FT_OPT_CENTERY,(char*)pgm_read_word(&info[2]));  
    FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,120,26,FT_OPT_CENTERX|FT_OPT_CENTERY,(char*)pgm_read_word(&info[3]));  
    FTImpl.Cmd_Text(FT_DISPLAYWIDTH/2,FT_DISPLAYHEIGHT-30,26,FT_OPT_CENTERX|FT_OPT_CENTERY,"Click to play");
    //Check if the Play key and change the color
    if(sk!='P')
    FTImpl.ColorRGB(255,255,255);
    else
    FTImpl.ColorRGB(100,100,100);
    FTImpl.Begin(FT_POINTS);   
    FTImpl.PointSize(20*16);
    FTImpl.Tag('P');
    FTImpl.Vertex2f((FT_DISPLAYWIDTH/2)*16,(FT_DISPLAYHEIGHT-60)*16);
    FTImpl.ColorRGB(180,35,35);
    FTImpl.Begin(FT_BITMAPS);
    FTImpl.Vertex2ii((FT_DISPLAYWIDTH/2)-14,(FT_DISPLAYHEIGHT-75),14,4);
    FTImpl.DLEnd();
    FTImpl.Finish();
  }while(Read_Keys()!='P');
  Ft_Play_Sound(0x51,255,0xc0);
  /* wait until Play key is not pressed*/
}


/* API for android method*/

void menu()
{
  uint8_t 	imh = 50,	imw = 100,	dt = 30,dx,dy,	col,row,per_f,n_f,c_f=0,
	i,key_in=0,dg_count=0,temp=0;

  int16_t Ox,Oy,sx,drag=0,prev=0,drag_dt=30,dragth=0,dragpv=0,ddt;
  
  #define NOTOUCH		-32768
   
// for points

  uint8_t  Opt,pdt =15;
sTagXY sTagxy;

 
  dx = (dt*2)+imw;	
  dy = (10*2)+imh;
  col = FT_DISPLAYWIDTH/dx;
  row = 2;
  per_f = col*row;
  n_f = (MAX_MENUS-1)/per_f;

  Opt = (FT_DISPLAYWIDTH-(n_f+1)*(MENU_POINTSIZE+pdt))/2;
  
  Load_Thumbnails();


  FTImpl.WriteCmd(CMD_INFLATE);
  FTImpl.WriteCmd(121*1024L);
  FTImpl.WriteCmdfromflash(home_star_icon,sizeof(home_star_icon));
  FTImpl.DLStart();   	
  FTImpl.BitmapHandle(12);
  FTImpl.BitmapSource(121*1024L);
  FTImpl.BitmapLayout(FT_L4, 16, 32);
  FTImpl.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 32, 32);
  for(temp=0;temp<MAX_MENUS;temp++)
  {
    FTImpl.BitmapHandle(temp);	
    FTImpl.BitmapSource(THUMBNAIL_ADDRESS+(temp*10*1024L));
    FTImpl.BitmapLayout(FT_RGB565,100*2,50);	
    FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 100, 50);	
  }
  temp = 0;
  FTImpl.DLEnd();
  FTImpl.Finish();	
  scroller_init((FT_DISPLAYWIDTH*n_f)*16);
  while(1)
  {
/*Read touch screen x varaiation and tag in*/
FTImpl.GetTagXY(sTagxy);
sx=sTagxy.x;
Serial.print("sx: ");
Serial.println(sx);
    key_in =  FTImpl.Read(REG_TOUCH_TAG);	
	//key_in = sTagxy.tag;
/*Check if any tag in*/    
    if(sx!=NOTOUCH)	
    {
      dg_count++;     	
      temp = key_in;
    }
/*Move into the particular frame based on dragdt now 30pixels*/
    if(sx==NOTOUCH)
    {
      dg_count = 0; 		
      if(drag>((c_f*FT_DISPLAYWIDTH)+drag_dt)){drag = min((c_f+1)*FT_DISPLAYWIDTH,drag+15);} 
      if(drag<((c_f*FT_DISPLAYWIDTH)-drag_dt)) {drag = max((c_f-1)*FT_DISPLAYWIDTH,drag-15);}
      if(dragth==drag) {c_f = drag/FT_DISPLAYWIDTH*1L;}
      dragth = drag;
      scroller.vel = 0; 
      scroller.base = dragth*16;				// 16bit pre
   }
/*if tag in but still pendown take a scroller basevalue*/
   else if(dg_count>5)
   {
      key_in = 0;
      temp = 0;
      drag = scroller.base>>4;
    }
    if(key_in==0)  scroller_run();
 /*Display list start*/   
    FTImpl.DLStart();   

    FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
    FTImpl.ColorRGB(255,255,255);
    FTImpl.LineWidth(25);				// for rect	
    FTImpl.Begin(FT_RECTS);
    Oy = 10;
    for(i=0;i<=n_f;i++)
    {
      Ox = 10;  
      Ox+=(i*FT_DISPLAYWIDTH*1L);
      Ox-=drag;
      if(i==0) FTImpl.ColorRGB(156,100,128);
      if(i==1) FTImpl.ColorRGB(100,106,156);
      if(i==2) FTImpl.ColorRGB(156,152,100);   
      FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
      FTImpl.Vertex2f((Ox+FT_DISPLAYWIDTH-20)*16,(int16_t)(FT_DISPLAYHEIGHT*0.75)*16L);					// i pixels wide than image width +1 
    }
    FTImpl.ColorRGB(255,255,255); 
    for(i=0;i<MAX_MENUS;i++)
    {
      Ox = dt+dx*(i%col);                                          // Calculate the xoffsets
      Ox +=((i/per_f)*FT_DISPLAYWIDTH*1L);       
      Ox -= drag;        
      Oy = dt+(dy*((i/col)%row));
      if(Ox > (FT_DISPLAYWIDTH+dt)) 0;	
      else
      {
      FTImpl.Vertex2f((Ox-1)*16,(Oy-1)*16);	
      FTImpl.Vertex2f((imw+Ox+1)*16,(imh+Oy+1)*16);}					// i pixels wide than image width +1
    } 
    FTImpl.TagMask(1); 
    FTImpl.Begin(FT_BITMAPS);		
    for(i=0;i<MAX_MENUS;i++)
    {
      Ox = dt+dx*(i%col);                                          // Calculate the xoffsets
      Ox +=((i/per_f)*FT_DISPLAYWIDTH*1L);       
      Ox -= drag;        
      Oy = dt+(dy*((i/col)%row));	
      if(Ox > (FT_DISPLAYWIDTH+dt) || Ox < -dx) 0;	
      else
      {
      FTImpl.BitmapHandle(i);
      FTImpl.Tag(i+1);	
      FTImpl.Vertex2f(Ox*16,Oy*16);	
      }
    }
    FTImpl.TagMask(0);
  
  // frame_no_points
  
    FTImpl.PointSize(MENU_POINTSIZE*16);
    FTImpl.Begin(FT_POINTS);				
    FTImpl.ColorA(50);	
    Oy = FT_DISPLAYHEIGHT - 20;
    for(i=0;i<=n_f;i++)
    {
      Ox = Opt+(i*(MENU_POINTSIZE+pdt));
      FTImpl.Vertex2f(Ox*16,Oy*16);		
    }
    
    Ox = Opt+(c_f*(MENU_POINTSIZE+pdt));
    FTImpl.ColorA(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);	
    
    FTImpl.DLEnd();
    FTImpl.Finish();	
    key_in = temp; 

//   Show the  entered menu
    if(key_in!=0 && key_in<=12 && sx==NOTOUCH)
    {	
      Ft_Play_Sound(0x51,255,0xc0);
      FTImpl.DLStart();   
      FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
      FTImpl.Begin(FT_BITMAPS);
      FTImpl.BitmapHandle(key_in-1);
      FTImpl.BitmapTransformA(128);
      FTImpl.BitmapTransformE(128);
      FTImpl.BitmapSize(FT_NEAREST,FT_BORDER,FT_BORDER,200,100); 
      FTImpl.Vertex2f(((FT_DISPLAYWIDTH-200)/2)*16L,((FT_DISPLAYHEIGHT-100)/2)*16L);	
      FTImpl.BitmapTransformA(256);
      FTImpl.BitmapTransformE(256);
      FTImpl.ColorRGB(255,255,255);
      FTImpl.BitmapHandle(14);
      FTImpl.Tag('H');
      FTImpl.Vertex2f(5*16,5*16);	
      FTImpl.DLEnd();
      FTImpl.Finish();	
      while(Read_Keys()!='H');	
      Ft_Play_Sound(0x51,255,0xc0);
      //* wait until the home is pressed*//
      key_in = 0;temp = key_in; 
    }
  }
}

// Api to Single Row//
void menu_loopback()
{
	
  int16_t 
		imh = 50,
		imw = 100,
        	dt = 30,dx,dy,
		col,row,per_f,n_f,c_f=0,
		key_in=0,dg_count=0,temp=0;

  int16_t Ox,Oy,sx,drag=0,prev=0,drag_dt=30,dragth=0,dragpv=0,temp_drag=0,i,cts=0;

  uint8_t  Opt,pdt=15;
  sTagXY sTagxy;
  
  
  dx = (dt*2)+imw;	
  dy = (10*2)+imh;
  col = FT_DISPLAYWIDTH*1L/dx;
  row = 1;
  per_f = col*row;
  n_f = (MAX_MENUS-1)/per_f;

  Opt = (FT_DISPLAYWIDTH-(n_f+1)*(MENU_POINTSIZE+pdt))/2;
  Load_Thumbnails();
  FTImpl.WriteCmd(CMD_INFLATE);
  FTImpl.WriteCmd(121*1024L);
  FTImpl.WriteCmdfromflash(home_star_icon,sizeof(home_star_icon));
  FTImpl.DLStart();   	
  FTImpl.Clear(1,1,1);	
  FTImpl.BitmapHandle(12);
  FTImpl.BitmapSource(121*1024L);
  FTImpl.BitmapLayout(FT_L4, 16, 32);
  FTImpl.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 32, 32);
  for(temp=0;temp<MAX_MENUS;temp++)
  {
    FTImpl.BitmapHandle(temp);	
    FTImpl.BitmapSource(THUMBNAIL_ADDRESS+(temp*10*1024L));
    FTImpl.BitmapLayout(FT_RGB565,100*2,50);	
    FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 100, 50);	
  }
  temp = 0;
  FTImpl.DLEnd();
  FTImpl.Finish();	
  scroller_init((FT_DISPLAYWIDTH*1L*n_f)*16);

  while(1)
  {
/*Read touch screen x varaiation and tag in*/
FTImpl.GetTagXY(sTagxy);
sx=sTagxy.x;
    //key_in =  FTImpl.Read(REG_TOUCH_TAG);	
	key_in = sTagxy.tag;
 
/*Check if any tag in*/    
    if(sx!=NOTOUCH)	
    {
      dg_count++;     	
      temp = key_in;
      scroller.vel = 0;
    }
/*Move into the particular frame based on dragdt now 30pixels*/
   if(sx==NOTOUCH)
   {
      dg_count = 0;
      drag = scroller.base>>4; 	
   }
/*if tag in but still pendown take a scroller basevalue*/
   else if(dg_count>5)
   {
      key_in = 0;
      temp = key_in;
      drag = scroller.base>>4;
   }
   if(key_in==0)
   scroller_run();
		

   FTImpl.DLStart();   
   FTImpl.Clear(1,1,1);
   FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
   FTImpl.TagMask(1);
   FTImpl.ColorRGB(255,255,255);
   FTImpl.LineWidth(1*16);				// for rect	
   Oy = (FT_DISPLAYHEIGHT-imw)/2;					//dt+(dy*((i/col)%row));
   cts = drag/dx;                       // noof items moved in +/- directions
   dragth = drag%dx;

   for(i=-1;i<(per_f+1);i++)
   {
    Ox = dt+dx*i;
    Ox-=dragth;
	if(Ox > (FT_DISPLAYWIDTH+dt) || Ox < (-dx))
	{

	}
    else
    {
      FTImpl.Begin(FT_RECTS);
      FTImpl.Vertex2f((Ox-1)*16,(Oy-1)*16);	
      FTImpl.Vertex2f((imw+Ox+1)*16,(imh+Oy+1)*16);					// i pixels wide than image width +1
      FTImpl.Begin(FT_BITMAPS);											// draw the bitmap
      FTImpl.BitmapHandle((MAX_MENUS+i+cts)%12);
      FTImpl.Tag((1+i+cts)%(MAX_MENUS+1));	
      FTImpl.Vertex2f(Ox*16,Oy*16);	
    }
   }
   FTImpl.DLEnd();
   FTImpl.Finish();	
   
   key_in = temp;
   //   Show the  entered menu
   if(key_in!=0 && key_in<=12 && sx==NOTOUCH)
   {	
      Ft_Play_Sound(0x51,255,0xc0);
      FTImpl.DLStart();   
      FTImpl.Clear(1,1,1);
      FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
      FTImpl.Begin(FT_BITMAPS);
      FTImpl.BitmapHandle(key_in-1);
      FTImpl.BitmapTransformA(128);
      FTImpl.BitmapTransformE(128);
      FTImpl.BitmapSize(FT_NEAREST,FT_BORDER,FT_BORDER,200,100); 
      FTImpl.Vertex2f(((FT_DISPLAYWIDTH-200)/2)*16,((FT_DISPLAYHEIGHT-100)/2)*16);	
      FTImpl.BitmapTransformA(256);
      FTImpl.BitmapTransformE(256);
      FTImpl.ColorRGB(255,255,255);
      FTImpl.BitmapHandle(14);
      FTImpl.Tag('H');
      FTImpl.Vertex2f(5*16,5*16);	
      FTImpl.DLEnd();
      FTImpl.Finish();	
      while(Read_Keys()!='H');	
      Ft_Play_Sound(0x51,255,0xc0);
            //* wait until the home is pressed*//
      key_in = 0;temp = key_in;
    }
  }  
}  

// Api to tiles type menu*/
void menu_win8()
{
  uint8_t 
	imh = 50,imw = 100,dt = 30,dx,dy,col,row,per_f,n_f,c_f=0,
        i,key_in=0,dg_count=0,temp=0;
	int16_t Ox,Oy,sx,drag=0,prev=0,dragth=0,dragpv=0,ddt;
	sTagXY sTagxy;

	uint8_t color[12][3] = {  0xE0,0x01B,0xA2,
				     0x1B,0xE0,0xA8,
				     0x9E,0x9E,0x73,
				     0xE0,0x8E,0x1B,
				     0xB8,0x91,0xB3,
		                     0x6E,0x96,0x8e,
				     0x1B,0x60,0xE0,
				     0xC7,0xE3,0x7B,
				     0x8B,0x1B,0xE0,
				     0xE3,0x91,0xC1,
				     0xE0,0x8E,0x1B,
				     0xAC,0x91,0xE3,
				};

	char *mes[20]=	{  "Music",  "Gauges ",  "Gradient",  "Photo",  "Metaball",  "Notepad",  "Signature",  "Sketch","Swiss","Waves","Player","Clocks"};

// for points

	uint8_t  Opt,ps = 5,pdt=15;

	
  dx = (dt*2)+imw;	
  dy = (10*2)+imh;
  col = FT_DISPLAYWIDTH/dx;
  row = 2;
  per_f = col*row;
  n_f = (MAX_MENUS-1)/per_f;
  Opt = (FT_DISPLAYWIDTH-(n_f+1)*(ps+pdt))/2;
  
  Load_Thumbnails();
  FTImpl.WriteCmd(CMD_INFLATE);
  FTImpl.WriteCmd(121*1024L);
  FTImpl.WriteCmdfromflash(home_star_icon,sizeof(home_star_icon));
  FTImpl.DLStart();   	
  FTImpl.Clear(1,1,1);	
  FTImpl.BitmapHandle(12);
  FTImpl.BitmapSource(121*1024L);
  FTImpl.BitmapLayout(FT_L4, 16, 32);
  FTImpl.BitmapSize(FT_NEAREST, FT_BORDER, FT_BORDER, 32, 32);
  for(temp=0;temp<MAX_MENUS;temp++)
  {
  FTImpl.BitmapHandle(temp);	
  FTImpl.BitmapSource(THUMBNAIL_ADDRESS+(temp*10*1024L));
  FTImpl.BitmapLayout(FT_RGB565,100*2,50);	
  FTImpl.BitmapSize(FT_BILINEAR, FT_BORDER, FT_BORDER, 100, 50);	
  }
  temp = 0;
  FTImpl.DLEnd();
  FTImpl.Finish();	
  scroller_init((FT_DISPLAYWIDTH*n_f)*16);
  while(1)
  {
  FTImpl.GetTagXY(sTagxy);
  sx=sTagxy.x;

    key_in =  FTImpl.Read(REG_TOUCH_TAG);
    if(key_in!=0)
    key_in = key_in;
    if(sx!=NOTOUCH)	
    {
      dg_count++;	
      temp = key_in;
      scroller.vel = 0;
    }
    if(sx==NOTOUCH)
    {
      dg_count = 0;	
      drag = scroller.base>>4; 		
    }
    else if(dg_count>5)
    {
      key_in = 0;
      temp = key_in;
      drag = scroller.base>>4;
    }
    if(key_in==0)
    {
      scroller_run();
    }

   FTImpl.DLStart();   
   FTImpl.Clear(1,1,1);
   FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
   FTImpl.TagMask(1);
   FTImpl.Tag(0);
   FTImpl.ColorRGB(255,255,255);
   FTImpl.LineWidth(1*16);				// for rect	
   FTImpl.Begin(FT_RECTS);

#if (FT_DISPLAY_RESOLUTION == FT_DISPLAY_WQVGA_480x272)

   imw = 220;
   imh = 100;		
   for(i=0;i<12;i+=6)
   {
    Ox = 10+FT_DISPLAYWIDTH*(i/6);
    Ox -= drag;
    Oy = 10+(110*((i/col)%row));
    FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
    FTImpl.Tag(i+1);	
    FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
    FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
   }
   for(i=5;i<12;i+=6)
   {
     Ox = 250+FT_DISPLAYWIDTH*(i/6);
     Ox -= drag;
     Oy = 10+(110*((i/col)%row));
     FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
     FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
     FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
   }

   imw = 100;
   imh = 100;	

   for(i=1;i<3;i+=1)
   {
     Ox = 250+FT_DISPLAYWIDTH*(i/per_f)+120*(i/2);
     Ox -= drag;
     Oy = 10+(110*((i/col)%row));
     FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
     FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
     FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
   }
   for(i=3;i<5;i+=1)
   {
     Ox = 10+FT_DISPLAYWIDTH*(i/per_f)+120*(i/4);
     Ox -= drag;
     Oy = 10+(110*((i/col)%row));
     FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
     FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
     FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
   }
   for(i=7;i<9;i+=1)
   {
     Ox = 250+FT_DISPLAYWIDTH*(i/per_f)+120*(i/8);
     Ox -= drag;
     Oy = 10+(110*((i/col)%row));
     FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
     FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
     FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
   }
   for(i=9;i<11;i+=1)
   {
     Ox = 10+FT_DISPLAYWIDTH*(i/per_f)+120*(i/10);
     Ox -= drag;
     Oy = 10+(110*((i/col)%row));
     FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
     FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
     FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
    }
    
    
    FTImpl.TagMask(0);
    
    FTImpl.ColorRGB(255,255,255);
    for(i=0;i<12;i+=6)
    {
      Ox = 10+FT_DISPLAYWIDTH*(i/4);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }
    for(i=5;i<12;i+=6)
    {
      Ox = 250+FT_DISPLAYWIDTH*(i/6);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }
    for(i=1;i<3;i+=1)
    {
      Ox = 250+FT_DISPLAYWIDTH*(i/per_f)+120*(i/2);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }
    for(i=3;i<5;i+=1)
    {
      Ox = 10+FT_DISPLAYWIDTH*(i/per_f)+120*(i/4);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }

    for(i=7;i<9;i+=1)
    {
      Ox = 250+FT_DISPLAYWIDTH*(i/per_f)+120*(i/8);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }
    for(i=9;i<11;i+=1)
    {
      Ox = 10+FT_DISPLAYWIDTH*(i/per_f)+120*(i/10);
      Ox -= drag;
      Oy = 10+(110*((i/col)%row));
      FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
    }
// Draw the bitmaps

   FTImpl.Begin(FT_BITMAPS);
   for(i=0;i<12;i+=6)
   {
      Ox = 65+FT_DISPLAYWIDTH*(i/6);
      Ox -= drag;
      Oy = 35+(110*((i/col)%row));
      FTImpl.BitmapHandle(i);
      FTImpl.Tag(255);
      FTImpl.Vertex2f(Ox*16,Oy*16);
  }
  for(i=5;i<12;i+=6)
  {
    Ox = 305+FT_DISPLAYWIDTH*(i/6);
    Ox -= drag;
    Oy = 35+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);
  }
		
  FTImpl.BitmapTransformA(512);

  for(i=1;i<3;i+=1)
  {
    Ox = 275+FT_DISPLAYWIDTH*(i/per_f)+120*(i/2);
    Ox -= drag;
    Oy = 35+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);				// i pixels wide than image width +1
  }
  for(i=3;i<5;i+=1)
  {
    Ox = 35+FT_DISPLAYWIDTH*(i/per_f)+120*(i/4);
    Ox -= drag;
    Oy = 35+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);				// i pixels wide than image width +1
  }

  for(i=7;i<9;i+=1)
  {
    Ox = 275+FT_DISPLAYWIDTH*(i/per_f)+120*(i/8);
    Ox -= drag;
    Oy = 35+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);				// i pixels wide than image width +1
  }
  for(i=9;i<11;i+=1)
  {
    Ox = 35+FT_DISPLAYWIDTH*(i/per_f)+120*(i/10);
    Ox -= drag;
    Oy = 35+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);				// i pixels wide than image width +1
  }
  FTImpl.BitmapTransformA(256);
  
#elif (FT_DISPLAY_RESOLUTION == FT_DISPLAY_WQVGA_320x240)
  imw = 170;
  imh = 100;		
  for(i=0;i<12;i+=2)
  {
    Ox = 10+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 10+(110*((i/col)%row));
    FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
     FTImpl.Tag(i+1);	
    FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
    FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
  }

  imw = 110;
  imh = 100;	

  for(i=1;i<12;i+=2)
  {
    Ox = 200+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 10+(110*((i/col)%row));
    FTImpl.ColorRGB(color[i][0],color[i][1],color[i][2]);
        FTImpl.Tag(i+1);	
    FTImpl.Vertex2f((Ox)*16,(Oy)*16);	
    FTImpl.Vertex2f((imw+Ox+1)*16,(100+Oy+1)*16);					// i pixels wide than image width +1
  }
      FTImpl.TagMask(0); 
      
  FTImpl.ColorRGB(255,255,255);
  for(i=0;i<12;i+=2)
  {
    Ox = 10+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 10+(110*((i/col)%row));
    FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
  }
  for(i=1;i<12;i+=2)
  {
    Ox = 200+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 10+(110*((i/col)%row));
    FTImpl.Cmd_Text(Ox+10,Oy+80,26,0,mes[i]);
  }
		
  imw = 100;
  imh = 50;
  FTImpl.Begin(FT_BITMAPS);											// draw the bitmap
  for(i=0;i<12;i+=2)
  {
    Ox = 75+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 20+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);	
  }
  FTImpl.BitmapTransformA(512);
  for(i=1;i<12;i+=2)
  {
    Ox = 230+FT_DISPLAYWIDTH*(i/4);
    Ox -= drag;
    Oy = 20+(110*((i/col)%row));
    FTImpl.BitmapHandle(i);
    FTImpl.Tag(255);
    FTImpl.Vertex2f(Ox*16,Oy*16);	
  }
  FTImpl.BitmapTransformA(256);
#endif

  FTImpl.DLEnd();
  FTImpl.Finish();	
  key_in = temp;
  // show the entered menu
  if(key_in!=0 && key_in<=12 && sx==NOTOUCH)
  {	
      Ft_Play_Sound(0x51,255,0xc0);

    FTImpl.DLStart();
    FTImpl.Clear(1,1,1);
    FTImpl.Cmd_Gradient(0,0,0x1A99E8,0,FT_DISPLAYHEIGHT,0x0A4F7A);
    FTImpl.Begin(FT_BITMAPS);
    FTImpl.BitmapHandle(key_in-1);
    FTImpl.BitmapTransformA(128);
    FTImpl.BitmapTransformE(128);
    FTImpl.BitmapSize(FT_NEAREST,FT_BORDER,FT_BORDER,200,100);
    FTImpl.Vertex2f(((FT_DISPLAYWIDTH-200)/2)*16,((FT_DISPLAYHEIGHT-100)/2)*16);	
    FTImpl.BitmapTransformA(256);
    FTImpl.BitmapTransformE(256);
    FTImpl.ColorRGB(255,255,255);
    FTImpl.BitmapHandle(14);
    FTImpl.Tag('H');
    FTImpl.Vertex2f(5*16,5*16);	
    FTImpl.DLEnd();
    FTImpl.Finish();				
    while(Read_Keys()!='H');	
      Ft_Play_Sound(0x51,255,0xc0);
   //* wait until the home is pressed*//
    key_in = 0;  temp = key_in;
   }
  }
}


/* Api to bootup ft800, verify FT800 hardware and configure display/audio pins */
/* Returns 0 in case of success and 1 in case of failure */
int16_t BootupConfigure(){
	uint32_t chipid = 0;
	FTImpl.Init(FT_DISPLAY_RESOLUTION);//configure the display to the WQVGA

	delay(20);//for safer side
	chipid = FTImpl.Read32(FT_ROM_CHIPID);
	
	/* Identify the chip */
	if(FT800_CHIPID != chipid)
	{
		Serial.print("Error in chip id read ");
		Serial.println(chipid,HEX);
		return 1;
	}
	
	/*Platform pressure sensitivity adjustment*/
	FTImpl.Write16(REG_TOUCH_RZTHRESH,1200); 
	
	/* Set the Display & audio pins */
	FTImpl.SetDisplayEnablePin(FT_DISPENABLE_PIN);
    FTImpl.SetAudioEnablePin(FT_AUDIOENABLE_PIN); 
	FTImpl.DisplayOn(); 
	FTImpl.AudioOn(); 	
	return 0;
}

/* Display an on-screen message if the storage device is not found */
void CheckStorageDevicePrecence(){
	while(sd_present){
		FTImpl.DLStart();     
		FTImpl.Clear(1,1,1);
		FTImpl.ColorRGB(255,255,255);
		FTImpl.Cmd_Text(FT_DISPLAYWIDTH>>1,FT_DISPLAYHEIGHT>>1,27,FT_OPT_CENTER,"STORAGE DEVICE NOT FOUND");
		FTImpl.DLEnd();
		FTImpl.Finish();
		delay(5000);
		}
}


void setup()
{
	/*Initialize the SD object. Screen error message can only be displayed when the FT800 is ready*/
	sd_present = FtSd.Init();
	
	/* Initialize serial print related functionality */
	Serial.begin(9600);
	
	/* Set the Display Enable pin*/   
	Serial.println("--Start Application--");
	if(BootupConfigure())
	{
		//error case - do not do any thing
	}
  	else
	{
		CheckStorageDevicePrecence(); 
		home_setup();
		Info(); 
		#ifdef ANDROID_METHOD
			menu();
		#endif 
		#ifdef LOOPBACK_METHOD
			menu_loopback();
		#endif
		#ifdef WIN8_METHOD
			menu_win8();
		#endif
		FTImpl.Exit();  /*  Safely close all opened handles */
	}
	Serial.println("--End Application--");
}

void loop()
{

}



/* Nothing beyond this */













  








