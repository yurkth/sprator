#include "Interface.h"

void sprator::Interface::parseColor(HSV& targetColor, TextEditState& targetTes, double brightness) {
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
    label(U"Generate").draw(generateLabel.pos, darkGray);
    label(U"Seed").draw(seedLabel.pos, darkGray);
    if (textBox(seedTes, seedTextBox)) {
        seed = ParseOr<int>(seedTes.text, Random(0, 99999999));
    }
    if (button(U"Load", loadButton)) {
        seedTes.text = Format(seed);
        updateSprites(sprites, growCount);
        selected = -1;
    }
    if (bool a = button(U"Refresh", refreshButton)) {
        seed = Random(0, 99999999);
        seedTes.text = Format(seed);
        updateSprites(sprites, growCount);
        selected = -1;
    }
}

void sprator::Interface::drawExportMenu(Array<sprator::Sprite>& sprites) {
    exportBG.draw(Palette::White).drawFrame(3, 0, darkGray);
    label(U"Export").draw(exportLabel.pos, darkGray);
    label(U"Base Color").draw(baseLabel.pos, darkGray);
    if (radioButtons({ U"Random", U"#" }, baseRadio)) {
        updateSprites(sprites);
    }
    if (textBox(baseTes, baseTextBox, baseIndex)) {
        parseColor(baseColor, baseTes, 0.7);
        updateSprites(sprites);
    }

    label(U"BG Color").draw(bgLabel.pos, darkGray);
    label(U"#").draw(sharpLabel.pos, darkGray);
    if (textBox(bgTes, bgTextBox)) {
        parseColor(bgColor, bgTes, 0.3);
        updateSprites(sprites);
    }

    label(U"Size").draw(sizeLabel.pos, darkGray);
    label(U"px").draw(pxLabel.pos, darkGray);
    if (textBox(sizeTes, sizeTextBox)) {
        exportSize = ParseOr<int>(sizeTes.text, 0);
    }
    if (button(U"Save", saveButton)) {
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
    seedTes.text = Format(seed);
    baseTes.text = U"b3b3b3";
    bgTes.text = U"4d4d4d";
    sizeTes.text = U"400";
}

void sprator::Interface::draw(Array<sprator::Sprite>& sprites) {
    drawSprites(sprites);
    drawGenerateMenu(sprites);
    drawExportMenu(sprites);
    copyright(U"Sprator 0.1.1").draw(copyrightLabel.pos, Palette::White);
}