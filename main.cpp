#include <memory>

#include "main_window.h"

using namespace tintenschaft;

int main() {
    MainWindow mainWindow { };
    mainWindow.show();

    auto &canvas { mainWindow.getCanvas() };

    auto &doc { canvas.getSketch() };
    doc.newFile("Mono Lisa.svg");

    canvas.setBackgroundColor(Color(colors::WHITE));
    auto face { Ellipse::create({20, 0}, {80, 100}) };
    auto leftEye { Ellipse::create({30, 30}, {40, 40}) };
    auto rightEye { Ellipse::create({60, 30}, {70, 40}) };

    canvas.addShape(face);
    canvas.addShape(leftEye);
    auto rightEyeUid { canvas.addShape(rightEye) };

    canvas.deleteShape(rightEyeUid);

    doc.saveFile();
}