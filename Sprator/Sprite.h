#pragma once
#include <Siv3D.hpp>

namespace sprator {
    class Sprite {
    private:
        struct Cell {
            bool previous : 1;
            bool current : 1;
        };

        Rect frame;
        int spriteSize = 10; // 4à»è„ÇÃãÙêî(é¿ópìIÇ»ÇÃÇÕ8,10,12Ç≠ÇÁÇ¢)
        Grid<Cell> spriteGrid = Grid<Cell>(spriteSize / 2, spriteSize);
        HSV rndColor;
        Image image;
        DynamicTexture texture = DynamicTexture(image);

        int countNeighborhood(int y, int x);
        void grow();

    public:
        Sprite(const Rect& _frame);

        void init(int growCount);
        void update(const HSV& baseColor, const HSV& bgColor);
        void draw() const;
        Image grid2image(const int exportSize, const HSV& baseColor, const HSV& bgColor);

        const Rect& getFrame() const { return frame; }
        const HSV& getColor() const { return rndColor; }
    };
}