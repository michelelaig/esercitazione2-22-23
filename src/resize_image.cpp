#include <cmath>
#include "image.h"
#include <iostream>

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)



float Image::pixel_nearest(float x, float y, int c) const
  {
  
  return clamped_pixel(round(x),round(y),c);
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  int x_f = floor(x); int x_c = ceil(x);
  int y_f = floor(y); int y_c = ceil(y);

  float dxf = 1-(x-x_f); float dxc = 1-(x_c-x);
  float dyf = 1-(y-y_f); float dyc = 1-(y_c-y);

  return  clamped_pixel(x_f,y_f,c) * dxf * dyf
          + clamped_pixel(x_c,y_f,c) * dxc * dyf
          + clamped_pixel(x_c,y_c,c) * dxc * dyc
          + clamped_pixel(x_f,y_c,c) * dxf * dyc;
  
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  float scale_w = (float) w/im.w;
  float scale_h = (float) h/im.h;
  auto original_x = [&](int x) { return ((x + 0.5) / scale_w) - 0.5; };
  auto original_y = [&](int y) { return ((y + 0.5) / scale_h) - 0.5; };

  for (int c= 0;c<im.c;c++) for (int j=0;j<h;j++) for(int i=0;i<w;i++)
  ret.set_pixel(i,j,c,im.pixel_nearest(original_x(i),original_y(j),c));
  
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  float scale_w = (float) w/im.w;
  float scale_h = (float) h/im.h;
  auto original_x = [&](int x) { return ((x + 0.5) / scale_w) - 0.5; };
  auto original_y = [&](int y) { return ((y + 0.5) / scale_h) - 0.5; };
  
  for (int c= 0;c<im.c;c++) for (int j=0;j<h;j++) for(int i=0;i<w;i++)
  ret.set_pixel(i,j,c,im.pixel_bilinear(original_x(i),original_y(j),c));

  
  return ret;
  }


