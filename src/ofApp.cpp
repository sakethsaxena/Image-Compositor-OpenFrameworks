


#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(245, 245, 245);
	ofSetVerticalSync(true);
	ofSetWindowTitle("Pic Compositor");
	ofEnableAlphaBlending();



	// set when <control> key is held down
	//
	ctrlKeyDown = false;
	//
	// set when <control> key is held down
	//
	selectedImage = NULL;
}

//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(245,245,245);
	ofFill();

	for (int i = 0; i < images.size(); i++ ) {
		images[i]->draw();
	}

}

//
// delete all images in list on exit
//
void ofApp::exit() {
	for (int i = 0; i < images.size(); i++) {
		delete images[i];
	}
}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'b': 
	case 'B': 
		// send image to the back
		curr = images[0];
		for (int i = 0; i < images.size()-1; i++)
		{
			images[i] = images[i + 1];
		}
		images[images.size()-1] = curr;
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;

	case 's':
	case 'S':
		// remove the selection marker and save the image
		if (selectedImage) {
			selectedImage->bSelected = false;
		}
		draw();
		fullImage.grabScreen(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		fullImage.save("composition.jpg");

		break;
	case OF_KEY_ALT:
		altKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		ctrlKeyDown = true;
		break;
	case OF_KEY_RIGHT:
		rightKeyDown = true;
		
		break;
	case OF_KEY_DOWN:
		downKeyDown = true;
		
		break;
	case OF_KEY_LEFT:
		leftKeyDown = true;
		
		break;
	case OF_KEY_UP:
		upKeyDown = true;
		break;
	case 'a':
	case 'A':
		scaleUpAspectRatio = true;
		break;
	case 'd':
	case 'D': 
		scaleDownAspectRatio = true;
		break;
	case OF_KEY_DEL:
		deleteImg();
		break;
	}
}


void ofApp::update() {

	// applies scaling alt key is pressed along with one of the arrow keys 
	if (altKeyDown) {

		// ctrl + A/a
		if (scaleUpAspectRatio && !upKeyDown && !rightKeyDown && !leftKeyDown && !downKeyDown) {
			selectedImage->scale += ofVec2f(0.001, 0.001);
		}
		//ctrl + D/d
		else if (scaleDownAspectRatio && !upKeyDown && !rightKeyDown && !leftKeyDown && !downKeyDown) {
			selectedImage->scale += ofVec2f(-0.001, -0.001);

		}// ctrl + up
		else if (upKeyDown && !rightKeyDown && !leftKeyDown && !downKeyDown) {
			selectedImage->scale += ofVec2f(0, 0.001);
		}//ctrl + down
		else if (downKeyDown && !rightKeyDown && !leftKeyDown && !upKeyDown)
		{

			selectedImage->scale += ofVec2f(0, -0.001);

		}// ctrl + right
		 else if (rightKeyDown && !leftKeyDown && !downKeyDown && !upKeyDown) {

			selectedImage->scale += ofVec2f(0.001, 0);
		}
		  //ctrl + left
		else if (leftKeyDown && !rightKeyDown && !downKeyDown && !upKeyDown) {

			selectedImage->scale += ofVec2f(-0.001, 0);
		}
		else {
			return;
		}

	}
	//applies rotation if control key is pressed along with left or right arrow
	else if (ctrlKeyDown) {
		if (rightKeyDown)
		{
			selectedImage->rot += 0.1;
		}
		if (leftKeyDown)
		{
			selectedImage->rot += -0.1;
		}

	}
	// applies translation only if arrow keys are pressed.
	else {
		if (upKeyDown) {
			selectedImage->trans += ofVec2f(0, -1);
		}
		if (downKeyDown)
		{
			selectedImage->trans += ofVec2f(0, 1);
		}
		if (rightKeyDown) {
			selectedImage->trans += ofVec2f(1, 0);
		}

		if (leftKeyDown) {
			selectedImage->trans += ofVec2f(-1, 0);
		}

	}

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {

	case 'a':
	case 'A':
		scaleUpAspectRatio = false;
		break;
	case 'd':
	case 'D':
		scaleDownAspectRatio = false;
		break;
	case OF_KEY_LEFT_ALT:
	case OF_KEY_RIGHT_ALT:
		altKeyDown = false;
		break;
	case OF_KEY_LEFT_CONTROL:
	case OF_KEY_RIGHT_CONTROL:
		ctrlKeyDown = false;
		break;
	case OF_KEY_RIGHT:
		rightKeyDown = false;

		break;
	case OF_KEY_DOWN:
		downKeyDown = false;

		break;
	case OF_KEY_LEFT:
		leftKeyDown = false;

		break;
	case OF_KEY_UP:
		upKeyDown = false;
		break;

	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	// Rotatate object when ctrl key + mousedrag
	if (ctrlKeyDown) doMouseRotation(x, y);
	
	//Image translation mouse is dragged on selected Image then Image moves along with the mouse pointer
	if (selectedImage && !ctrlKeyDown)
	{
		selectedImage->trans = ofVec2f(x, y);
	}
}

//  Applies Mouse Rotation to the Image when ctrl + mousedragg event occurs

void ofApp::doMouseRotation(int x, int y) {

	// if there are no images stored in the list, just return

	if (images.size() == 0) return;

	float rotation;
	ofVec2f mouse = ofVec2f(x, y);
	ofVec2f delta = mouse - lastMouse;
	float dist = delta.length();

	// if mouse is moving from left to right, apply positive rotation
	// otherwise apply negative rotation.
	// 
	if (mouse.x < lastMouse.x)
		rotation = -dist;
	else
		rotation = dist;

	// store value of where the mouse was last for next entry
	// 
	lastMouse = mouse;

	// apply rotation to image (will be rotated to this value on next redraw)
	
	selectedImage ->rot += rotation;
}





//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	renderSelection();
	processSelection(x, y);
	lastMouse = ofVec2f(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
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
//
// Simple file drag and drop implementation.
//

void ofApp::dragEvent(ofDragInfo dragInfo){ 

	Image *imageObj = new Image();
	imageObj->trans = dragInfo.position;
	if (imageObj->image.load(dragInfo.files[0]) == true)
		images.push_back(imageObj);
	else {
		cout << "Can't load image: " << dragInfo.files[0] << endl;
		delete imageObj;
	}
}


void ofApp::deleteImg() {
	images.erase(images.begin() + images.size() - 1);
}

//
// Render for the purposes of selection hit testing.  In this case
// we use the color method. We render echo object as a different
// value of r,g,b.  We then compare the pixel under the mouse. The
// value is the index into the image list;
//
void ofApp::renderSelection() {
	
	ofBackground(245,245,245);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < images.size(); i++) {
		images[i]->draw(true, i);
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
}

void ofApp::processSelection(int x, int y) {
	unsigned char res[4];
	GLint viewport[4];

	// read pixel under mouse x y
	//
	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(x, viewport[3] - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);

	if (selectedImage) {
		selectedImage->bSelected = false;
		selectedImage = NULL;
	}

	//
	//  lookup image by color of pixel
	//

	if (res[0] > 0 && res[0] <= images.size()) {
		Image *image = images[res[0] - 1];
		image->bSelected = true;
		selectedImage = image;

		// move selected image to beginning of list

		images.erase(images.begin() + (res[0] - 1) );
		images.push_back(image);
	}

}

Image::Image()
{
	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	rot = 0;
	bSelected = false;


}

void Image::draw(bool bSelectMode, int index) {
	ofPushMatrix();
	ofTranslate(trans);
	ofScale(scale);
	ofRotate(rot);

	//
	// if drawing image normally
	//

	if (!bSelectMode) {

		//
		// if selected, draw a blue box around image
		//
		if (bSelected) {
			ofNoFill();
			ofSetColor(0, 0, 255);
			ofSetLineWidth(3);
			selRect.x = -image.getWidth() / 2.0;
			selRect.y = -image.getHeight() / 2.0;
			selRect.width = image.getWidth();
			selRect.height = image.getHeight();
			ofDrawRectangle(selRect);
			ofFill();

		}
		ofSetColor(255, 255, 255, 255);
		image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
	
	}
	//
	// else if drawing to test selection
	//
	else {
		ofFill();
		ofSetColor(index + 1, 0, 0);
		ofDrawRectangle(-image.getWidth() / 2.0, -image.getHeight() / 2.0,
			image.getWidth(), image.getHeight());
	}
	ofPopMatrix();
}

