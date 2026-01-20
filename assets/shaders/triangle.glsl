@vs vertex_shader

layout(binding=0) uniform vs_params {
    mat4 model; // transform object -> world space
    mat4 view_proj; // view: camera -> camera space, proj: camera -> clip space. clip -> screen done by GPU perspective division and viewport transformation
};

in vec3 pos;
in vec3 color;

out vec3 out_color;

void main() {
  gl_Position = view_proj * model * vec4(pos, 1.0);
  out_color = color;
}

@end

@fs fragment_shader

in vec3 out_color;

out vec4 frag_color;

void main() {
  frag_color = vec4(out_color, 1.0);
}

@end

@program triangle vertex_shader fragment_shader
