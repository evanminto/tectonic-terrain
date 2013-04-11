varying vec3 normal;
varying vec3 position_eyespace;
varying vec3 position_worldspace;

// a shader for a black & white checkerboard

void main (void) {  

  vec3 color;

  // determine the parity of this point in the 3D checkerboard
  int count = 0;
  if (mod(position_worldspace.x,0.3)> 0.15) count++;
  if (mod(position_worldspace.y,0.3)> 0.15) count++;
  if (mod(position_worldspace.z,0.3)> 0.15) count++;
  if (count == 1 || count == 3) {
    color = vec3(0.1,0.1,0.1);
  } else {
    color = vec3(1,1,1);
  }

  // direction to the light
  vec3 light = normalize(gl_LightSource[1].position.xyz - position_eyespace);

  // basic diffuse
  float ambient = 0.3;
  float diffuse = 0.7*max(dot(normal,light),0.0);
  color = ambient*color + diffuse*color;
  gl_FragColor = vec4 (color, 1.0);
}
