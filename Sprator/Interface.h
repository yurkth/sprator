#pragma once
#include <Siv3D.hpp>
#include "Sprite.h"

namespace sprator {
    namespace detail {
        class Menu {
        private:
            struct Label {
                String text;
                Point pos;
                Label(const String& _text, const Point& _pos)
                    : text(_text)
                    , pos(_pos) {}
            };

            Rect background_;
            Array<Label> labels_;

            const Font font_ = Font(20, Typeface::Medium);
            const HSV color_ = HSV(0, 0, 0.2);

        public:
            Menu(const Point& pos, const Size& size)
                : background_(Rect(pos, size)) {}

            Menu& addLabel(const String& text, const Point& offset);

            void draw() const;
        };
    }

    class Interface {
    private:
        Array<sprator::Sprite> sprites_;

        int8 selected_ = -1;
        uint8 growCount_ = 2;
        size_t seed_ = Random(0, 99999999);

        HSV baseColor_ = HSV(0, 0, 0.7); // #b3b3b3
        HSV bgColor_ = HSV(0, 0, 0.3); // #4d4d4d

        detail::Menu generateMenu = detail::Menu(Point(600, 5), Size(195, 216));
        detail::Menu exportMenu = detail::Menu(Point(600, 226), Size(195, 353));
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
