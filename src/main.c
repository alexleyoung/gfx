#include <X11/Xlib.h>
#include <math.h>
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
#include "cglm/util.h"
#include "cglm/vec3.h"

#include "model.h"
#include "shapes.h"
#include "triangle_shader.h"

#define RES_1080P {1920.0f, 1080.0f}
#define RES_720P {1280.0f, 720.0f}
#define RES_800x800 {800.0f, 800.0f}
#define AR_4_3 4.0f / 3.0f
#define AR_16_9 16.0f / 9.0f

static size_t frame_count = 0;
static clock_t start_time = 0;

static float move_speed = 5.0f;
static float vim_horizontal_sens = 10.0f;
static float vim_vertical_sens = 10.0f;
static float horizontal_sens = 0.5f;
static float vertical_sens = 0.5f;

static vec2 resolution = RES_720P;
static float aspect = AR_16_9;

static struct {
  sg_pass_action pass_action;
  sg_bindings binding;
  sg_pipeline pipeline;

  model cube;

  vec3 up;
  vec3 eye;
  vec3 center;
  vec3 forward;
  vec3 right;
  float yaw, pitch; // horizontal, vertical

  bool keys_down[SAPP_MAX_KEYCODES];
} state;

void move_camera_look(float dx, float dy, double dt) {
  state.yaw += dx * horizontal_sens * dt;
  state.pitch += dy * -vertical_sens * dt;
  const float maxPitch = glm_rad(89.0f);
  const float minPitch = -maxPitch;
  state.pitch = glm_clamp(state.pitch, minPitch, maxPitch);
}

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
  state.cube = model_new();
  state.cube.pos[2] -= 1.0f;

  state.up[0] = 0.0f;
  state.up[1] = 1.0f;
  state.up[2] = 0.0f;

  state.eye[0] = 0.0f;
  state.eye[1] = 0.0f;
  state.eye[2] = 1.0f;

  state.yaw = -90.0f;
  state.pitch = 0.0f;

  start_time = clock();

  sapp_lock_mouse(true);
}

void frame(void) {
  float dt = (float)sapp_frame_duration();

  // wasd movement
  if (state.keys_down[SAPP_KEYCODE_W])
    glm_vec3_muladds(state.forward, move_speed * dt,
                     state.eye); // eye += forward * speed * dt
  if (state.keys_down[SAPP_KEYCODE_S])
    glm_vec3_muladds(state.forward, -move_speed * dt,
                     state.eye); // eye -= forward * speed * dt
  if (state.keys_down[SAPP_KEYCODE_A])
    glm_vec3_muladds(state.right, -move_speed * dt,
                     state.eye); // eye -= right * speed * dt
  if (state.keys_down[SAPP_KEYCODE_D])
    glm_vec3_muladds(state.right, move_speed * dt,
                     state.eye); // eye += right * speed * dt

  // vertical movement
  if (state.keys_down[SAPP_KEYCODE_SPACE])
    glm_vec3_muladds(state.up, move_speed * dt, state.eye);
  if (state.keys_down[SAPP_KEYCODE_LEFT_CONTROL])
    glm_vec3_muladds(state.up, -move_speed * dt, state.eye);

  // vim camera movement
  if (state.keys_down[SAPP_KEYCODE_H])
    move_camera_look(-vim_horizontal_sens, 0.0f, dt);
  if (state.keys_down[SAPP_KEYCODE_J])
    move_camera_look(0.0f, vim_vertical_sens, dt);
  if (state.keys_down[SAPP_KEYCODE_K])
    move_camera_look(0.0f, -vim_vertical_sens, dt);
  if (state.keys_down[SAPP_KEYCODE_L])
    move_camera_look(vim_horizontal_sens, 0.0f, dt);

  mat4 model, view, proj;

  model_matrix(&state.cube, model);

  glm_mat4_identity(view);
  state.forward[0] = cosf(state.yaw) * cosf(state.pitch);
  state.forward[1] = sinf(state.pitch);
  state.forward[2] = sinf(state.yaw) * cosf(state.pitch);
  vec3 flatForward = {state.forward[0], 0.0f, state.forward[2]};
  glm_vec3_normalize(flatForward);
  glm_vec3_cross(flatForward, state.up, state.right);
  glm_vec3_normalize(state.right);
  glm_vec3_add(state.eye, state.forward, state.center);
  glm_lookat(state.eye, state.center, state.up, view);

  float fov = 90.0f;
  float near = 0.1f;
  float far = 100.0f;
  glm_perspective(fov, aspect, near, far, proj);

  sg_begin_pass(
      &(sg_pass){.action = state.pass_action, .swapchain = sglue_swapchain()});
  sg_apply_pipeline(state.pipeline);
  sg_apply_bindings(&state.binding);

  // uniforms
  vs_params_t params = {0};
  glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, params.mvp);
  sg_apply_uniforms(UB_vs_params, &SG_RANGE(params));

  sg_draw(0, 36, 1);
  sg_end_pass();
  sg_commit();
}

void cleanup(void) {}

void handle_key_down(const sapp_event *ev) {
  switch (ev->key_code) {
  case SAPP_KEYCODE_ESCAPE:
    sapp_request_quit();
    break;
  case SAPP_KEYCODE_W:
    state.keys_down[SAPP_KEYCODE_W] = true;
    break;
  case SAPP_KEYCODE_A:
    state.keys_down[SAPP_KEYCODE_A] = true;
    break;
  case SAPP_KEYCODE_S:
    state.keys_down[SAPP_KEYCODE_S] = true;
    break;
  case SAPP_KEYCODE_D:
    state.keys_down[SAPP_KEYCODE_D] = true;
    break;

  case SAPP_KEYCODE_SPACE:
    state.keys_down[SAPP_KEYCODE_SPACE] = true;
    break;
  case SAPP_KEYCODE_LEFT_CONTROL:
    state.keys_down[SAPP_KEYCODE_LEFT_CONTROL] = true;
    break;

  case SAPP_KEYCODE_H:
    state.keys_down[SAPP_KEYCODE_H] = true;
    break;
  case SAPP_KEYCODE_J:
    state.keys_down[SAPP_KEYCODE_J] = true;
    break;
  case SAPP_KEYCODE_K:
    state.keys_down[SAPP_KEYCODE_K] = true;
    break;
  case SAPP_KEYCODE_L:
    state.keys_down[SAPP_KEYCODE_L] = true;
    break;

  default:
    break;
  }
}
void handle_key_up(const sapp_event *ev) {
  switch (ev->key_code) {
  case SAPP_KEYCODE_W:
    state.keys_down[SAPP_KEYCODE_W] = false;
    break;
  case SAPP_KEYCODE_A:
    state.keys_down[SAPP_KEYCODE_A] = false;
    break;
  case SAPP_KEYCODE_S:
    state.keys_down[SAPP_KEYCODE_S] = false;
    break;
  case SAPP_KEYCODE_D:
    state.keys_down[SAPP_KEYCODE_D] = false;
    break;

  case SAPP_KEYCODE_SPACE:
    state.keys_down[SAPP_KEYCODE_SPACE] = false;
    break;
  case SAPP_KEYCODE_LEFT_CONTROL:
    state.keys_down[SAPP_KEYCODE_LEFT_CONTROL] = false;
    break;

  case SAPP_KEYCODE_H:
    state.keys_down[SAPP_KEYCODE_H] = false;
    break;
  case SAPP_KEYCODE_J:
    state.keys_down[SAPP_KEYCODE_J] = false;
    break;
  case SAPP_KEYCODE_K:
    state.keys_down[SAPP_KEYCODE_K] = false;
    break;
  case SAPP_KEYCODE_L:
    state.keys_down[SAPP_KEYCODE_L] = false;
    break;

  default:
    break;
  }
}
void handle_char_event(const sapp_event *ev) {
  switch (ev->char_code) {
  case 'c':
    state.eye[0] = 0.0f;
    state.eye[1] = 0.0f;
    state.eye[2] = 1.0f;
    state.yaw = -90.0f;
    state.pitch = 0.0f;
    break;
  }
}
void handle_quit_requested(const sapp_event *ev) {
  printf("quitting application\n");
}
void handle_mouse_move(const sapp_event *ev) {
  move_camera_look(ev->mouse_dx, ev->mouse_dy, sapp_frame_duration());
}
void handle_mouse_down(const sapp_event *ev) {
  if (ev->mouse_button == SAPP_MOUSEBUTTON_LEFT)
    sapp_lock_mouse(true);
  else
    sapp_lock_mouse(false);
}
void handle_mouse_up(const sapp_event *ev) {}
void event(const sapp_event *ev) {
  frame_count = ev->frame_count;
  switch (ev->type) {
  case SAPP_EVENTTYPE_KEY_DOWN:
    handle_key_down(ev);
    break;
  case SAPP_EVENTTYPE_KEY_UP:
    handle_key_up(ev);
    break;
  case SAPP_EVENTTYPE_CHAR:
    handle_char_event(ev);
    break;
  case SAPP_EVENTTYPE_QUIT_REQUESTED:
    handle_quit_requested(ev);
    break;
  case SAPP_EVENTTYPE_MOUSE_MOVE:
    handle_mouse_move(ev);
    break;
  case SAPP_EVENTTYPE_MOUSE_DOWN:
    handle_mouse_down(ev);
    break;
  case SAPP_EVENTTYPE_MOUSE_UP:
    handle_mouse_up(ev);
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
      .width = resolution[0],
      .height = resolution[1],
  });

  clock_t end_time = clock();
  double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("total frames rendered: %zu in %.2f seconds (%.2f fps)\n", frame_count,
         elapsed_time, frame_count / elapsed_time);

  return 0;
}
