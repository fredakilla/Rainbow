attribute vec3 a_position;
attribute vec3 a_color0;

varying vec3 v_color0;

void main()
{
    gl_Position = vec4(a_position.xyz, 1.0f);
    v_color0 = a_color0;
}
