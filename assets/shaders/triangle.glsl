@vs vertex_shader

in vec3 pos;

void main() {
  gl_Position = vec4(pos, 1.0);
}

@end

@fs fragment_shader

out vec4 frag_color;

void main() {
  frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}

@end

@program triangle vertex_shader fragment_shader
