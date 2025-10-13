#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	cam = new ofEasyCam;
    ofBackground(10, 10, 20);
	ofEnableDepthTest();
	ofSetCylinderResolution(8, 2);

    baseAngle = 35.0;
    baseLength = 27.5;

    lsysString = "FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]FFF[+F&F][-F/F][^F\F]F[+FF[+F&F][-F/F][^F\F]F&FF[+F&F][-F/F][^F\F]F][-FF[+F&F][-F/F][^F\F]F/FF[+F&F][-F/F][^F\F]F][^FF[+F&F][-F/F][^F\F]F\FF[+F&F][-F/F][^F\F]F]FF[+F&F][-F/F][^F\F]F";
    
    // precomputed randomness for angles
    for (char c : lsysString) {
        if (c == '+' || c == '-' || c == '&' || c == '^' || c == '\\' || c == '/')
            randomOffsets.push_back(ofRandom(-5, 5));
        else
            randomOffsets.push_back(0.0f);
    }
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    cam->begin();
    ofEnableDepthTest();

    ofPushMatrix();
    ofTranslate(0, -300, -500);

    // base info
    float segLength = baseLength;
    float baseRadius = 8.0f;
    float taper = 0.5f;
    int stackDepth = 0;
    int index = 0;

    ofSetColor(255, 225, 210);

    for (char c : lsysString) {
        float offset = randomOffsets[index++]; // stable offset for this command

        switch (c) {
        // forward
        case 'F': {
            float radius = baseRadius * pow(taper, stackDepth);
            ofDrawCylinder(0, segLength / 2, radius, segLength);
            ofTranslate(0, segLength, 0);
            break;
        }
        // rotation
        case '+': ofRotateDeg(baseAngle + offset, 0, 0, 1); break;
        case '-': ofRotateDeg(-baseAngle + offset, 0, 0, 1); break;
        case '&': ofRotateDeg(baseAngle + offset, 1, 0, 0); break;
        case '^': ofRotateDeg(-baseAngle + offset, 1, 0, 0); break;
        case '\\': ofRotateDeg(baseAngle + offset, 0, 1, 0); break;
        case '/': ofRotateDeg(-baseAngle + offset, 0, 1, 0); break;
        // push or pop matrix
        case '[':
            ofPushMatrix();
            segLength *= 0.8;
            baseRadius *= 0.8;
            stackDepth++;
            break;
        case ']':
            ofPopMatrix();
            segLength /= 0.8;
            baseRadius /= 0.8;
            stackDepth--;
            break;
        }
    }

    ofPopMatrix();
    cam->end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
