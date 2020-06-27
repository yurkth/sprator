# Sprator

Sprator is a GUI application to generate sprites using [cellular automaton](https://en.wikipedia.org/wiki/Cellular_automaton). Generated sprites can be exported as icons with specified color and size.

![](https://user-images.githubusercontent.com/59264002/72552708-d2453b80-38da-11ea-8059-5fb624933144.png)

## Installation

You can [download](https://github.com/yurkth/sprator/releases) Sprator for Windows.

## Requirement

Sprator can be build using [Visual Studio 2019](https://visualstudio.microsoft.com/ja/vs/) and [OpenSiv3D v0.4.2](https://github.com/Siv3D/OpenSiv3D).

## Usage

Select the sprite and click "Save" to save the PNG.

Sprite color can be set with "Base Color" and background color can be set with "BG Color".

Right-click to change the number of cellular automaton steps.

## Theory

1. Generate 4x8 white noise.

   ![](https://user-images.githubusercontent.com/59264002/76070404-d38c0e00-5fd7-11ea-9ec2-674813c12490.png)

2. Change the state according to the following rules.

   - Any live cell with two or three neighbors survives.
   - Any dead cell with one or less live neighbors becomes a live cell.
   - All other live cells die in the next generation. Similarly, all other dead cells stay dead.

3. Repeat steps 2 several times.

   ![](https://user-images.githubusercontent.com/59264002/76137835-c8db8280-6084-11ea-80e8-68d436590d7b.png)

4. Flip and add an outline, complete!

   ![](https://user-images.githubusercontent.com/59264002/76070456-e56db100-5fd7-11ea-9fed-4c178bf0a756.png)

## Blog

[以前作ったアイコン生成ソフトをリメイクした - 屋根裏](https://yurkth.hateblo.jp/entry/sprite-generator)

## License

Sprator is licensed under the MIT license. See the [LICENSE](/LICENSE) for more information.

