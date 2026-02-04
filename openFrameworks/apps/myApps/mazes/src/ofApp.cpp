#include "ofApp.h"

#include "distanceGrid.h"
#include "colorGrid.h"
#include "gridVisitor.h"

#include "binaryTree.h"
#include "sidewinder.h"
#include "aldousBroder.h"
#include "wilsons.h"

const static int frame_rate = 60;

const static vector<pair<string, GridCtor>> all_grids{
    {"DistanceGrid", [](int r, int c) { return make_unique<DistanceGrid>(r, c); }},
    {"ColorGrid",    [](int r, int c) { return make_unique<ColorGrid>(r, c); }}
};

const static vector<pair<string, AlgorithmCtor>> all_algorithms{
    {"BinaryTree",   [](Grid& g, std::mt19937& rng) { BinaryTree::on(g, rng); }},
    {"Sidewinder",   [](Grid& g, std::mt19937& rng) { Sidewinder::on(g, rng); }},
    {"AldousBroder", [](Grid& g, std::mt19937& rng) { AldousBroder::on(g, rng); }},
    {"Wilsons",      [](Grid& g, std::mt19937& rng) { Wilsons::on(g, rng); }},
};

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(frame_rate);
    ofSetVerticalSync(true);

    rng = std::mt19937{std::random_device{}()};
    // rng = std::mt19937{42};

    gui.setup();
    gui.add(rows.setup("rows", 56, 2, 100));
    gui.add(columns.setup("columns", 84, 2, 100));
    gui.add(picked_color.set("color", ofColor::lightGreen, ofColor(0, 0, 0), ofColor(255 , 255, 255)));
    gui.add(picked_intensity_stretch.setup("intensity", 0.75f, 0.0f, 1.0f));
    gui.add(animation_speed.setup("animation speed", 4, 0, frame_rate));
    // only affects mazes drawn on screen
    gui.add(output_ascii.setup("ASCII", false));

    selected_algorithm.setName("algorithm");
    algorithms_dropdown = make_unique<ofxDropdown>(selected_algorithm);
    algorithms_dropdown->enableCollapseOnSelection(true);
    algorithms_dropdown->disableMultipleSelection(true);
    gui.add(algorithms_dropdown.get());
    for (auto kv : all_algorithms)
        algorithms_dropdown->add(kv.first);

    selected_grid.setName("grid");
    grids_dropdown = make_unique<ofxDropdown>(selected_grid);
    grids_dropdown->enableCollapseOnSelection(true);
    grids_dropdown->disableMultipleSelection(true);
    gui.add(grids_dropdown.get());
    for (auto kv : all_grids)
        grids_dropdown->add(kv.first);

    selected_algorithm_ = selected_algorithm = all_algorithms.at(1).first;
    selected_grid_ = selected_grid = all_grids.at(1).first;
    listeners.push(selected_algorithm.newListener([&](const ofParameter<string>& p){
        // ofxDropdown allows deselection
        // and reverting ofOption inside of the listener lambda doesn't trigger UI redraw no matter what,
        // hence we rely on backup local values to restore previously selected value inside `update` method
        if (p.get() == "") {
            algorithms_dropdown.get()->hideDropdown();
            return;
        }
        selected_algorithm_ = p.get();
        generate_grid();
    }));
    listeners.push(selected_grid.newListener([&](const ofParameter<string>& p){
        if (p.get() == "") {
            grids_dropdown.get()->hideDropdown();
            return;
        }
        selected_grid_ = p.get();
        generate_grid();
    }));

    generate_grid();
}

//--------------------------------------------------------------
void ofApp::generate_grid() {
    auto& grid_ctor = get_selected_grid();
    auto new_grid = grid_ctor(rows, columns);
    grid = std::move(new_grid);
    grid.get()->accept(*this, rng);
}

const AlgorithmCtor& ofApp::get_selected_algorithm() const {
    auto algorithm = find_if(all_algorithms.begin(), all_algorithms.end(), [&](const auto& kv) {
        return kv.first == selected_algorithm.get();
    });
    if (algorithm == all_algorithms.end())
        throw runtime_error("Unsupported algorithm: " + selected_algorithm.get());

    return algorithm->second;
}

const GridCtor& ofApp::get_selected_grid() const {
    auto grid_type = find_if(all_grids.begin(), all_grids.end(), [&](const auto& kv) {
        return kv.first == selected_grid.get();
    });
    if (grid_type == all_grids.end())
        throw runtime_error("Unsupported grid type: " + selected_grid.get());

    return grid_type->second;
}

//--------------------------------------------------------------
void ofApp::visit(DistanceGrid& grid, std::mt19937& rng) {
    auto& algorithm = get_selected_algorithm();
    algorithm(grid, rng);

    auto start = grid.cell_at(0, 0);
    auto distances = start->distances();
    auto [new_start, _] = distances.max();

    if (output_ascii) {
        grid.set_distances(distances);
        cout << grid << endl;
    }

    auto new_distances = new_start->distances();
    auto [goal, _] = new_distances.max();
    auto path = new_distances.path_to(*goal);

    distanced_cells = {path.distanced_cells(), -1};
    grid.set_distances(path);

    if (output_ascii)
        cout << grid << endl;
}

void ofApp::visit(ColorGrid& grid, std::mt19937& rng) {
    auto& algorithm = get_selected_algorithm();
    algorithm(grid, rng);

    auto start = grid.cell_at(grid.rows() / 2, grid.columns() / 2);
    auto distances = start->distances();

    // setting -1 as initial index, so we can increment it to 0 in the update method
    distanced_cells = {distances.distanced_cells(), -1};

    grid.set_distances(distances);

    if (output_ascii)
        cout << grid << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (selected_grid.get() == "") {
        selected_grid = selected_grid_;
    }
    if (selected_algorithm.get() == "") {
        selected_algorithm = selected_algorithm_;
    }

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
        grid->draw(draw_cfg);
    } else {
        auto grid_cfg = draw_cfg;
        grid_cfg.draw_mode = DrawMode::Walls;
        grid->draw(grid_cfg);

        auto cells_cfg = draw_cfg;
        cells_cfg.draw_mode = DrawMode::BgColor;
        auto [cells, cells_batch] = distanced_cells;
        for (int i = 0; i < cells_batch; ++i)
            grid->draw_cells(cells.at(i), cells_cfg);
    }

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
    case 's':
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
        break;
    case 'r':
        // generate_color_grid();
        generate_grid();
        break;
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
