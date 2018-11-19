varying vec3 v_color0;

void main()
{
    gl_FragColor = vec4(v_color0.xyz, 1.0);
}
