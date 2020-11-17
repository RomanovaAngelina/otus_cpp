#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <string>

#include "sketch.h"
#include "shape.h"

namespace tintenschaft {
    void connect(std::function<void()>, std::function<void()>) {
        std::cout << "Connect signal" << std::endl;
    }

    void disconnect(std::function<void()>, std::function<void()>) {
        std::cout << "Disconnect signal" << std::endl;
    }

    class Widget {
    public:
        virtual ~Widget() { }
        virtual void show() = 0;
    };

    class Canvas: public Widget {
    public:
        class UnknownShape: std::runtime_error{
        public:
            UnknownShape(std::string const &what): std::runtime_error(what) { }
        };

        Sketch &getSketch() { return sketch; }

        void setBackgroundColor(Color color) { backgroundColor = color; }

        virtual void show() override {
            std::cout << "Show Canvas" << std::endl;
        }

        unsigned int addShape(std::shared_ptr<Shape> shape) {
            std::cout << "Add Shape to Canvas" << std::endl;
            shapes[currentUid] = shape;
            connect(
                    std::bind(&Shape::changeSignal, shape.get()),
                    std::bind(&Canvas::onShapeChangeSignal, this));
            shape->show();
            return currentUid++;
        }

        /// @throw Canvas::UnknownShape On uid that is unknown for Canvas.
        void deleteShape(unsigned int uid) {
            std::cout << "Delete Shape with uid " << uid << " from Canvas" << std::endl;
            try {
                disconnect(
                        std::bind(&Shape::changeSignal, shapes.at(uid).get()),
                        std::bind(&Canvas::onShapeChangeSignal, this));
            } catch (std::out_of_range &) {
                throw (UnknownShape("Shape for deletion is unknown for Canvas"));
            }
            shapes.erase(uid);
        }

        /// @throw Canvas::UnknownShape On uid that is unknown for Canvas.
        void deleteShape(std::shared_ptr<Shape> shape) {
            auto it {
                    std::find_if(
                            shapes.begin(),
                            shapes.end(),
                            [shape](auto const &item){ return item.second == shape; })
            };
            if (it != shapes.end()) {
                std::cout << "Delete Shape with uid " << it->first << " from Canvas" << std::endl;
                shapes.erase(it);
                disconnect(
                        std::bind(&Shape::changeSignal, shape.get()),
                        std::bind(&Canvas::onShapeChangeSignal, this));
            } else {
                std::cout << "Failed to delete Shape from Canvas" << std::endl;
                throw (UnknownShape("Shape for deletion is unknown for Canvas"));
            }
        }

        void onShapeChangeSignal() { }

    private:
        Sketch sketch { };
        Color backgroundColor { colors::WHITE };
        unsigned int currentUid { 1 };
        std::unordered_map<unsigned int, std::shared_ptr<Shape>> shapes;
    };

    class MainWindow: public Widget {
    public:
        MainWindow(): canvas() { }

        MainWindow(std::string title): canvas(), title(title) { }

        virtual void show() override {
            std::cout << "Show the MainWindow" << std::endl;
        }

        Canvas &getCanvas() { return canvas; }

    private:
        Canvas canvas;
        std::string title;
    };
}
