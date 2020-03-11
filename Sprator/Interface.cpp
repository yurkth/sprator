#include "Interface.h"

void sprator::Interface::parseColor(HSV& targetColor, TextEditState& targetTes, double brightness) {
    targetColor = HSV(ParseOr<Color>(U"#{}"_fmt(targetTes.text), HSV(0, 0, brightness)));
}

void sprator::Interface::updateSprites(Array<sprator::Sprite>& sprites, size_t baseIndex, HSV& baseColor, HSV& bgColor) {
    for (auto& sprite : sprites) {
        sprite.update(baseIndex ? baseColor : sprite.getColor(), bgColor);
    }
}

void sprator::Interface::updateSprites(Array<sprator::Sprite>& sprites, size_t baseIndex, HSV& baseColor, HSV& bgColor, int growCount) {
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
        updateSprites(sprites, baseIndex, baseColor, bgColor, growCount);
    }
}

void sprator::Interface::drawGenerateMenu(Array<sprator::Sprite>& sprites) {
    GenerationMenu.draw(Palette::White).drawFrame(3, 0, darkGray);
    label(U"Generate").draw(GenerationMenu.x + 52, GenerationMenu.y + 5, darkGray);
    label(U"Seed").draw(GenerationMenu.x + 11, GenerationMenu.y + 35, darkGray);
    if (SimpleGUI::TextBox(seedTes, GenerationMenu.pos + Vec2(63, 71), 120, 8)) {
        seed = ParseOr<int>(seedTes.text, Random(0, 99999999));
    }
    if (SimpleGUI::Button(U"Load", GenerationMenu.pos + Vec2(12, 120), 171)) {
        seedTes.text = Format(seed);
        updateSprites(sprites, baseIndex, baseColor, bgColor, growCount);
        selected = -1;
    }
    if (bool a = SimpleGUI::Button(U"Refresh", GenerationMenu.pos + Vec2(12, 168), 171)) {
        seed = Random(0, 99999999);
        seedTes.text = Format(seed);
        updateSprites(sprites, baseIndex, baseColor, bgColor, growCount);
        selected = -1;
    }
}

void sprator::Interface::drawExportMenu(Array<sprator::Sprite>& sprites) {
    ExportMenu.draw(Palette::White).drawFrame(3, 0, darkGray);
    label(U"Export").draw(ExportMenu.x + 65, ExportMenu.y + 5, darkGray);
    label(U"Base Color").draw(ExportMenu.x + 11, ExportMenu.y + 35, darkGray);
    if (SimpleGUI::RadioButtons(baseIndex, { U"Random", U"#" }, ExportMenu.pos + Vec2(35, 61), 155)) {
        updateSprites(sprites, baseIndex, baseColor, bgColor);
    }
    if (SimpleGUI::TextBox(baseTes, ExportMenu.pos + Vec2(89, 100), 94, 6, baseIndex)) {
        parseColor(baseColor, baseTes, 0.7);
        updateSprites(sprites, baseIndex, baseColor, bgColor);
    }

    label(U"BG Color").draw(ExportMenu.x + 11, ExportMenu.y + 142, darkGray);
    label(U"#").draw(ExportMenu.x + 70, ExportMenu.y + 182, darkGray);
    if (SimpleGUI::TextBox(bgTes, ExportMenu.pos + Vec2(89, 178), 94, 6)) {
        parseColor(bgColor, bgTes, 0.3);
        updateSprites(sprites, baseIndex, baseColor, bgColor);
    }

    label(U"Size").draw(ExportMenu.x + 11, ExportMenu.y + 220, darkGray);
    label(U"px").draw(ExportMenu.x + 160, ExportMenu.y + 260, darkGray);
    if (SimpleGUI::TextBox(sizeTes, ExportMenu.pos + Vec2(87, 256), 68, 4)) {
        exportSize = ParseOr<int>(sizeTes.text, 0);
    }
    if (SimpleGUI::Button(U"Save", ExportMenu.pos + Vec2(12, 305), 171)) {
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
    copyright(U"Sprator 0.1.1").draw(727, 581, Palette::White);
}