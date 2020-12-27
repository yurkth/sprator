#include "Interface.h"

namespace sprator {
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

        // generate menu
        const Rect generateBG = Rect(600, 5, 195, 216);
        generateBG.draw(Palette::White).drawFrame(3, 0, darkGray_);
        label_(U"Generate").draw(generateBG.pos + Point(52, 5), darkGray_);
        label_(U"Seed").draw(generateBG.pos + Point(11, 35), darkGray_);

        // export menu
        const Rect exportBG = Rect(600, 226, 195, 353);
        exportBG.draw(Palette::White).drawFrame(3, 0, darkGray_);
        label_(U"Export").draw(exportBG.pos + Point(65, 5), darkGray_);
        label_(U"Base Color").draw(exportBG.pos + Point(11, 35), darkGray_);
        label_(U"#").draw(exportBG.pos + Point(70, 182), darkGray_);
        label_(U"BG Color").draw(exportBG.pos + Point(11, 142), darkGray_);
        label_(U"Size").draw(exportBG.pos + Point(11, 220), darkGray_);
        label_(U"px").draw(exportBG.pos + Point(160, 260), darkGray_);

        copyright_(U"Sprator 0.1.1").draw(Point(727, 581), Palette::White);

        // GUI
        gui_.draw();
    }
}