#include <Siv3D.hpp> // OpenSiv3D v0.4.2
#include "Sprite.h"
#include "Interface.h"

void Main() {
    Window::SetTitle(U"Sprator | grow: 2");
    Scene::SetBackground(HSV(0, 0, 0.3));

    sprator::Interface interface;
    Array<sprator::Sprite> sprites;

    for (auto i : step(49)) {
        sprites << sprator::Sprite(Rect(5 + 85 * (i % 7), 5 + 85 * (i / 7), 80));
    }

    while (System::Update()) {
        interface.draw(sprites);
    }
}