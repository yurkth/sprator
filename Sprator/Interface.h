#pragma once
#include <Siv3D.hpp>
#include "Sprite.h"

namespace sprator {
    class Interface {
    private:
        HSV baseColor = HSV(0, 0, 0.7); // #b3b3b3
        HSV bgColor = HSV(0, 0, 0.3); // #4d4d4d

        int selected = -1;
        int growCount = 2;
        TextEditState seedTes, baseTes, bgTes, sizeTes;
        size_t baseIndex = 0;
        int exportSize = 400;

        // UI
        const HSV darkGray = HSV(0, 0, 0.2);
        const Font label = Font(20, Typeface::Medium);
        const Font copyright = Font(10, Typeface::Regular);
        const Rect GenerationMenu = Rect(600, 5, 195, 216);
        const Rect ExportMenu = Rect(600, 226, 195, 353);

        void parseColor(HSV& targetColor, TextEditState& targetTes, double brightness);
        void updateSprites(Array<sprator::Sprite>& sprites, size_t baseIndex, HSV& baseColor, HSV& bgColor);
        void updateSprites(Array<sprator::Sprite>& sprites, size_t baseIndex, HSV& baseColor, HSV& bgColor, int growCount);

        void drawSprites(Array<sprator::Sprite>& sprites);
        void drawGenerateMenu(Array<sprator::Sprite>& sprites);
        void drawExportMenu(Array<sprator::Sprite>& sprites);
    public:
        Interface();
        int seed = Random(0, 99999999);
        void draw(Array<sprator::Sprite>& sprites);
    };
}
