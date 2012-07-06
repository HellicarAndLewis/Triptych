/**
 *  KinectSkeletons.cpp
 *
 *  Created by Marek Bereza on 03/07/2012.
 */

#include "KinectSkeletons.h"


void KinectSkeletons::setup() {
	kinect.setup();
	kinect.setLogLevel(OF_LOG_VERBOSE);
	kinect.addDepthGenerator();
	kinect.addUserGenerator();
	kinect.setMaxNumUsers(2);
	kinect.start();
}

void KinectSkeletons::update() {
	kinect.update();
}

void KinectSkeletons::exit() {
	kinect.stop();
}

void KinectSkeletons::drawDebug() {
	kinect.drawDebug();
}


int KinectSkeletons::getNumSkeletons() {
	return kinect.getNumTrackedUsers();
}
KinectSkeleton KinectSkeletons::getSkeleton(int index) {
	KinectSkeleton skel;
	ofxOpenNIUser &user = kinect.getTrackedUser(index);
	skel.id = user.getXnID();
	skel.leftHand  = user.getJoint(JOINT_LEFT_HAND) .getProjectivePosition();
	skel.rightHand = user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition();
	return skel;
}