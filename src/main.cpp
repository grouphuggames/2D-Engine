#include "GLGraphics.h"
#include "glfw/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"
#include "vector.h"
#include "Timer.h"
#include "Particles.h"
#include "Audio.h"
#include "Input.h"
#include "Scene.h"
#include "Animation.h"


const f64 PI = 3.14159;

static inline f32 ToRadians(f32 degrees)
{
  return degrees * ((f32)PI / 180.f);
}

f32 GetRandomFloat()
{
  std::mt19937 generator((u32)std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<f32> distribution(0.f, 1.f);
  return distribution(generator);
}

f32 GetRandomFloatInRange(f32 lower, f32 upper)
{
  std::mt19937 generator((u32)std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<f32> distribution(lower, upper);
  return distribution(generator);
}

TimerInfo game_timer;
TimerInfo frame_timer;
f32 game_time = 0.f;

f32 delta_time = 0.f;
f32 last_frame = 0.f;

//en::vector<std::string> scene_names;

bool application_active = true;

void CloseWindow()
{
  application_active = false;
}

void Hit()
{
  PlaySound("Hit.wav");
}

AnimInfo megaman_anim = {};

void RunRight()
{
  megaman_anim.anim_flipped = false;
  megaman_anim.anim_state = ANIM_STATE::ACTIVE;
  u32 sprite_timer_value = GetTimerValue(megaman_anim.anim_timer);
  if (sprite_timer_value > 100)
  {
    if (megaman_anim.anim_frame == 9)
    {
      megaman_anim.anim_frame = 0;
    }
    else
    {
      megaman_anim.anim_frame++;
    }

    StartTimer(megaman_anim.anim_timer);
  }
}

void RunLeft()
{
  megaman_anim.anim_flipped = true;
  megaman_anim.anim_state = ANIM_STATE::ACTIVE;
  u32 sprite_timer_value = GetTimerValue(megaman_anim.anim_timer);
  if (sprite_timer_value > 100)
  {
    if (megaman_anim.anim_frame == 9)
    {
      megaman_anim.anim_frame = 0;
    }
    else
    {
      megaman_anim.anim_frame++;
    }

    StartTimer(megaman_anim.anim_timer);
  }
}

s32 main()
{
  aspect_ratio = (f32)window_width / (f32)window_height;
  const char* glsl_version = "#version 330";

  GLFWwindow* window;
  
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(window_width, window_height, "Hello Window", NULL, NULL);
  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  glewInit();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
    style.WindowRounding = 0.f;
    style.Colors[ImGuiCol_WindowBg].w = 1.f;
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

  if (!InitSound())
  {
    DebugPrintToConsole("Error: Could not init sound!");
  }

  StartTimer(game_timer);

  SetKeyboardInput(CloseWindow, GLFW_KEY_ESCAPE, BUTTON_ACTION::PRESS);
  SetKeyboardInput(Hit, GLFW_KEY_SPACE, BUTTON_ACTION::PRESS);

  SetKeyboardInput(RunRight, GLFW_KEY_D, BUTTON_ACTION::HOLD);
  SetKeyboardInput(RunLeft, GLFW_KEY_A, BUTTON_ACTION::HOLD);

  particle_textures[0] = LoadGLTexture("cloud0.png");
  particle_textures[1] = LoadGLTexture("cloud1.png");
  particle_textures[2] = LoadGLTexture("cloud2.png");
  particle_textures[3] = LoadGLTexture("cloud3.png");
  particle_textures[4] = LoadGLTexture("cloud4.png");

  Entity megaman;
  megaman.position = vec2(0.75f, 0.75f);
  megaman.scale = vec2(0.25f, 0.25f * aspect_ratio);
  megaman.shader = LoadGLShader("entity_animated.glsl");
  megaman.texture = LoadGLTexture("megaman_run.jpg");
  megaman.angle = 0.f;
  glGenVertexArrays(1, &megaman.vao);
  glGenBuffers(1, &megaman.vbo);
  glGenBuffers(1, &megaman.ebo);
  glBindVertexArray(megaman.vao);
  glBindBuffer(GL_ARRAY_BUFFER, megaman.vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), &megaman.verts[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, megaman.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u32), quad_indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  entities.PushBack(megaman);
  original_scales.PushBack(vec2(megaman.scale.x(), megaman.scale.y() / aspect_ratio));
  StartTimer(megaman_anim.anim_timer);

  en::vector<ParticleVertex> particle_verts;
  en::vector<u32> particle_indices;

  s32 particle_shader = LoadGLShader("particle_textured.glsl");
  glUseProgram(particle_shader);
  s32 samplers[5] = { 0, 1, 2, 3, 4 };
  glUniform1iv(glGetUniformLocation(particle_shader, "textures"), 5, samplers);

  for (s32 i = 0; i < MAX_PARTICLES; i++)
  {
    particles.position.PushBack(vec2(GetRandomFloatInRange(-1.f, 1.f), GetRandomFloatInRange(-1.f, 1.f)));
    particles.scale.PushBack(vec2(0.05f, 0.05f));
    particles.color.PushBack(vec4(GetRandomFloat(), GetRandomFloat(), GetRandomFloat(), GetRandomFloat()));

    particle_verts.PushBack({ vec2(0.1f + particles.position[i].x(), 0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(0.f, 0.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(0.1f + particles.position[i].x(), -0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(1.f, 0.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(-0.1f + particles.position[i].x(), -0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(1.f, 1.f), 5.f * GetRandomFloat() });
    particle_verts.PushBack({ vec2(-0.1f + particles.position[i].x(), 0.1f + particles.position[i].y()), vec4(particles.color[i]), vec2(0.f, 1.f), 5.f * GetRandomFloat() });

    particle_indices.PushBack(0 + i * 4);
    particle_indices.PushBack(1 + i * 4);
    particle_indices.PushBack(3 + i * 4);
    particle_indices.PushBack(1 + i * 4);
    particle_indices.PushBack(2 + i * 4);
    particle_indices.PushBack(3 + i * 4);
  }

  glGenVertexArrays(1, &particles.vao);
  glGenBuffers(1, &particles.vbo);
  glGenBuffers(1, &particles.ebo);
  glBindVertexArray(particles.vao);
  glBindBuffer(GL_ARRAY_BUFFER, particles.vbo);
  glBufferData(GL_ARRAY_BUFFER, particles.position.Size() * 4 * sizeof(ParticleVertex), &particle_verts[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles.position.Size() * 6 * sizeof(u32), &particle_indices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32) + 4 * sizeof(f32)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32) + 4 * sizeof(f32) + 2 * sizeof(f32)));
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  LoadScene("test_scene.enscene");
  PlaySound("MainTheme.wav");

  bool show_demo_window = true;

  while (application_active)
  {
    glfwPollEvents();
    ProcessKeyboardInput(window);

    StartTimer(frame_timer);

    glClearColor(1.f, 0.8f, 0.7f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (entities.Size() > 0)
    {
      for (auto& e : entities)
      {
        mat4 transform = mat4::Identity();
        transform = mat4::Translate(vec3(e.position.x(), e.position.y(), 0.f));
        transform *= mat4::Rotate(e.angle, vec3(0.f, 0.f, 1.f));
        transform *= mat4::Scale(vec3(e.scale.x(), e.scale.y(), 1.f));

        glUseProgram(e.shader);
        glUniformMatrix4fv(glGetUniformLocation(e.shader, "transform"), 1, GL_FALSE, transform.elements);
        if (e.shader == megaman.shader)
        {
          if (megaman_anim.anim_state == ANIM_STATE::ACTIVE)
          {
            glUniform1i(glGetUniformLocation(e.shader, "anim_frame"), megaman_anim.anim_frame);
          }
          else
          {
            megaman_anim.anim_frame = 0;
            glUniform1i(glGetUniformLocation(e.shader, "anim_frame"), megaman_anim.anim_frame);
          }
        
          if (megaman_anim.anim_flipped)
          {
            glUniform1f(glGetUniformLocation(e.shader, "anim_flip_val"), -1.f);
          }
          else
          {
            glUniform1f(glGetUniformLocation(e.shader, "anim_flip_val"), 1.f);
          }
        }

        glBindVertexArray(e.vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, e.texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      }
    }

    mat4 particle_transform = mat4::Identity();
    glUseProgram(particle_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particle_textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, particle_textures[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, particle_textures[2]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, particle_textures[3]);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, particle_textures[4]);
    glUniformMatrix4fv(glGetUniformLocation(particle_shader, "transform"), 1, GL_FALSE, particle_transform.elements);

    glBindVertexArray(particles.vao);
    glDrawElements(GL_TRIANGLES, 6 * particles.position.Size(), GL_UNSIGNED_INT, 0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(window);
    }

    glfwSwapBuffers(window);
    //DebugPrintToConsole("Frame Time: ", delta_time, "s");

    game_time = (f32)GetTimerValue(game_timer) / 1000.f;
    StopTimer(frame_timer);
    delta_time = frame_timer.time_delta / 1000.f;
    megaman_anim.anim_state = ANIM_STATE::INACTIVE;
  }

  DebugPrintToConsole("Clean program exit");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
