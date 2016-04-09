# Lambda Engine - a fast 2D OpenGL game engine


Directory structure:
```
 dist/
  the build system sticks output here
 tools/
  map compiler and other tools
 engine/
  source for the core of the engine itself
 sample/
  sample game for testing
``` 

The engine is designed to make games that are easy to mod and a final distribution package therefore contains the following at a minimal:
```
   bin/lambda      - main executable
   data/core01.pak - patchable engine code
   data/core02.pak - game code and assets
```
Patches can be installed by placing patch##.pak (where ## is a 2-digit number) into data/ and will automatically be loaded.

Mods can be placed into mods/ in the distribution package or in ~/.lambda/mods/
