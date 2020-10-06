//--------------------------------------------------------------
//
// This is a simple first OpenFrameworks example of how to draw
// a primitive (such as a triangle), select it and drag/rotate it
// around in the viewport with the mouse.  The program illustrates
// a few key concepts that will prove to be useful later in our 
// journey
// (1) How to do simple 2D intersection testing on triangles.
// (2) How to compute a 2D rotation transformation manually using 
//     trig functions (the rotation matrix).
// (3) How to use the glm::vec3 class for simple point/vector ops
// (4) The concept of a local coordinate system around a shape (triangle) 
//     which can translated to any arbitrary position and rotated
//     about it's origin.
// (5) Using basic callbacks in OF.
// (6) Animating along a path
//   
//  (c) Kevin M. Smith - 2019 

#include "ofApp.h"


void Triangle::draw() {
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

// Determine if point p is inside the triangle shape. Method is the simplest 
	// way but not necessarily the most efficent.  We look at the sign of the angles
	// between the vectors connecting p and the three triangle vertices. They each
	// have to be ( < 0 ) for the point to be inside.  Note: glm::orientedAngle
	// will provide a signed angle if you supply an orientation vector (in this case Z axis).
	//
bool Triangle::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	glm::vec3 v1 = glm::normalize(p1 - p);
	glm::vec3 v2 = glm::normalize(p2 - p);
	glm::vec3 v3 = glm::normalize(p3 - p);
	float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
	float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
	float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
	if (a1 < 0 && a2 < 0 && a3 < 0) return true;
	else return false;
}

void Image::load() {
	image.load("pencil.png");
	width = image.getWidth();
	height = image.getHeight();
}

void Image::draw() {
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	image.draw(-width / 2.0, -height / 2.0);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::setup(){
	gui.setup();
	gui.add(speed.setup("Speed", 5, 1, 10));
	gui.add(ampli.setup("Amplitude", 150, 0, 300));
	gui.add(cycle.setup("Cycles", 4, 1, 8));
	gui.add(path.setup("Show Path", true));
	gui.add(image.setup("Use Image", false));

	i.load();
	i.pos = curveEval(0, ampli, cycle);

	tri.pos = curveEval(0, ampli, cycle);
	tri.verts.push_back(glm::vec3(-50, -50, 0));
	tri.verts.push_back(glm::vec3(0, 100, 0));
	tri.verts.push_back(glm::vec3(50, -50, 0));
	glm::vec3 temp = glm::normalize(glm::vec3(0, 100, 0));
	tri.header = curveEval(tri.pos.x + 1, ampli, cycle) - tri.pos;
	tri.header = glm::normalize(tri.header);
	float angle = glm::orientedAngle(temp, tri.header, glm::vec3(0, 0, 1));
	tri.rotation += angle * 180 / PI;
	tri.pos = curveEval(tri.pos.x + speed * tri.header.x, ampli, cycle);

	float angle2 = glm::orientedAngle(glm::vec3(1, 0, 0), tri.header, glm::vec3(0, 0, 1));
	i.rotation = angle2 * 180 / PI;
	i.pos = curveEval(tri.pos.x + speed * tri.header.x, ampli, cycle);
	ofSetBackgroundColor(ofColor::black);
}


//--------------------------------------------------------------
void ofApp::update(){
	if (startAnim) {
		if (tri.pos.x + speed > ofGetWidth()) {
			tri.pos.x = 0;
		}

		glm::vec3 sine = curveEval(tri.pos.x + speed*tri.header.x, ampli, cycle);
		glm::vec3 newHeader = sine - tri.pos;
		newHeader = glm::normalize(newHeader);
		float angle = glm::orientedAngle(tri.header, newHeader, glm::vec3(0, 0, 1));
		tri.rotation += angle * 180 / PI;
		tri.pos = sine;
		tri.header = newHeader;

		i.rotation += angle*180/PI;
		i.pos = sine;
	}
}

void ofApp::draw() {
	gui.draw();

	if (image) {
		i.draw();
	}
	else {
		ofSetColor(ofColor::blue);
		tri.draw();
	}
	
	ofSetColor(ofColor::purple);
	if (path) {
		for (int i = 0; i < ofGetWidth(); i++) {
			glm::vec3 p = curveEval(i, ampli, cycle);
			ofDrawCircle(p.x, p.y, 1);
		}
	}
}

// Given x in pixel coordinates, return (x, y, z) on the sin wave
// Note that "z" is not used, so it is set to "0".
//
// Additional Parameters
//    scale - scales the curve in Y  (the amplitude)
//    cycles - number of cycles in sin wave.
//
glm::vec3 ofApp::curveEval(float x, float scale, float cycles)
{
	// x is in screen coordinates and his in [0, WindowWidth]
	float u = (cycles * x * PI) / ofGetWidth();
	return (glm::vec3(x, -scale *sin(u) + (ofGetHeight() / 2), 0));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		bFullscreen = !bFullscreen;
		ofSetFullscreen(bFullscreen);
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case ' ':
		startAnim = !startAnim;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!bDrag) return;

	glm::vec3 mousePoint(x, y, 0);

	if (bCtrlKeyDown) {
		tri.rotation += x - lastMouse.x;
	}
	else {
		tri.pos += mousePoint - lastMouse;
	}
	lastMouse = mousePoint;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// check if we mouse down inside the triangle.  If true, we can drag or rotate .
	// we need to do this check in screen spacec since the mouse coordinate is in screen space
	//
	vector<glm::vec3> points;
	rotate(tri.verts, points, glm::radians(tri.rotation));

	if (tri.inside(glm::vec3(x, y, 0), tri.pos + points[0], tri.pos + points[1], tri.pos + points[2])) {
		bDrag = true;
		lastMouse = glm::vec3(x, y, 0);
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
