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
        struct UISettings {
            Vec2 pos;
            int width = 0;
            int maxChars = 0;
        };
        const HSV darkGray = HSV(0, 0, 0.2);
        const Font label = Font(20, Typeface::Medium);

        const Rect generateBG = Rect(600, 5, 195, 216);
        const UISettings generateLabel = { generateBG.pos + Vec2(52, 5) };
        const UISettings seedLabel = { generateBG.pos + Vec2(11, 35) };
        const UISettings seedTextBox = { generateBG.pos + Vec2(63, 71), 120, 8 };
        const UISettings loadButton = { generateBG.pos + Vec2(12, 120), 171 };
        const UISettings refreshButton = { generateBG.pos + Vec2(12, 168), 171 };

        const Rect exportBG = Rect(600, 226, 195, 353);
        const UISettings exportLabel = { exportBG.pos + Vec2(65, 5) };
        const UISettings baseLabel = { exportBG.pos + Vec2(11, 35) };
        const UISettings baseRadio = { exportBG.pos + Vec2(35, 61), 155 };
        const UISettings baseTextBox = { exportBG.pos + Vec2(89, 100), 94, 6 };
        const UISettings bgLabel = { exportBG.pos + Vec2(11, 142) };
        const UISettings sharpLabel = { exportBG.pos + Vec2(70, 182) };
        const UISettings bgTextBox = { exportBG.pos + Vec2(89, 178), 94, 6 };
        const UISettings sizeLabel = { exportBG.pos + Vec2(11, 220) };
        const UISettings pxLabel = { exportBG.pos + Vec2(160, 260) };
        const UISettings sizeTextBox = { exportBG.pos + Vec2(87, 256), 68, 4 };
        const UISettings saveButton = { exportBG.pos + Vec2(12, 305), 171 };

        const Font copyright = Font(10, Typeface::Regular);
        const UISettings copyrightLabel = { Vec2(727, 581) };

        bool textBox(TextEditState& tes, const UISettings& settings, bool enabled = true) {
            return SimpleGUI::TextBox(tes, settings.pos, settings.width, settings.maxChars, enabled);
        }
        bool button(const String& text, const UISettings& settings) {
            return SimpleGUI::Button(text, settings.pos, settings.width);
        }
        bool radioButtons(const Array<String>& options, const UISettings& settings) {
            return SimpleGUI::RadioButtons(baseIndex, options, settings.pos, settings.width);
        }

        void parseColor(HSV& targetColor, TextEditState& targetTes, double brightness);
        void updateSprites(Array<sprator::Sprite>& sprites);
        void updateSprites(Array<sprator::Sprite>& sprites, int growCount);

        void drawSprites(Array<sprator::Sprite>& sprites);
        void drawGenerateMenu(Array<sprator::Sprite>& sprites);
        void drawExportMenu(Array<sprator::Sprite>& sprites);

    public:
        Interface();
        int seed = Random(0, 99999999);
        void draw(Array<sprator::Sprite>& sprites);
    };
}
