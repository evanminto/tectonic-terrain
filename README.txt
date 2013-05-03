tectonic-terrain

Build Instructions:
1) mkdir build
2) cp data/* build
3) cd build
4) cmake ../src
5) make

Example:
./render -i flat16.obj -h hm16.bmp -n norm16.bmp

Options:
-i filename.obj   - Specify .obj file
-v filename.vs    - Specify vertex shader
-f filename.fs    - Specify fragment shader
-h filename.bmp   - Specify heightmap texture
-n filename.bmp   - Specify normalmap texture
-s 600            - Specify window size (pixels)
-t 200            - Specify timestep size (ms)
-p 2	 		  - Specify preset (0-9)

Controls:
'd',' ' - Single step terrain displacement
'a'     - Animate terrain displacement
'x'     - Enable shaders
'q'     - Quit

