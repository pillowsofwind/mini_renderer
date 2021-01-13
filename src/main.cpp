#include <iostream>

#include "scene_parser.hpp"
#include "render.hpp"

using namespace std;


int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/Final <input scene file> <output bmp file>" << endl;
        return 1;
    }
    char *inputFile = argv[1];
    char *outputFile = argv[2];

//    char *inputFile = "../testcases/scene10_sphere_copy.txt";
//    char *outputFile = "a.bmp";

    SceneParser *parser = new SceneParser(inputFile); // parse the scene

    Render r(parser,outputFile);

    r.render(1);

    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}


