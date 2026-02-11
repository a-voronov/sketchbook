#pragma once

#include <random>
#include <memory>

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDropdown.h"

#include "grid.h"
#include "gridVisitor.h"

using AlgorithmCtor = std::function<void(Grid&, std::mt19937&)>;
using GridCtor = std::function<unique_ptr<Grid>()>;

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
    vector<pair<string, GridCtor>> all_grids;
    unique_ptr<Grid> grid;
    // a pair of distanced cells batches and an index of currently drawn batch
    pair<vector<vector<const Cell*>>, int> distanced_cells;
    vector<const Cell*> deadends;

    void visit(DistanceGrid&, std::mt19937& rng) override;
    void visit(ColorGrid&, std::mt19937& rng) override;
    void visit(MaskedGrid&, std::mt19937& rng) override;

    ofxPanel gui;
    ofxIntSlider rows;
    ofxIntSlider columns;
    ofParameter<ofColor> tone_color;
    ofxFloatSlider picked_intensity_stretch;
    ofxIntSlider animation_speed;
    ofxToggle output_ascii;
    ofxToggle repeat;
    ofxButton mask_picker;

    ofxToggle show_deadends;
    ofParameter<ofColor> deadends_color;

    ofParameter<string> selected_algorithm;
    unique_ptr<ofxDropdown> algorithms_dropdown;

    ofParameter<string> selected_grid;
    unique_ptr<ofxDropdown> grids_dropdown;

    ofEventListeners listeners;

private:
    // ofxDropdown allows deselection, and we can end up in an empty state,
    // so these backup values help us to restore previously selected state
    string selected_algorithm_;
    string selected_grid_;
    // apparently ofxDropdown is calling listener twice when param changes,
    // which in case of masked grid causes to prompt user with file dialog twice if user decides not to pick any file
    string selected_grid_attempt_;
    string mask_path_;

    bool generate_grid();
    void pick_mask();

    const AlgorithmCtor& get_selected_algorithm() const;
    const GridCtor& get_selected_grid() const;
};
