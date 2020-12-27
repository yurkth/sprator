#include "Interface.h"

void sprator::Interface::parseColor(HSV& targetColor, const TextEditState& targetTes, double brightness) const {
    targetColor = HSV(ParseOr<Color>(U"#{}"_fmt(targetTes.text), HSV(0, 0, brightness)));
}

void sprator::Interface::updateSprites(Array<sprator::Sprite>& sprites) {
    for (auto& sprite : sprites) {
        sprite.update(baseIndex ? baseColor : sprite.getColor(), bgColor);
    }
}

void sprator::Interface::updateSprites(Array<sprator::Sprite>& sprites, int growCount) {
    Reseed(seed);
    for (auto& sprite : sprites) {
        sprite.init(growCount);
        sprite.update(baseIndex ? baseColor : sprite.getColor(), bgColor);
    }
}

void sprator::Interface::drawSprites(Array<sprator::Sprite>& sprites) {
    for (const auto& sprite : sprites) {
        sprite.draw();
    }
    for (const auto& [i, sprite] : Indexed(sprites)) {
        if (sprite.getFrame().leftClicked()) {
            selected = i;
        }
    }
    if (selected != -1) {
        sprites[selected].getFrame().drawFrame(3, 0, Color(89, 178, 255));
    }
    if (MouseR.up()) {
        growCount = growCount % 3 + 1;
        Window::SetTitle(U"Sprator | grow: {}"_fmt(growCount));
        updateSprites(sprites, growCount);
    }
}

void sprator::Interface::drawGenerateMenu(Array<sprator::Sprite>& sprites) {
    generateBG.draw(Palette::White).drawFrame(3, 0, darkGray);
    label(U"Generate").draw(generateBG.pos + Point(52, 5), darkGray);
    label(U"Seed").draw(generateBG.pos + Point(11, 35), darkGray);
    if (gui.button(U"bt-Load")) {
        seed = ParseOr<int>(gui.textBox(U"tb-Seed").text, Random(0, 99999999));
        std::get<SimpleGUIWidget::TextBox>(gui.widget(U"tb-Seed").widget).state.text = Format(seed);
        updateSprites(sprites, growCount);
        selected = -1;
    }
    if (gui.button(U"bt-Refresh")) {
        seed = Random(0, 99999999);
        std::get<SimpleGUIWidget::TextBox>(gui.widget(U"tb-Seed").widget).state.text = Format(seed);
        updateSprites(sprites, growCount);
        selected = -1;
    }
}

void sprator::Interface::drawExportMenu(Array<sprator::Sprite>& sprites) {
    exportBG.draw(Palette::White).drawFrame(3, 0, darkGray);
    label(U"Export").draw(exportBG.pos + Point(65, 5), darkGray);
    label(U"Base Color").draw(exportBG.pos + Point(11, 35), darkGray);
    if (gui.hasChanged(U"rb-BaseColor")) {
        baseIndex = gui.radioButtons(U"rb-BaseColor");
        std::get<SimpleGUIWidget::TextBox>(gui.widget(U"tb-BaseColor").widget).enabled = baseIndex;
        updateSprites(sprites);
    }
    if (gui.hasChanged(U"tb-BaseColor")) {
        parseColor(baseColor, gui.textBox(U"tb-BaseColor"), 0.7);
        updateSprites(sprites);
    }

    label(U"BG Color").draw(exportBG.pos + Point(11, 142), darkGray);
    label(U"#").draw(exportBG.pos + Point(70, 182), darkGray);
    if (gui.hasChanged(U"tb-BgColor")) {
        parseColor(bgColor, gui.textBox(U"tb-BgColor"), 0.3);
        updateSprites(sprites);
    }

    label(U"Size").draw(exportBG.pos + Point(11, 220), darkGray);
    label(U"px").draw(exportBG.pos + Point(160, 260), darkGray);
    if (gui.button(U"bt-Save")) {
        const int exportSize = ParseOr<int>(gui.textBox(U"tb-Size").text, 0);
        if (16 <= exportSize && exportSize <= 8192 && selected != -1) {
            // ダイアログを開いて画像を保存
            sprites[selected].grid2image(exportSize, baseIndex ? baseColor : sprites[selected].getColor(), bgColor)
                .savePNG(Dialog::SaveFile({ FileFilter::PNG() }).value_or(U""));
        }
        else if (selected == -1) {
            System::ShowMessageBox(U"Please select the sprite.", MessageBoxStyle::Error);
        }
        else {
            System::ShowMessageBox(U"Size must be 16 or more and 8192 or less.", MessageBoxStyle::Error);
        }
    }
}

sprator::Interface::Interface() {
    Reseed(seed);
    const_cast<Array<SimpleGUI::Widget>&>(gui.widgets()).reverse();
    std::get<SimpleGUIWidget::TextBox>(gui.widget(U"tb-Seed").widget).state.text = Format(seed);
}

void sprator::Interface::draw(Array<sprator::Sprite>& sprites) {
    drawSprites(sprites);
    drawGenerateMenu(sprites);
    drawExportMenu(sprites);
    gui.draw();
    copyright(U"Sprator 0.1.1").draw(Point(727, 581), Palette::White);
}