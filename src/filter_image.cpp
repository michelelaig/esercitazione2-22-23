#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image &im) {
    float sum = 0;
    for (int k=0;k<im.w*im.h*im.c;k++) sum+=im.data[k];
    for (int k=0;k<im.w*im.h*im.c;k++) im.data[k]/=sum;
}

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w) {
    assert(w % 2); // w needs to be odd
    Image ret(w,w,1);
    for (int k=0;k<w*w;k++) ret.data[k]=1.0;
    l1_normalize(ret);
    return ret;
}

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds

    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the convolution operator
    int w=filter.w;

	if(preserve){
		
		Image ret(im.w, im.h, im.c);
		for(int x=0; x<im.w; x++){
			for(int y=0; y<im.h; y++){
				for(int c=0; c<im.c; c++){
					float sum=0; //somma della maschera 

					for(int i=-w/2; i<=w/2; i++){//maschera
						for(int j=-w/2; j<=w/2; j++){
							sum+=im.clamped_pixel(x+i,y+j,c)*filter.clamped_pixel(i+w/2,j+w/2,0);
						}
					}//fine maschera
				ret.set_pixel(x,y,c, sum);
				}
			}
		}
	return ret;
	}
	
	
	else {
		printf("\nnon preserve\n");
		printf("\nw=%d\n",w);

		Image ret(im.w, im.h, 1);
		for(int x=0; x<im.w; x++){
			for(int y=0; y<im.h; y++){
				//for(int c=0; c<im.c; c++){
				float sum=0; //somma della maschera 

				for(int i=-w/2; i<=w/2; i++){//maschera
					for(int j=-w/2; j<=w/2; j++){
						sum+=im.clamped_pixel(x+i,y+j,0)*filter.clamped_pixel(i+w/2,j+w/2,0);
						sum+=im.clamped_pixel(x+i,y+j,1)*filter.clamped_pixel(i+w/2,j+w/2,0);
						sum+=im.clamped_pixel(x+i,y+j,2)*filter.clamped_pixel(i+w/2,j+w/2,0);

					}
				}//fine maschera
			ret.set_pixel(x,y,0, sum);
			}
		}
			return ret;
	}
	
    // Make sure to return ret and not im. This is just a placeholder
	
}

// HW1 #2.2+ Fast convolution
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image_fast(const Image &im, const Image &filter, bool preserve) {
    assert(filter.c == 1);
    Image ret;
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds

    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the fast convolution operator. Remember to use Eigen for matrix operations
    NOT_IMPLEMENTED();

    // Make sure to return ret and not im. This is just a placeholder
    return im;
}

void print_f(Image f){
    cout << "w: " << f.w << " h: " << f.h << endl;
    for (int i=0;i<f.w;i++) {
        cout << "| ";
        for (int j=0;j<f.w;j++) {
            cout << " " << f(i,j);
        }
        cout <<"|" << endl;
    }
    
}
Image make_arr_filter(float* arr,int l){
    Image ret(l,l,1);
    for (int k=0;k<l*l;k++) ret.data[k] = arr[k];
    
    print_f(ret);
    return ret;
}

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter() {
    float a[9] = {0,-1,0,-1,4,-1,0,-1,0}; 
    return make_arr_filter(a,3);

}

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter() {
    // TODO: Implement the filter
    NOT_IMPLEMENTED();

    return Image(1, 1, 1);

}

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter() {
    // TODO: Implement the filter
    NOT_IMPLEMENTED();

    return Image(1, 1, 1);

}

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma) {
    int ker_dim = ceil(6*sigma);
    //cout << "ciao" << ker_dim << endl;

    if (!ker_dim%2==0) ker_dim++;
    //cout << "ciao" << ker_dim << endl;
    auto g = [&] (int x,int y){
        int x_dist = x-ker_dim/2;
        int y_dist = y-ker_dim/2;
        return exp(-1 * (x_dist * x_dist + y_dist * y_dist) / (2 * sigma * sigma)) /
              (2 * M_PI * sigma * sigma);
    };
    Image ret = make_box_filter(ker_dim);
    for (int i=0;i<ker_dim;i++)    for (int j=0;j<ker_dim;j++)
    ret(i,j) = g(i,j);
    //l1_normalize(ret);
    return ret;
    
}


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size
    Image ret(a.w,a.h,a.c);
    for(int k=0;k<a.h*a.w*a.c;k++) ret.data[k]=a.data[k]+b.data[k];
    return ret;

}

Image sub_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size
    Image ret(a.w,a.h,a.c);
    for(int k=0;k<a.h*a.w*a.c;k++) ret.data[k]=a.data[k]-b.data[k];
    return ret;

}

Image sub_mod_image(const Image &a, const Image &b) {
    assert(a.w == b.w && a.h == b.h &&
           a.c == b.c); // assure images are the same size
    Image ret(a.w,a.h,a.c);
    for(int k=0;k<a.h*a.w*a.c;k++) ret.data[k]=a.data[k]-b.data[k];
    for(int k=0;k<a.h*a.w*a.c;k++) if (ret.data[k]<0) ret.data[k]-=2*ret.data[k];
    return ret;

}

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter() {
    // TODO: Implement the filter
    NOT_IMPLEMENTED();

    return Image(1, 1, 1);
}

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter() {
    // TODO: Implement the filter
    NOT_IMPLEMENTED();

    return Image(1, 1, 1);
}

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image &im) {
    assert(im.w * im.h); // assure we have non-empty image

    // TODO: Normalize the features for each channel
    NOT_IMPLEMENTED();

}


// Normalizes features across all channels
void feature_normalize_total(Image &im) {
    assert(im.w * im.h * im.c); // assure we have non-empty image

    int nc = im.c;
    im.c = 1;
    im.w *= nc;

    feature_normalize(im);

    im.w /= nc;
    im.c = nc;

}


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image, Image> sobel_image(const Image &im) {
    // TODO: Your code here
    NOT_IMPLEMENTED();

    return {im, im};
}


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image &im) {

    // TODO: Your code here
    NOT_IMPLEMENTED();

    return im;
}


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image &im, float sigma1, float sigma2) {
    Image bf = im;

    // TODO: Your bilateral code
    NOT_IMPLEMENTED();

    return bf;
}

// HW1 #4.5+ Fast bilateral filter
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter_fast(const Image &im, float sigma1, float sigma2) {
    Image bf = im;

    // TODO: Your fast bilateral code
    NOT_IMPLEMENTED();

    return bf;
}

// HM #5
//
float *compute_histogram(const Image &im, int ch, int num_bins) {
    float *hist = (float *) malloc(sizeof(float) * num_bins);
    for (int i = 0; i < num_bins; ++i) {
        hist[i] = 0;
    }

    // TODO: Your histogram code
    NOT_IMPLEMENTED();

    return hist;
}

float *compute_CDF(float *hist, int num_bins) {
    float *cdf = (float *) malloc(sizeof(float) * num_bins);

    cdf[0] = hist[0];

    // TODO: Your cdf code
    NOT_IMPLEMENTED();

    return cdf;
}

Image histogram_equalization_hsv(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // TODO: Your histogram equalization code
    NOT_IMPLEMENTED();
    // convert to hsv
    // compute histograms for the luminance channel
    // compute cdf
    // equalization
    // convert back to rgb

    // delete the allocated memory!
//    delete hist;
//    delete cdf;

    return new_im;
}

Image histogram_equalization_rgb(const Image &im, int num_bins) {
    Image new_im(im);
    float eps = 1.0 / (num_bins * 1000);

    // compute histograms for each color channel
    for (int c = 0; c < im.c; ++c) {

        // TODO: Your equalization code
        NOT_IMPLEMENTED();

        // delete the allocated memory!
//        delete hist;
//        delete cdf;
    }

    return new_im;
}


// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }

void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }

void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image &a, const Image &b) { return sub_image(a, b); }

Image operator+(const Image &a, const Image &b) { return add_image(a, b); }
