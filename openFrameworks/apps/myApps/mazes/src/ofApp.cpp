#include "ofApp.h"
#include "binaryTree.h"
#include "sidewinder.h"

const static int frame_rate = 60;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(frame_rate);
    ofSetVerticalSync(true);

    gui.setup();
    gui.add(picked_color.set("color", ofColor::lightGreen, ofColor(0, 0, 0), ofColor(255 , 255, 255)));
    gui.add(picked_intensity_stretch.setup("intensity", 0.75f, 0.0f, 1.0f));
    gui.add(animation_speed.setup("animation speed", 4, 0, frame_rate));

    rng = std::mt19937{std::random_device{}()};
    // rng = std::mt19937{42};
    grid = ColorGrid{56, 84};
    // BinaryTree::on(grid, rng);
    Sidewinder::on(grid, rng);

    // MARK: ColoredGrid

    auto start = grid.cell_at(grid.rows() / 2, grid.columns() / 2);
    auto distances = start->distances();

    distanced_cells = {distances.distanced_cells(), 0};
    distanced_cells.second = -1;

    grid.set_distances(distances);
    cout << grid << endl;

    // MARK: DistanceGrid

    // auto start = grid.cell_at(0, 0);
    // auto distances = start->distances();
    // auto [new_start, _] = distances.max();
    //
    // grid.distances = make_unique<Distances>(distances);
    // cout << grid << endl;
    //
    // auto new_distances = new_start->distances();
    // auto [goal, _] = new_distances.max();
    //
    // grid.distances = make_unique<Distances>(new_distances.path_to(*goal));
    // cout << grid << endl;

    // MARK: Grid

    // auto distances = grid.cell_at(0, 0)->distances();
    // grid.distances = make_unique<Distances>(distances);
    // cout << grid << endl;
    //
    // cout << "path from northwest corner to southwest corner:" << endl;
    // const auto south_west = grid.cell_at(grid.rows() - 1, 0);
    // grid.distances = make_unique<Distances>(distances.path_to(*south_west));
    // cout << grid << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    int update_speed = int(frame_rate / animation_speed);
    if (ofGetFrameNum() % update_speed == 0) {
        if (distanced_cells.second < distanced_cells.first.size() - 1) {
            ++distanced_cells.second;
        } else {
            distanced_cells.second = 0;
        }
    }
    if (animation_speed == 0) {
        distanced_cells.second = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::white);
    DrawCfg draw_cfg{
        .tone = picked_color,
        .intensity_stretch = picked_intensity_stretch,
        .cell_size = 25,
        .wall_width = 2
    };

    if (animation_speed == 0) {
        grid.draw(draw_cfg);
    } else {
        auto grid_cfg = draw_cfg;
        grid_cfg.draw_mode = DrawMode::Walls;
        grid.draw(grid_cfg);

        auto cells_cfg = draw_cfg;
        cells_cfg.draw_mode = DrawMode::BgColor;
        auto [cells, cells_batch] = distanced_cells;
        for (int i = 0; i < cells_batch; ++i)
            grid.draw_cells(cells.at(i), cells_cfg);
    }

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

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
