varying vec3 normal; 
void main() {
   float intensity = dot( normalize( normal ),
                          normalize( gl_LightSource[0].position.xyz ) );
   gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 ) * clamp( intensity, 0.2, 1.0 );
}
