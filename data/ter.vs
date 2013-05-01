varying vec3 normal;
varying vec3 position_eyespace;
varying vec3 position_worldspace;
varying vec3 vertex_light_position;

uniform sampler2D terrainMap;
uniform sampler2D normalMap;

varying vec2 tex_coord;

float getHeight(vec4 dv);

// Terrain vertex shader

void main(void) {
  vec4 newVertexPos;
  vec4 dv;
  vec4 norm;

  // load heightmap and normal map
  tex_coord = gl_MultiTexCoord0.xy;
  dv   = texture2D(terrainMap, tex_coord);
  norm = texture2D( normalMap, tex_coord);

  // get displacement from heightmap
  float df = getHeight(dv);

  // calculate new vertex position
  newVertexPos = vec4(0.0,df,0.0,0.0) + gl_Vertex;

  // the fragment shader requires both the world space position (for
  // consistent bump mapping) & eyespace position (for the phong
  // specular highlight)
  position_eyespace = vec3(gl_ModelViewMatrix * newVertexPos);
  position_worldspace = newVertexPos.xyz;

  // pass along the normal
  normal = gl_NormalMatrix * (norm.xyz + gl_Normal)/2.0;

  // set the position
  gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;
}

// uses rgb values to determine displacement from height map
float getHeight(vec4 dv) {
  float df;

  // add effect from RGB components
  df = 256.0*256.0*dv.x + 256.0*dv.y + dv.z;

  // adjust value to match weighting required for the scene
  df = df / (256.0*256.0*2.0);

  return df;
}
