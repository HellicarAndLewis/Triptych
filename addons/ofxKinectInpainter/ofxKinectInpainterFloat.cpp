#include "ofxKinectInpainterFloat.h"

ofxKinectInpainterFloat::ofxKinectInpainterFloat() {
	scale = 4;
	inpaintRadius = DEFAULT_INPAINT_RADIUS;
}

void ofxKinectInpainterFloat::setup(int width, int height, int downsampling) {
	scale = downsampling;
	this->width = width;
	this->height = height;
	mask.allocate(width, height);
	inpainted.allocate(width, height);
	scaled.allocate((float)width/scale, (float)height/scale);
	scaledMask.allocate((float)width/scale, (float)height/scale);
	
}


// inpainting happens at a low resolution becuse it's quite
// expensive to compute
void ofxKinectInpainterFloat::inpaint(ofxCvFloatImage &img) {
	
	
	// inverted threshold at pix>1 will give a mask of holes only.
	cvThreshold(img.getCvImage(), mask.getCvImage(), 1, 255, CV_THRESH_BINARY_INV);
	mask.flagImageChanged();
	
	// scale the mask down
	scaledMask.scaleIntoMe(mask);
	
	// make a scaled version of the image for inpainting
	scaled.scaleIntoMe(img);
	
	
	// do the actual inpainting on a low res version
	// using INPAINT_NS is faster than the other option (cv::INPAINT_TELEA)
	cv::Mat imgg = scaled.getCvImage();
	cv::inpaint(scaled.getCvImage(), scaledMask.getCvImage(), imgg, inpaintRadius, cv::INPAINT_NS);
	scaled.flagImageChanged();
	
	
	// scale up the inpainted image
	inpainted.scaleIntoMe(scaled, CV_INTER_LINEAR);
	
	//inpainted.convertToRange(0, 100);
	// now blend it with the orignal, using a mask
	cvCopy(inpainted.getCvImage(), img.getCvImage(), mask.getCvImage());
	img.flagImageChanged();
}

void ofxKinectInpainterFloat::setInpaintingRadius(int radius) {
	inpaintRadius = radius;
}
void ofxKinectInpainterFloat::setDownsampling(int downsampling) {
	if(scale!=downsampling) {
		scale = downsampling;
		scaled.allocate((float)width/scale, (float)height/scale);
		scaledMask.allocate((float)width/scale, (float)height/scale);
	}
}


int ofxKinectInpainterFloat::getInpaintingRadius() {
	return inpaintRadius;
}

int ofxKinectInpainterFloat::getDownsampling() {
	return scale;
}