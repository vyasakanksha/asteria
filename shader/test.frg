varying vec3 normal;

void main() {
   normal = normalize( normal );
   float intensity = clamp( dot( normal, normalize( gl_LightSource[0].position
   ) ), 0.2f, 1.0f );
   gl_FragColor = intensity * gl_Color;
}
