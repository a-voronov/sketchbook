#pragma once

#include <random>

#include "ofMain.h"
#include "ofxGui.h"
#include "colorGrid.h"

class ofApp : public ofBaseApp{
public:
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y ) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void mouseEntered(int x, int y) override;
    void mouseExited(int x, int y) override;
    void windowResized(int w, int h) override;
    void dragEvent(ofDragInfo dragInfo) override;
    void gotMessage(ofMessage msg) override;

    std::mt19937 rng;
    ColorGrid grid;

    pair<vector<vector<const Cell*>>, int> distanced_cells;

    ofxPanel gui;
    ofParameter<ofColor> picked_color;
    ofxFloatSlider picked_intensity_stretch;
    ofxIntSlider animation_speed;
    ofxToggle output_ascii;

private:
    void generate_regular_grid();
    void generate_distance_grid();
    void generate_color_grid();
};
