#include "imgui.h"
#include "implot.h"
#include "imgui_impl_glfw_gl3.h"
#include "icons_font_awesome.h"
// Needed by icons_font_awesome.ttf.h
#include <cstdint>
#include "icons_font_awesome.ttf.h"
#include "GL/gl3w.h" // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>
#include <cstdio>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <fstream>

static void default_error_callback(int error, const char* description)
{
  fprintf(stderr, "Error %d: %s\n", error, description);
}

namespace o2
{
namespace framework
{

bool first = true;
int times = 0;
std::ofstream data_file;

// @return an object of kind GLFWwindow* as void* to avoid having a direct dependency
void* initGUI(const char* name, void(*error_callback)(int, char const*description))
{
  // Setup window
  if (error_callback == nullptr) {
    glfwSetErrorCallback(default_error_callback);
  }
  if (!glfwInit())
    return nullptr;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  GLFWwindow* window = glfwCreateWindow(1280, 720, name, nullptr, nullptr);
  glfwMakeContextCurrent(window);
  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init(window, true);

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->AddFontDefault();
  static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true; icons_config.FontDataOwnedByAtlas = false;
  io.Fonts->AddFontFromMemoryTTF((void*)s_iconsFontAwesomeTtf, sizeof(s_iconsFontAwesomeTtf), 12.0f, &icons_config, icons_ranges);

  ImPlot::CreateContext();

  data_file.open("/home/luigi/alice/DebugGUI/data.json");
  return window;
}


void showDebugInfo(ImDrawData *draw_data) {
  for (int i = 0; i < draw_data->CmdListsCount; ++i) {
    const auto cmd_list = draw_data->CmdLists[i];
    const auto vtx_buffer = cmd_list->VtxBuffer;
    const auto idx_buffer = cmd_list->IdxBuffer;
    const auto cmd_buffer = cmd_list->CmdBuffer;
    
    times++;
    if (times == 10) {
      first = false;
      data_file << "{\"vtx\":[";
      for (int i = 0; i < vtx_buffer.size(); ++i) {
        auto v = vtx_buffer[i];
        data_file << "[" << v.pos.x << "," << v.pos.y << "," << v.col << ',' << v.uv.x << "," << v.uv.y << "]";
        if (i < vtx_buffer.size() - 1) data_file << ",";
      }
      data_file << "],\"idx\":[";
      for (int i = 0; i < idx_buffer.size(); ++i) {
        auto id = idx_buffer[i];
        data_file << id;
        if (i < idx_buffer.size() - 1) data_file << ",";
      }
      data_file << "],\"cmd\":[";
      for (int i = 0; i < cmd_buffer.size(); ++i) {
        auto cmd = cmd_buffer[i];
        data_file << cmd.ElemCount;
        if (i < cmd_buffer.size() - 1) data_file << ",";
      }
      /*ImGuiIO& io = ImGui::GetIO();
      unsigned char* pixels;
      int width, height;
      io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
      data_file << "],\"tex\":{\"w\":" << width << ",\"h\":" << height << ",\"p\":[";
      for (int i = 0; i < width*height; ++i) {
        data_file << (int)pixels[i*4+3] << ",";
      }
      data_file << "]}}";*/
      data_file << "]}";
      data_file.close();
    }
  }
}

/// @return true if we do not need to exit, false if we do.
bool pollGUI(void* context, std::function<void(void)> guiCallback)
{
  GLFWwindow* window = reinterpret_cast<GLFWwindow*>(context);
  if (glfwWindowShouldClose(window)) {
    return false;
  }
  glfwPollEvents();
  ImGui_ImplGlfwGL3_NewFrame();


  // Rendering
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImVec4 clear_color = ImColor(114, 144, 154);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  // This is where the magic actually happens...
  if (guiCallback) {
    guiCallback();
  }
  ImGui::Render();
  auto draw_data = ImGui::GetDrawData();
  ImGui_ImplGlfwGL3_RenderDrawLists(draw_data);
  showDebugInfo(draw_data);
  glfwSwapBuffers(window);
  return true;
}

void disposeGUI()
{
  ImPlot::DestroyContext();
  // Cleanup
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();
}

} // namespace framework
} // namespace o2
