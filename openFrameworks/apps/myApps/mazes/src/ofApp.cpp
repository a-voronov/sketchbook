#include "ofApp.h"

#include "distanceGrid.h"
#include "colorGrid.h"
#include "maskedGrid.h"
#include "gridVisitor.h"

#include "binaryTree.h"
#include "sidewinder.h"
#include "aldousBroder.h"
#include "wilsons.h"
#include "huntAndKill.h"
#include "recursiveBacktracker.h"

const static int frame_rate = 60;

const static vector<pair<string, AlgorithmCtor>> all_algorithms{
    {"BinaryTree",           [](Grid& g, std::mt19937& rng) { BinaryTree::on(g, rng); }},
    {"Sidewinder",           [](Grid& g, std::mt19937& rng) { Sidewinder::on(g, rng); }},
    {"AldousBroder",         [](Grid& g, std::mt19937& rng) { AldousBroder::on(g, rng); }},
    {"Wilsons",              [](Grid& g, std::mt19937& rng) { Wilsons::on(g, rng); }},
    {"HuntAndKill",          [](Grid& g, std::mt19937& rng) { HuntAndKill::on(g, rng); }},
    {"RecursiveBacktracker", [](Grid& g, std::mt19937& rng) { RecursiveBacktracker::on(g, rng); }},
};

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(frame_rate);
    ofSetVerticalSync(true);

    rng = std::mt19937{std::random_device{}()};
    // rng = std::mt19937{42};

    all_grids = {
        {"DistanceGrid", [this]() {
            return make_unique<DistanceGrid>(rows, columns);
        }},
        {"ColorGrid", [this]() {
            return make_unique<ColorGrid>(rows, columns);
        }},
        {"MaskedGrid", [this]() -> unique_ptr<MaskedGrid> {
            if (mask_path_.empty())
                pick_mask();
            if (mask_path_.empty())
                return nullptr;

            auto extension = ofGetExtensionLower(mask_path_);
            unique_ptr<Mask> mask = nullptr;
            if (extension == ".txt") {
                mask = Mask::from_txt(mask_path_);
            } else if (extension == ".png") {
                mask = Mask::from_png(mask_path_);
            }
            if (!mask)
                return nullptr;

            rows = mask->rows();
            columns = mask->columns();
            return make_unique<MaskedGrid>(std::move(mask));
        }}
    };

    gui.setup();
    gui.add(rows.setup("rows", 56, 2, 100));
    gui.add(columns.setup("columns", 84, 2, 100));
    gui.add(tone_color.set("tone color", ofColor::lightGreen, ofColor(0, 0, 0), ofColor(255 , 255, 255)));
    gui.add(picked_intensity_stretch.setup("intensity", 0.75f, 0.0f, 1.0f));
    gui.add(animation_speed.setup("animation speed", 4, 0, frame_rate));
    // only affects mazes drawn on screen
    gui.add(output_ascii.setup("ASCII", false));
    gui.add(repeat.setup("repeat", true));
    gui.add(mask_picker.setup("pick mask", 200, 18));
    mask_picker.addListener(this, &ofApp::pick_mask);

    gui.add(show_deadends.setup("deadends", false));
    gui.add(deadends_color.set("deadends color", ofColor::silver, ofColor(0, 0, 0), ofColor(255 , 255, 255)));

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

    listeners.unsubscribeAll();
    selected_algorithm_ = selected_algorithm = all_algorithms.at(5).first;
    selected_grid_ = selected_grid = all_grids.at(1).first;
    listeners.push(selected_algorithm.newListener([&](const string& param) {
        algorithms_dropdown->hideDropdown();
        // ofxDropdown allows deselection
        // and reverting ofOption inside of the listener lambda doesn't trigger UI redraw no matter what,
        // hence we rely on backup local values to restore previously selected value inside `update` method
        if (param == "")
            return;
        if (selected_algorithm_ != param && generate_grid())
            selected_algorithm_ = param;
    }));
    listeners.push(selected_grid.newListener([&](const string& param) {
        grids_dropdown->hideDropdown();
        if (param == "")
            return;
        if (selected_grid_attempt_ != param && selected_grid_ != param && generate_grid())
            selected_grid_ = param;
        selected_grid_attempt_ = param;
    }));

    generate_grid();
}

//--------------------------------------------------------------
bool ofApp::generate_grid() {
    distanced_cells = {};
    deadends = {};

    auto& grid_ctor = get_selected_grid();
    if (auto new_grid = grid_ctor()) {
        grid = std::move(new_grid);
        grid->accept(*this, rng);
        deadends = grid->deadends();
        return true;
    }
    return false;
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

void ofApp::pick_mask() {
    auto of_result = ofSystemLoadDialog("Select a txt or png file", false, filesystem::current_path().string() + "/../../../");
    if (!of_result.bSuccess)
        return;

    auto extension = ofGetExtensionLower(of_result.filePath);
    if (extension == ".txt" || extension == ".png")
        mask_path_ = of_result.filePath;

    ofGetWindowPtr()->makeCurrent();
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

    distanced_cells = {path.distanced_cells(), 0};
    grid.set_distances(path);

    if (output_ascii)
        cout << grid << endl;
}

void ofApp::visit(ColorGrid& grid, std::mt19937& rng) {
    auto& algorithm = get_selected_algorithm();
    algorithm(grid, rng);

    auto start = grid.cell_at(grid.rows() / 2, grid.columns() / 2);
    auto distances = start->distances();

    distanced_cells = {distances.distanced_cells(), 0};
    grid.set_distances(distances);

    if (output_ascii)
        cout << grid << endl;
}

void ofApp::visit(MaskedGrid& grid, std::mt19937& rng) {
    auto& algorithm = get_selected_algorithm();
    algorithm(grid, rng);

    auto start = grid.cell_at(grid.rows() / 2, grid.columns() / 2);
    if (!start)
        start = grid.random_cell(rng);
    auto distances = start->distances();

    distanced_cells = {distances.distanced_cells(), 0};
    grid.set_distances(distances);

    if (output_ascii)
        cout << grid << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (selected_grid.get() != selected_grid_) {
        selected_grid = selected_grid_;
    }
    if (selected_algorithm.get() != selected_algorithm_) {
        selected_algorithm = selected_algorithm_;
    }

    if (!distanced_cells.first.empty()) {
        int update_speed = int(frame_rate / animation_speed);
        if (ofGetFrameNum() % update_speed == 0) {
            if (distanced_cells.second < distanced_cells.first.size()) {
                ++distanced_cells.second;
            } else if (repeat) {
                distanced_cells.second = 0;
            }
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
        .tone = tone_color,
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

        if (!distanced_cells.first.empty()) {
            auto cells_cfg = draw_cfg;
            cells_cfg.draw_mode = DrawMode::BgColor;
            auto [cells, cells_batch] = distanced_cells;
            for (int i = 0; i < cells_batch; ++i)
                grid->draw_cells(cells.at(i), cells_cfg);
        }
    }

    if (show_deadends) {
        auto deadends_cfg = draw_cfg;
        deadends_cfg.draw_mode = DrawMode::BgColor;
        deadends_cfg.tone = deadends_color;
        grid->draw_cells(deadends, deadends_cfg);
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
