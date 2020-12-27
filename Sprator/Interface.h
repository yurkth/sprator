#pragma once
#include <Siv3D.hpp>
#include "Sprite.h"

namespace sprator {
    class Interface {
    private:
        Array<sprator::Sprite> sprites_;

        int8 selected_ = -1;
        uint8 growCount_ = 2;
        size_t seed_ = Random(0, 99999999);

        HSV baseColor_ = HSV(0, 0, 0.7); // #b3b3b3
        HSV bgColor_ = HSV(0, 0, 0.3); // #4d4d4d

        const HSV darkGray_ = HSV(0, 0, 0.2);
        const Font label_ = Font(20, Typeface::Medium);
        const Font copyright_ = Font(10, Typeface::Regular);

        SimpleGUIManager gui_ = SimpleGUIManager(Resource(U"gui.toml"));

        void parseColor(HSV& targetColor, const TextEditState& targetTes, double brightness) const;
        void updateSprites();
        void regenerateSprites();

    public:
        Interface();
        void update();
        void draw() const;
    };
}
