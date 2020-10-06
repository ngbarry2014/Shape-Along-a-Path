#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p) { return false; }   // returns True if p is inside Shape

	glm::mat4 getMatrix() {
		glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 M = glm::rotate(m, glm::radians(rotation), glm::vec3(0, 0, 1));
		return M;
	}

	// data
	//
	vector<glm::vec3> verts;
	glm::vec3 header;
	glm::vec3 pos;   // translation
	float rotation = 0;   // degrees
};

class Triangle: public Shape {
public:
	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void draw();

};

class Image : public Shape {
public:
	void draw();
	void load();
	ofImage image;
	float width;
	float height;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
	  	void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	
		glm::vec3 curveEval(float x, float scale, float cycles);
		

		// rotate vertices (vin) by "angle" and return result in (vout).
		//
		void rotate(const vector<glm::vec3> &vin, vector<glm::vec3> &vout, float angle);

		// application data
	
		glm::vec3 lastMouse;   // location of where mouse moved last (when dragging)

		// UI control data
		//
		bool bFullscreen = false;	
		bool bCtrlKeyDown = false;
		bool bDrag = false;

		// create one triangle
		//
		Triangle tri;
		Image i;

		bool startAnim = false;

		ofxFloatSlider speed;
		ofxFloatSlider ampli;
		ofxFloatSlider cycle;
		ofxPanel gui;
		ofxToggle path;
		ofxToggle image;
};
