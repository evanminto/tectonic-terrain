varying vec3 normal;
varying vec3 position_eyespace;
varying vec3 position_worldspace;
uniform sampler2D terrainMap;

// Terrain height map shader

void main(void) {
  vec4 newVertexPos;
  vec4 dv;
  float df;

  gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;
  dv = texture2D(terrainMap, gl_MultiTexCoord0.xy);
  df = 256.0*256.0*dv.x + 256.0*dv.y + dv.z;
  df = df / (256.0*256.0*2.0);
  newVertexPos = vec4(gl_Normal * df * 1.0, 0.0) + gl_Vertex;

  // the fragment shader requires both the world space position (for
  // consistent bump mapping) & eyespace position (for the phong
  // specular highlight)
  position_eyespace = vec3(gl_ModelViewMatrix * newVertexPos);
  position_worldspace = newVertexPos.xyz;

  // pass along the normal
  normal = normalize(gl_NormalMatrix * gl_Normal);

  gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;
 }
