/*inline double findnoise(double x)
{
  int x1 = (((int)x)<<13) ^ x;
  return (double)( 1.0 - ( (x1 * (x1 * x1 * 15731 + 789221) + 1376312589) & Ox7fffffff) / 1073741824.0);
}*/

inline double findnoise(double x,double y)
{
  int n=(int)x+(int)y*57;
  n=(n<<13)^n;
  int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
  return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x)
{
  double ft=x * 3.1415927;
  double f=(1.0-cos(ft))* 0.5;
  return a*(1.0-f)+b*f;
}

double perlinNoise(double x,double y)
{
  double floorx=(double)((int)x);//This is kinda a cheap way to floor a double integer.
  double floory=(double)((int)y);
  double s,t,u,v;//Integer declaration
  s=findnoise(floorx,floory); 
  t=findnoise(floorx+1,floory);
  u=findnoise(floorx,floory+1);//Get the surrounding pixels to calculate the transition.
  v=findnoise(floorx+1,floory+1);
  double int1=interpolate(s,t,x-floorx);//Interpolate between the values.
  double int2=interpolate(u,v,x-floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.
  return interpolate(int1,int2,y-floory);//Here we use y-floory, to get the 2nd dimension.
}

/*
SDL_Surface *clouds(int w,int h,double zoom,double p, int r, int g, int b)//w and h speak for themselves, zoom wel zoom in and out on it, I usually
{ //										   use 75. P stands for persistence, this controls the roughness of the picture, i use 1/2
  int octaves=2;
  SDL_Surface *ret=SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,24,0,0,0,0);//Create an empty image.
  for(int y=0;y<h;y++)
  {  //Loops to loop trough all the pixels
    for(int x=0;x<w;x++)
    {
      double getnoise = 0;
      for(int a=0;a<octaves-1;a++)//This loops trough the octaves.
      {
	    double frequency = pow(2,a);//This increases the frequency with every loop of the octave.
	    double amplitude = pow(p,a);//This decreases the amplitude with every loop of the octave.
	    getnoise += perlinNoise(((double)x)*frequency/zoom,((double)y)/zoom*frequency)*amplitude;//This uses our perlin noise functions. It calculates all our zoom and frequency and amplitude
      } //											It gives a decimal value, you know, between the pixels. Like 4.2 or 5.1
      int color= (int)((getnoise*128.0)+128.0);//Convert to 0-256 values.
      if(color>255)
	    color=255;
      if(color<0)
	    color=0;
      SetPixel(ret,x,y,(int)((r/255.0)*(double)color),(int)((g/255.0)*(double)color),(int)((b/255.0)*(double)color));//This colours the image with the RGB values 
    }//														   given at the beginning in the function.
  }
  return ret;
}*/


