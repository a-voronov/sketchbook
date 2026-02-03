#pragma once

#include <random>
#include <memory>

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

#include "grid.h"
#include "gridVisitor.h"

using Algorithm = std::function<void(Grid&, std::mt19937&)>;

class ofApp : public ofBaseApp, public GridVisitor {
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
    unique_ptr<Grid> grid;

    void visit(DistanceGrid&, std::mt19937& rng) override;
    void visit(ColorGrid&, std::mt19937& rng) override;
    // a pair of distanced cells batches and an index of currently drawn batch
    pair<vector<vector<const Cell*>>, int> distanced_cells;

    ofxPanel gui;
    ofxIntSlider rows;
    ofxIntSlider columns;
    ofParameter<ofColor> picked_color;
    ofxFloatSlider picked_intensity_stretch;
    ofxIntSlider animation_speed;
    ofxToggle output_ascii;

    ofParameter<string> selected_algorithm;
    unique_ptr<ofxDropdown> algorithms_dropdown;

    ofEventListeners listeners;

private:
    void generate_distance_grid();
    void generate_color_grid();

    const Algorithm& get_selected_algorithm() const;
};
