# Kamikaze Birds (using ORX)

A lot of expirmenting and shortcuts here...

Probably not the best way to do a lot of things.

To run, on Linux, just need meson/ninja and run `python build.py init` then `./build/game` After the first build, only need `python build.py` to compile code. Config changes should just work like a normal ORX projec (no need to rebuild.)

If on Windows, may need to build dll of orx and copy to `ext/orx` but otherwise should work the same.
Note: The dlls should be in the `ext/orx` folder now. May have to manually copy them over to the build folder however. Things work slightly different on Windows than Linux it seems.

To play the game:

Pressing left/right arrow keys adds 'force' in that direction. Max force is 3x in either direction. To reverse direction could take up to 6 presses either way. Goal is to navigate through the gap in the waves of birds. 100 points per clear.

To restart the game press space bar.
