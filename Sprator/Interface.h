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
        size_t baseIndex = 0;
        int seed = Random(0, 99999999);

        // UI
        const HSV darkGray = HSV(0, 0, 0.2);
        const Font label = Font(20, Typeface::Medium);
        const Font copyright = Font(10, Typeface::Regular);

        const Rect generateBG = Rect(600, 5, 195, 216);
        const Rect exportBG = Rect(600, 226, 195, 353);
        SimpleGUIManager gui = SimpleGUIManager(Resource(U"gui.toml"));

        void parseColor(HSV& targetColor, const TextEditState& targetTes, double brightness) const;
        void updateSprites(Array<sprator::Sprite>& sprites);
        void updateSprites(Array<sprator::Sprite>& sprites, int growCount);

        void drawSprites(Array<sprator::Sprite>& sprites);
        void drawGenerateMenu(Array<sprator::Sprite>& sprites);
        void drawExportMenu(Array<sprator::Sprite>& sprites);

    public:
        Interface();
        void draw(Array<sprator::Sprite>& sprites);
    };
}
