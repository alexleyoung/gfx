#include <time.h>

#define SOKOL_IMPL
#define SOKOL_GLCORE
#define SOKOL_NO_ENTRY
#include "../vendor/sokol/sokol_app.h"
#include "../vendor/sokol/sokol_gfx.h"
#include "../vendor/sokol/sokol_glue.h"

#define CGLM_ALL_UNALIGNED
#include "cglm/cam.h"
#include "cglm/mat4.h"

#include "model.h"
#include "my_math.h"
#include "shapes.h"
#include "triangle_shader.h"

enum ROTATION {
  ROT_X,
  ROT_Y,
  ROT_Z,
};

static size_t frame_count = 0;
static clock_t start_time = 0;
static enum ROTATION rot = ROT_X;

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;
  model triangle;
} state;

void init(void) {
  sg_setup(&(sg_desc){
      .environment = sglue_environment(),
  });

  state.binding.vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
      .data = SG_RANGE(triangle_vertices),
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
  state.triangle = model_new();

  start_time = clock();
}

void frame(void) {
  state.triangle.pos[2] -= sapp_frame_duration();

  mat4 model, view, proj;

  model_matrix(&state.triangle, model);

  glm_mat4_identity(view);
  vec3 eye = {0.0f, 0.0f, 1.0f};
  vec3 center = {0.0f, 0.0f, -1.0f};
  vec3 up = {0.0f, 1.0f, 0.0f};
  glm_lookat(eye, center, up, view);

  float fov = 90.0f;
  float width = sapp_widthf();
  float height = sapp_heightf();
  float aspect = width / height;
  float near = 0.1f;
  float far = 100.0f;
  glm_perspective(fov, aspect, near, far, proj);

  // static float angle = 0.0f;
  // angle += 0.01f;
  //
  // switch (rot) {
  // case ROT_X:
  //   mat4_rotate_x(vs_params.mvp, angle);
  //   break;
  // case ROT_Y:
  //   mat4_rotate_y(vs_params.mvp, angle);
  //   break;
  // case ROT_Z:
  //   mat4_rotate_z(vs_params.mvp, angle);
  // }

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);

  // uniforms
  vs_params_t params = {0};
  glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, params.mvp);
  sg_apply_uniforms(UB_vs_params, &SG_RANGE(params));

  sg_draw(0, 3, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) {}

void handle_key_down(const sapp_event *ev) {
  switch (ev->key_code) {
  case SAPP_KEYCODE_ESCAPE:
    sapp_request_quit();
    break;
  default:
    break;
  }
}
void handle_char_event(const sapp_event *ev) {
  switch (ev->char_code) {
  case 'r':
    if (rot == ROT_X)
      rot = ROT_Y;
    else if (rot == ROT_Y)
      rot = ROT_Z;
    else
      rot = ROT_X;
  }
}
void handle_quit_requested(const sapp_event *ev) {
  printf("quitting application\n");
}
void event(const sapp_event *ev) {
  frame_count = ev->frame_count;
  switch (ev->type) {
  case SAPP_EVENTTYPE_KEY_DOWN:
    handle_key_down(ev);
    break;
  case SAPP_EVENTTYPE_CHAR:
    handle_char_event(ev);
    break;
  case SAPP_EVENTTYPE_QUIT_REQUESTED:
    handle_quit_requested(ev);
    break;
  default:
    break;
  }
}

int main() {
  sapp_run(&(sapp_desc){
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
      .width = 800,
      .height = 800,
  });

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("total frames rendered: %zu in %.2f seconds (%.2f fps)\n", frame_count,
         elapsed_time, frame_count / elapsed_time);

  return 0;
}
