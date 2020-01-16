#include "Sprite.h"

int sprator::Sprite::countNeighborhood(int y, int x) {
    int n = 0;

    if (y >= 1) {
        n += spriteGrid[y - 1][x].previous;
    }
    if (y <= spriteSize - 2) {
        n += spriteGrid[y + 1][x].previous;
    }
    if (x >= 1) {
        n += spriteGrid[y][x - 1].previous;
    }
    if (x <= spriteSize / 2 - 2) {
        n += spriteGrid[y][x + 1].previous;
    }

    return n;
}

void sprator::Sprite::grow() {
    for (auto y : Range(1, spriteSize - 2)) {
        for (auto x : Range(1, spriteSize / 2 - 1)) {
            const int prev = spriteGrid[y][x].previous;
            const int n = countNeighborhood(y, x);
            spriteGrid[y][x].current = (prev == 0 && n <= 1) || (prev == 1 && (n == 2 || n == 3));
        }
    }
    for (auto& cell : spriteGrid) {
        cell.previous = cell.current;
    }
}

sprator::Sprite::Sprite(const Rect& _frame)
    : frame(_frame) {
    init(2);
    update(rndColor, HSV(0, 0, 0.3));
}

void sprator::Sprite::init(int growCount) {
    for (auto y : Range(1, spriteSize - 2)) {
        for (auto x : Range(1, spriteSize / 2 - 1)) {
            spriteGrid[y][x] = { RandomBool(0.5), 0 };
        }
    }
    rndColor = HSV(Random(360), 0.6, Random(0.6, 0.8));
    for (auto i : step(growCount)) {
        grow();
    }
}

void sprator::Sprite::update(const HSV& baseColor, const HSV& bgColor) {
    image = grid2image(80, baseColor, bgColor);
    texture.fill(image);
}

void sprator::Sprite::draw() const {
    texture.draw(frame.pos);
    if (frame.mouseOver()) {
        frame.drawFrame(3, 0, Color(224));
    }
    else {
        frame.drawFrame(3, 0, Color(160));
    }
}

Image sprator::Sprite::grid2image(const int exportSize, const HSV& baseColor, const HSV& bgColor) {
    // Twitterのような円形アイコンでも収まるように比率を計算
    const int ratio = exportSize / (double)spriteSize / Math::Sqrt(2);
    // ソフトのアイコンにするなら枠ギリギリまで入るようにこっちを使う
    // const int ratio = exportSize / spriteSize;

    const int offset = (exportSize - spriteSize * ratio) / 2;
    const int posGap = exportSize % 2; // exportSizeが奇数のときの位置合わせ
    Image retImage(exportSize, bgColor);

    for (auto y : Range(0, spriteSize * ratio - 1)) {
        for (auto x : Range(0, spriteSize * ratio / 2 - 1)) {
            const int posY = offset + y;
            const int posX = offset + x;
            const int posMirrorX = exportSize - (posX + 1) - posGap;
            if (spriteGrid[y / ratio][x / ratio].previous) {
                retImage[posY][posX] = retImage[posY][posMirrorX] = baseColor;
            }
            else if (countNeighborhood(y / ratio, x / ratio)) {
                retImage[posY][posX] = retImage[posY][posMirrorX] = HSV(baseColor.h - 15, baseColor.s, baseColor.v * 0.4);
            }
        }
    }

    return retImage;
}