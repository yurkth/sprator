#include "Interface.h"

namespace sprator {
    namespace detail {
        Menu& Menu::addLabel(const String& text, const Point& offset) {
            labels_ << Label(text, offset);
            return *this;
        }

        void Menu::draw() const {
            background_.draw(Palette::White).drawFrame(3, 0, color_);
            for (const auto& label : labels_) {
                font_(label.text).draw(background_.pos + label.pos, color_);
            }
        }
    }

    void Interface::parseColor(HSV& targetColor, const TextEditState& targetTes, double brightness) const {
        targetColor = HSV(ParseOr<Color>(U"#{}"_fmt(targetTes.text), HSV(0, 0, brightness)));
    }

    void Interface::updateSprites() {
        for (auto& sprite : sprites_) {
            sprite.update(gui_.radioButtons(U"rb-BaseColor") ? baseColor_ : sprite.getColor(), bgColor_);
        }
    }

    void Interface::regenerateSprites() {
        Reseed(seed_);
        for (auto& sprite : sprites_) {
            sprite.init(growCount_);
            sprite.update(gui_.radioButtons(U"rb-BaseColor") ? baseColor_ : sprite.getColor(), bgColor_);
        }
    }

    Interface::Interface() {
        Reseed(seed_);
        std::get<SimpleGUIWidget::TextBox>(gui_.widget(U"tb-Seed").widget).state.text = Format(seed_);
        for (auto i : step(49)) {
            sprites_ << Sprite(Rect(5 + 85 * (i % 7), 5 + 85 * (i / 7), 80));
        }
        const_cast<Array<SimpleGUI::Widget>&>(gui_.widgets()).reverse();
        generateMenu
            .addLabel(U"Generate", Point(52, 5))
            .addLabel(U"Seed", Point(11, 35));
        exportMenu
            .addLabel(U"Export", Point(65, 5))
            .addLabel(U"Base Color", Point(11, 35))
            .addLabel(U"#", Point(70, 182))
            .addLabel(U"BG Color", Point(11, 142))
            .addLabel(U"Size", Point(11, 220))
            .addLabel(U"px", Point(160, 260));
    }

    void Interface::update() {
        // sprites
        for (const auto& [i, sprite] : Indexed(sprites_)) {
            if (sprite.getFrame().leftClicked()) {
                selected_ = i;
            }
        }
        if (MouseR.up()) {
            growCount_ = growCount_ % 3 + 1;
            Window::SetTitle(U"Sprator | grow: {}"_fmt(growCount_));
            regenerateSprites();
        }

        // generate menu
        if (gui_.button(U"bt-Load")) {
            seed_ = ParseOr<int>(gui_.textBox(U"tb-Seed").text, Random(0, 99999999));
            std::get<SimpleGUIWidget::TextBox>(gui_.widget(U"tb-Seed").widget).state.text = Format(seed_);
            regenerateSprites();
            selected_ = -1;
        }
        if (gui_.button(U"bt-Refresh")) {
            seed_ = Random(0, 99999999);
            std::get<SimpleGUIWidget::TextBox>(gui_.widget(U"tb-Seed").widget).state.text = Format(seed_);
            regenerateSprites();
            selected_ = -1;
        }

        // export menu
        if (gui_.hasChanged(U"rb-BaseColor")) {
            std::get<SimpleGUIWidget::TextBox>(gui_.widget(U"tb-BaseColor").widget).enabled = gui_.radioButtons(U"rb-BaseColor");
            updateSprites();
        }
        if (gui_.hasChanged(U"tb-BaseColor")) {
            parseColor(baseColor_, gui_.textBox(U"tb-BaseColor"), 0.7);
            updateSprites();
        }
        if (gui_.hasChanged(U"tb-BgColor")) {
            parseColor(bgColor_, gui_.textBox(U"tb-BgColor"), 0.3);
            updateSprites();
        }
        if (gui_.button(U"bt-Save")) {
            const size_t exportSize = ParseOr<int>(gui_.textBox(U"tb-Size").text, 0);
            if (16 <= exportSize && exportSize <= 8192 && selected_ != -1) {
                // ダイアログを開いて画像を保存
                sprites_[selected_].grid2image(exportSize, gui_.radioButtons(U"rb-BaseColor") ? baseColor_ : sprites_[selected_].getColor(), bgColor_)
                    .savePNG(Dialog::SaveFile({ FileFilter::PNG() }).value_or(U""));
            }
            else if (selected_ == -1) {
                System::ShowMessageBox(U"Please select the sprite.", MessageBoxStyle::Error);
            }
            else {
                System::ShowMessageBox(U"Size must be 16 or more and 8192 or less.", MessageBoxStyle::Error);
            }
        }
    }

    void Interface::draw() const {
        // sprites
        for (const auto& sprite : sprites_) {
            sprite.draw();
        }
        if (selected_ != -1) {
            sprites_[selected_].getFrame().drawFrame(3, 0, Color(89, 178, 255));
        }

        // UI
        generateMenu.draw();
        exportMenu.draw();
        copyright_(U"Sprator 0.1.1").draw(Point(727, 581), Palette::White);
        gui_.draw();
    }
}