#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->sphere_list.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->sphere_list.clear();

	vector<ofColor> base_color_list;
	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		base_color_list.push_back(color);
	}

	int color_index = 0;
	for (int i = 0; i < 150; i++) {

		auto u_speed = ofRandom(1, 1.5);
		auto v_speed = ofRandom(1, 1.5);
		auto sphere_u = ofRandom(360) + ofGetFrameNum() * u_speed;
		auto sphere_v = ofRandom(360) + ofGetFrameNum() * v_speed;
		int sphere_r = ofRandom(30);

		this->sphere_list.addVertex(this->make_point(200, sphere_r, sphere_u, sphere_v));

		this->sphere_list.addColor(base_color_list[color_index]);
		color_index = (color_index + 1) % base_color_list.size();
	}

	for (int i = 0; i < this->sphere_list.getNumVertices(); i++) {

		for (int k = i + 1; k < this->sphere_list.getNumVertices(); k++) {

			if (glm::distance(this->sphere_list.getVertex(i), this->sphere_list.getVertex(k)) < 30) {

				this->sphere_list.addIndex(i);
				this->sphere_list.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	int v_span = 1;
	int u_span = 60;

	ofMesh face, line;
	line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	for (int v = 0; v <= 360; v += v_span) {

		bool flag = true;
		int u_start = ofMap(ofNoise(200 * cos(v * DEG_TO_RAD) * 0.002, 200 * sin(v * DEG_TO_RAD) * 0.002, ofGetFrameNum() * 0.005), 0, 1, -360, 360);
		int next_u = ofMap(ofNoise(200 * cos((v + v_span) * DEG_TO_RAD) * 0.002, 200 * sin((v + v_span) * DEG_TO_RAD) * 0.002, ofGetFrameNum() * 0.005), 0, 1, -360, 360);
		for (int u = u_start; u < u_start + 360; u += u_span) {

			if (flag) {

				face.addVertex(this->make_point(200, 50, u, v));
				face.addVertex(this->make_point(200, 50, u + u_span, v));
				face.addVertex(this->make_point(200, 50, next_u + u_span, v + v_span));
				face.addVertex(this->make_point(200, 50, next_u, v + v_span));

				line.addVertex(this->make_point(200, 50, u, v));
				line.addVertex(this->make_point(200, 50, u + u_span, v));
				line.addVertex(this->make_point(200, 50, next_u + u_span, v + v_span));
				line.addVertex(this->make_point(200, 50, next_u, v + v_span));

				ofColor color = ofColor(39);
				//color.setHsb(ofMap(u, u_start, u_start + 360, 0, 255), 150, 255);

				face.addColor(color);
				face.addColor(color);
				face.addColor(color);
				face.addColor(color);

				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 3);
				face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

				line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 4);
				line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 3);
			}

			next_u += u_span;
			flag = !flag;
		}
	}

	face.drawFaces();

	ofSetColor(239);
	line.drawWireframe();

	this->sphere_list.drawWireframe();

	for (int i = 0; i < this->sphere_list.getNumVertices(); i++) {

		ofSetColor(this->sphere_list.getColor(i));
		ofDrawSphere(this->sphere_list.getVertex(i), 4);
	}

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u)) * cos(v);
	auto y = (R + r * cos(u)) * sin(v);
	auto z = r * sin(u);

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}