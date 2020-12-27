#include "Sprite.h"

namespace sprator {
    size_t Sprite::countNeighborhood(size_t y, size_t x) {
        size_t n = 0;

        if (y >= 1) {
            n += spriteGrid_[y - 1][x].previous;
        }
        if (y <= spriteSize_ - 2) {
            n += spriteGrid_[y + 1][x].previous;
        }
        if (x >= 1) {
            n += spriteGrid_[y][x - 1].previous;
        }
        if (x <= spriteSize_ / 2 - 2) {
            n += spriteGrid_[y][x + 1].previous;
        }

        return n;
    }

    void Sprite::grow() {
        for (auto y : Range(1, spriteSize_ - 2)) {
            for (auto x : Range(1, spriteSize_ / 2 - 1)) {
                const bool prev = spriteGrid_[y][x].previous;
                const size_t n = countNeighborhood(y, x);
                spriteGrid_[y][x].current = (prev == 0 && n <= 1) || (prev == 1 && (n == 2 || n == 3));
            }
        }
        for (auto& cell : spriteGrid_) {
            cell.previous = cell.current;
        }
    }

    Sprite::Sprite(const Rect& frame)
        : frame_(frame) {
        init(2);
        update(rndColor_, HSV(0, 0, 0.3));
    }

    void Sprite::init(uint8 growCount) {
        for (auto y : Range(1, spriteSize_ - 2)) {
            for (auto x : Range(1, spriteSize_ / 2 - 1)) {
                spriteGrid_[y][x] = { RandomBool(0.5), 0 };
            }
        }
        rndColor_ = HSV(Random(360), 0.6, Random(0.6, 0.8));
        for (auto i : step(growCount)) {
            grow();
        }
    }

    void Sprite::update(const HSV& baseColor, const HSV& bgColor) {
        image_ = grid2image(80, baseColor, bgColor);
        texture_.fill(image_);
    }

    void Sprite::draw() const {
        texture_.draw(frame_.pos);
        if (frame_.mouseOver()) {
            frame_.drawFrame(3, 0, Color(224));
        }
        else {
            frame_.drawFrame(3, 0, Color(160));
        }
    }

    Image Sprite::grid2image(const size_t exportSize, const HSV& baseColor, const HSV& bgColor) {
        // Twitterのような円形アイコンでも収まるように比率を計算
        const int32 ratio = exportSize / (double)spriteSize_ / Math::Sqrt(2);
        // ソフトのアイコンにするなら枠ギリギリまで入るようにこっちを使う
        // const size_t ratio = exportSize / spriteSize_;

        const int32 offset = (exportSize - spriteSize_ * ratio) / 2;
        const int32 posGap = exportSize % 2; // exportSizeが奇数のときの位置合わせ
        Image retImage(exportSize, bgColor);

        for (auto y : Range(0, spriteSize_* ratio - 1)) {
            for (auto x : Range(0, spriteSize_* ratio / 2 - 1)) {
                const int32 posY = offset + y;
                const int32 posX = offset + x;
                const int32 posMirrorX = exportSize - (posX + 1) - posGap;
                if (spriteGrid_[y / ratio][x / ratio].previous) {
                    retImage[posY][posX] = retImage[posY][posMirrorX] = baseColor;
                }
                else if (countNeighborhood(y / ratio, x / ratio)) {
                    retImage[posY][posX] = retImage[posY][posMirrorX] = HSV(baseColor.h - 15, baseColor.s, baseColor.v * 0.4);
                }
            }
        }

        return retImage;
    }
}