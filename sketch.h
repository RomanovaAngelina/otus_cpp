#pragma once

#include <iostream>
#include <string>

namespace tintenschaft {
    class Sketch {
    public:
        void newFile(std::string filename) {
            std::cout << "Sketch assigned to new file \"" << filename << '"' << std::endl;
            currentFilename = filename;
        }

        void loadFile(std::string filename) {
            std::cout << "Sketch loaded from file \"" << filename << '"' << std::endl;
            currentFilename = filename;
        }

        void saveFile() {
            std::cout << "Sketch saved to file \"" << currentFilename << '"' << std::endl;
        }

        void saveFileAs(std::string filename) {
            std::cout << "Sketch saved as file \"" << filename << '"' << std::endl;
            currentFilename = filename;
        }

    private:
        std::string currentFilename;
    };
}
