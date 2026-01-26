#include "ofApp.h"
#include "binaryTree.h"
#include "sidewinder.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    rng = std::mt19937{std::random_device{}()};
    // rng = std::mt19937{42};
    grid = DistanceGrid{8, 8};
    // BinaryTree::on(grid, rng);
    Sidewinder::on(grid, rng);

    auto start = grid.cell_at(0, 0);
    auto distances = start->distances();
    auto [new_start, _] = distances.max();

    grid.distances = make_unique<Distances>(distances);
    cout << grid << endl;

    auto new_distances = new_start->distances();
    auto [goal, _] = new_distances.max();

    grid.distances = make_unique<Distances>(new_distances.path_to(*goal));
    cout << grid << endl;

    // auto distances = grid.cell_at(0, 0)->distances();
    // grid.distances = make_unique<Distances>(distances);
    // cout << grid << endl;

    // cout << "path from northwest corner to southwest corner:" << endl;
    // const auto south_west = grid.cell_at(grid.rows() - 1, 0);
    // grid.distances = make_unique<Distances>(distances.path_to(*south_west));
    // cout << grid << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    grid.draw(50);
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
