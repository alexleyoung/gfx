#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "../vendor/sokol/sokol_app.h"
#include "../vendor/sokol/sokol_gfx.h"
#include "../vendor/sokol/sokol_glue.h"

#include "../include/my_math.h"
#include "../include/shapes.h"
#include "../include/triangle_shader.h"

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;
} state;

void init(void) {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
  });

  state.binding.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(cube_vertices),
  });

  state.pipeline = sg_make_pipeline(&(sg_pipeline_desc){
      .shader = sg_make_shader(triangle_shader_desc(sg_query_backend())),
      .layout = {.attrs =
                     {
                         [ATTR_triangle_pos].format = SG_VERTEXFORMAT_FLOAT3,
                         [ATTR_triangle_color].format = SG_VERTEXFORMAT_FLOAT3,
                     }},
      .primitive_type = SG_PRIMITIVETYPE_TRIANGLES,
  });

  state.pass_action =
      (sg_pass_action){.colors[0] = {.load_action = SG_LOADACTION_CLEAR,
                                     .clear_value = {0.1, 0.1, 0.1, 1.0}}};
}
void frame(void) {
  static float rot = 0.0f;
  rot += 0.01f;

  vs_params_t vs_params;
  mat4_rotate_z(vs_params.mvp, rot);

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});

  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);
  sg_apply_uniforms(0, &(sg_range){
                           .ptr = &vs_params,
                           .size = sizeof(vs_params),
                       });
  sg_draw(0, 36, 1);

  sg_end_pass();
  sg_commit();
}
void cleanup(void) {}
void event(const sapp_event *ev) {}

int main() {
  sapp_run(&(sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
      .width = 800,
      .height = 800,
  });

  return 0;
}
