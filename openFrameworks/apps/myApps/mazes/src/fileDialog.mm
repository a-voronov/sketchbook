#include <Foundation/Foundation.h>

#include "fileDialog.hh"

using namespace std;

std::string pick_file(
    std::string default_dir,
    std::vector<std::string> filters
) {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setLevel: CGShieldingWindowLevel()];
    [panel setAllowsMultipleSelection: FALSE];
    [panel setCanChooseFiles: YES];

    NSMutableArray *fileTypes = [NSMutableArray array];
    for (auto filter : filters) {
        NSString *fileType = [NSString stringWithUTF8String: filter.c_str()];
        [fileTypes addObject: fileType];
    }
    [panel setAllowedFileTypes: fileTypes];

    if (!default_dir.empty()) {
        [panel setDirectoryURL: [NSURL fileURLWithPath: [NSString stringWithUTF8String: default_dir.c_str()]]];
    }

    NSString *urlPath = @"";
    if ([panel runModal] == NSModalResponseOK) {
        NSURL *url = [[panel URLs] objectAtIndex: 0];
        urlPath = [url path];
    }
    return std::string([urlPath UTF8String]);
}
