#include <cmath>
#include "image.h"
#include <iostream>
using namespace std;


float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  return clamped_pixel(round(x),round(y),c);
  }


//h

float Image::pixel_bilinear(float x, float y, int c) const {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)

  int x_low = floor(x);
  int x_high = ceil(x);
  int y_low = floor(y);
  int y_high = ceil(y);

  float A1 = (x - x_low) * (y - y_low);
  float A2 = (x - x_low) * (y_high - y);
  float A3 = (x_high - x) * (y - y_low);
  float A4 = (x_high - x) * (y_high - y);

  return A4 * clamped_pixel(x_low, y_low, c) +
         A3 * clamped_pixel(x_low, y_high, c) +
         A2 * clamped_pixel(x_high, y_low, c) +
         A1 * clamped_pixel(x_high, y_high, c);
}


Image nearest_resize(const Image& im, int w, int h)  {
  Image ret(w,h,im.c);
  for (int x=0;x<w;x++) for (int y=0;y<h;y++) for (int c=0;c<im.c;c++)
  ret.set_pixel(x,y,c,im.pixel_nearest(((x+.5)*im.w/w)-.5,(((y+.5)*im.h/h)-.5),c));
  return ret;
  }



//h




Image bilinear_resize(const Image& im, int w, int h) {
  Image ret(w, h, im.c);

  float x_scale = (1.0 * w) / im.w;
  float y_scale = (1.0 * h) / im.h;

  auto original_x = [&](int x) { return ((x + 0.5) / x_scale) - 0.5; };
  auto original_y = [&](int y) { return ((y + 0.5) / y_scale) - 0.5; };

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      // (x’,y’) = (-1/2, -1/2) + scale*(x+1/2,y+1/2)
      for (int c = 0; c < im.c; c++) {
        ret(i, j, c) = im.pixel_bilinear(original_x(i), original_y(j), c);
      }
    }
  }
  return ret;
}


