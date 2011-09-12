varying vec3 normal; 
void main() {
   float intensity = clamp( dot( normalize( normal ), normalize(
   gl_LightSource[0].position.xyz ) ), 0.2f, 1.0f );
   gl_FragColor = intensity * gl_Color;
}
