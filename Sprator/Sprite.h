#pragma once
#include <Siv3D.hpp>

namespace sprator {
    class Sprite {
    private:
        struct Cell {
            bool previous : 1;
            bool current : 1;
        };

        Rect frame_;
        const size_t spriteSize_ = 10; // 4以上の偶数(実用的なのは8,10,12くらい)
        Grid<Cell> spriteGrid_ = Grid<Cell>(spriteSize_ / 2, spriteSize_);
        HSV rndColor_;
        Image image_;
        DynamicTexture texture_ = DynamicTexture(image_);

        size_t countNeighborhood(size_t y, size_t x);
        void grow();

    public:
        Sprite(const Rect& frame);

        void init(uint8 growCount);
        void update(const HSV& baseColor, const HSV& bgColor);
        void draw() const;
        Image grid2image(const size_t exportSize, const HSV& baseColor, const HSV& bgColor);

        const Rect& frame() const { return frame_; }
        const HSV& color() const { return rndColor_; }
    };
}