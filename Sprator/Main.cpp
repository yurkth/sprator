#include <Siv3D.hpp> // OpenSiv3D v0.4.3
#include "Interface.h"

void Main() {
    Window::SetTitle(U"Sprator | grow: 2");
    Scene::SetBackground(HSV(0, 0, 0.3));

    sprator::Interface interface;

    while (System::Update()) {
        interface.update();
        interface.draw();
    }
}