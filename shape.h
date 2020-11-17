#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <memory>
#include <tuple>
#include <vector>

namespace tintenschaft {
    using byte = uint8_t;

    class Color;
    std::ostream &operator <<(std::ostream &s, Color);

    class Color {
    public:
        Color(byte red, byte green, byte blue, byte alpha = 255):
                red(red), green(green), blue(blue), alpha(alpha) {
            std::cout << "Construct color " << *this << " by integer values" << std::endl;
        }

        Color(std::string htmlCode, byte alpha = 255): alpha(alpha) {
            std::cout << "Construct color " << *this << " by HTML code" << std::endl;
            std::tie(red, green, blue) = html2int(htmlCode);
        }

        operator std::string() {
            return std::string(
                    "RGBA(" +
                    std::to_string(red) + ", " +
                    std::to_string(green) + ", " +
                    std::to_string(blue) + ", " +
                    std::to_string(alpha) + ')');
        }

    private:
        byte
                red,
                green,
                blue,
                alpha;

        std::tuple<byte, byte, byte> html2int(std::string htmlCode) {
            std::cout << "Convert HMTL color code to integer values" << std::endl;
            return std::make_tuple(0, 0, 0);
        }
    };

    std::ostream &operator <<(std::ostream &s, Color color) {
        s << std::string(color);
        return s;
    }

    namespace colors {
        Color
                NONE { 0, 0, 0, 0 },
                BLACK { 0, 0, 0 },
                RED { 255, 0, 0 },
                GREEN { 0, 255, 0 },
                BLUE { 0, 0, 255 },
                WHITE { 255, 255, 255 };
    }

    struct Coordinate { long x, y; };

    class Shape {
    public:
        virtual ~Shape() {
            std::cout << "Destruct Shape" << std::endl;
        };

        virtual void show() = 0;

        virtual void setStrokeColor(Color color) {
            std::cout << "Setting shape stroke color to " << color << std::endl;
            strokeColor = color;
            changeSignal();
        }

        virtual void setStrokeWidth(unsigned int widthPX) {
            std::cout << "Setting shape stroke width to " << widthPX << std::endl;
            strokeWidth = widthPX;
            changeSignal();
        }

        virtual void setFillColor(Color color) {
            std::cout << "Setting shape fill color to " << color << std::endl;
            fillColor = color;
            changeSignal();
        }

        virtual void changeSignal() {
            std::cout << "Shape has emitted changeSignal";
        }

    protected:
        Color strokeColor { colors::BLACK };
        Color fillColor { colors::NONE };
        unsigned int strokeWidth { 1 };
    };

    class Dot: public Shape {
    public:
        Dot(Coordinate coordinate) {
            std::cout << "Construct Dot" << std::endl;
        }

        Dot(
                Coordinate coordinate,
                Color strokeColor) {
            std::cout
                    << "Construct Dot with strokeColor " << strokeColor << std::endl;
            Shape::strokeColor = strokeColor;
        }

        static std::shared_ptr<Dot> create(Coordinate coordinate) {
            return std::make_shared<Dot>(Dot(coordinate));
        }

        static std::shared_ptr<Dot> create(
                Coordinate coordinate,
                Color strokeColor) {
            return std::make_shared<Dot>(Dot(coordinate, strokeColor));
        }

        virtual void show() override { std::cout << "Show Dot" << std::endl; }
    };

    class Rectangle: public Shape {
    public:
        Rectangle(Coordinate corner1, Coordinate corner2) {
            std::cout << "Construct Rectangle" << std::endl;
        }

        Rectangle(
                Coordinate corner1,
                Coordinate corner2,
                Color strokeColor,
                Color fillColor) {
            std::cout
                    << "Construct Rectangle with strokeColor " << strokeColor
                    << ", fillColor " << fillColor << std::endl;
            Shape::strokeColor = strokeColor;
            Shape::fillColor = fillColor;
        }

        static std::shared_ptr<Rectangle> create(Coordinate corner1, Coordinate corner2) {
            return std::make_shared<Rectangle>(Rectangle(corner1, corner2));
        }

        static std::shared_ptr<Rectangle> create(
                Coordinate corner1,
                Coordinate corner2,
                Color strokeColor,
                Color fillColor) {
            return std::make_shared<Rectangle>(
                    Rectangle(corner1, corner2, strokeColor, fillColor));
        }

        virtual void show() override { std::cout << "Show Rectangle" << std::endl; }
    };

    class Ellipse: public Shape {
    public:
        Ellipse(Coordinate corner1, Coordinate corner2) {
            std::cout << "Construct Ellipse" << std::endl;
        }

        Ellipse(
                Coordinate corner1,
                Coordinate corner2,
                Color strokeColor,
                Color fillColor) {
            std::cout
                    << "Construct Ellipse with strokeColor " << strokeColor
                    << ", fillColor " << fillColor << std::endl;
            Shape::strokeColor = strokeColor;
            Shape::fillColor = fillColor;
        }

        static std::shared_ptr<Ellipse> create(Coordinate corner1, Coordinate corner2) {
            return std::make_shared<Ellipse>(Ellipse(corner1, corner2));
        }

        static std::shared_ptr<Ellipse> create(
                Coordinate corner1,
                Coordinate corner2,
                Color strokeColor,
                Color fillColor) {
            return std::make_shared<Ellipse>(
                    Ellipse(corner1, corner2, strokeColor, fillColor));
        }

        virtual void show() override { std::cout << "Show Ellipse" << std::endl; }
    };

    class Polyline: public Shape {
    public:
        Polyline(std::vector<Coordinate> coordinates) {
            std::cout << "Construct Polyline" << std::endl;
        }

        Polyline(
                std::vector<Coordinate> coordinates,
                Color strokeColor,
                Color fillColor) {
            std::cout
                    << "Construct Polyline with strokeColor " << strokeColor
                    << ", fillColor " << fillColor << std::endl;
            Shape::strokeColor = strokeColor;
            Shape::fillColor = fillColor;
        }

        static std::shared_ptr<Polyline> create(std::vector<Coordinate> coordinates) {
            return std::make_shared<Polyline>(Polyline(coordinates));
        }

        static std::shared_ptr<Polyline> create(
                std::vector<Coordinate> coordinates,
                Color strokeColor,
                Color fillColor) {
            return std::make_shared<Polyline>(
                    Polyline(coordinates, strokeColor, fillColor));
        }

        virtual void show() override { std::cout << "Show Polyline" << std::endl; }
    };
}
