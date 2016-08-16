//
//  Geometry.hpp
//  Asuna
//
//  Copyright © 2016 Sam Haves. All rights reserved.
//

/*
This is the HUD class implementation.

MORE DOCUMENTATION TO COME, CALM YOUR SHIT!!
*/

#include "GL/glew.h"
#include <stdio.h>
#include "../../includes/Asuna/HUD/HUD.hpp"
#include "../../includes/Asuna/Common.hpp"

static SDL_Window*     g_window;
static std::string     g_title;
static bool            g_mousePressed[3] = { false, false, false };
static float           g_mouseWheel = 0.0f;
static GLuint          g_fontTexture = 0, g_vaoHandle, g_vboHandle, g_elementsHandle, g_shaderHandle,
                       g_vertHandle, g_fragHandle, g_attribLocationPosition, g_attribLocationUV,
                       g_attribLocationColor, g_attribLocationTex, g_attribLocationProjMtx;

namespace Asuna{

HUD::HUD(SDL_Window* window, const std::string title){
  g_window = window; g_title = title;
}

HUD::~HUD(){
  shutdown();
}

static void RenderDrawLists(ImDrawData* draw_data)
{
  // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
  ImGuiIO& io = ImGui::GetIO();
  int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
  int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
  if (fb_width == 0 || fb_height == 0)
    return;
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // Backup GL state
  GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
  GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
  GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
  GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
  GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
  GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
  GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
  GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
  GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
  CheckOpenGLError("State backup", 57);

  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glActiveTexture(GL_TEXTURE0);
  CheckOpenGLError("State setup", 75);

  // Setup orthographic projection matrix
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  CheckOpenGLError("Viewport", 84);
  const float ortho_projection[4][4] =
  {
      { 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
      { 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f },
      { 0.0f,                  0.0f,                  -1.0f, 0.0f },
      {-1.0f,                  1.0f,                   0.0f, 1.0f },
  };
  glUseProgram(g_shaderHandle);
  CheckOpenGLError("Program", 85);
  glUniform1i(g_attribLocationTex, 0);
  glUniformMatrix4fv(g_attribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
    CheckOpenGLError("Program + uniforms", 94);
  GLenum error = GL_NO_ERROR;
    error = glGetError();
    if (GL_NO_ERROR != error) {
        printf("GL Error %x encountered in %s.\n", error);
    }
  glBindVertexArrayAPPLE(g_vaoHandle);

  for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];
      const ImDrawIdx* idx_buffer_offset = 0;

      glBindBuffer(GL_ARRAY_BUFFER, g_vboHandle);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_elementsHandle);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);
      if(glGetError() != GL_NO_ERROR){std::cout << "houston we have a binding" << std::endl;}


      for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
      {
          if (pcmd->UserCallback)
          {
              pcmd->UserCallback(cmd_list, pcmd);
          }
          else
          {
              glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
              glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
              glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
          }
          idx_buffer_offset += pcmd->ElemCount;
      }
  }
  if(glGetError() != GL_NO_ERROR){std::cout << "houston we have a drawing" << std::endl;}

  // Restore modified GL state
  glUseProgram(last_program);
  glActiveTexture(last_active_texture);
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindVertexArrayAPPLE(last_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFunc(last_blend_src, last_blend_dst);
  if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
  if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
  if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
  if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    if(glGetError() != GL_NO_ERROR){std::cout << "houston we have a reinitializing" << std::endl;}

}

bool HUD::init()
{
  ImGuiIO& io = ImGui::GetIO();

  io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
  io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
  io.KeyMap[ImGuiKey_A] = SDLK_a;
  io.KeyMap[ImGuiKey_C] = SDLK_c;
  io.KeyMap[ImGuiKey_V] = SDLK_v;
  io.KeyMap[ImGuiKey_X] = SDLK_x;
  io.KeyMap[ImGuiKey_Y] = SDLK_y;
  io.KeyMap[ImGuiKey_Z] = SDLK_z;

  std::cout << "binding draw function" << std::endl;
  io.RenderDrawListsFn = RenderDrawLists;
  io.SetClipboardTextFn = ImGui::SetClipboardText;
  io.GetClipboardTextFn = ImGui::GetClipboardText;

  return true;
}


bool show_test_window = true;
bool show_another_window = false;

void HUD::beginHUD()
{
  if(!g_fontTexture)
  {
    std::cout << "Creating devices" << std::endl;
    createDevices();
  }
  ImGuiIO& io = ImGui::GetIO();

  int w, h, display_w, display_h;

  SDL_GetWindowSize(g_window, &w, &h);
  SDL_GL_GetDrawableSize(g_window, &display_w, &display_h);

  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

  Uint32	time = SDL_GetTicks();
  double current_time = time / 1000.0;
  io.DeltaTime = (float)(1.0f/60.0f);
  time = current_time;

  int mx, my;
  Uint32 MouseMask = SDL_GetMouseState(&mx, &my);
  if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_MOUSE_FOCUS)
      io.MousePos = ImVec2((float)mx, (float)my);
  else
      io.MousePos = ImVec2(-1,-1);

  io.MouseDown[0] = g_mousePressed[0] || (MouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  io.MouseDown[2] = g_mousePressed[2] || (MouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  g_mousePressed[0] = g_mousePressed[1] = g_mousePressed[2] = false;

  io.MouseWheel = g_mouseWheel;
  g_mouseWheel = 0.0f;

  SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

  ImGui::NewFrame();

  {
      static float f = 0.0f;
      ImGui::Text("Hello, world!");
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      if (ImGui::Button("Test Window")) show_test_window ^= 1;
      if (ImGui::Button("Another Window")) show_another_window ^= 1;
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}

void HUD::endHUD()
{
  ImGui::Render();
}

bool HUD::processEvent(SDL_Event* event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
        {
            if (event->wheel.y > 0)
                g_mouseWheel = 1;
            if (event->wheel.y < 0)
                g_mouseWheel = -1;
            return true;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT) g_mousePressed[0] = true;
            if (event->button.button == SDL_BUTTON_RIGHT) g_mousePressed[1] = true;
            if (event->button.button == SDL_BUTTON_MIDDLE) g_mousePressed[2] = true;
            return true;
        }
    case SDL_TEXTINPUT:
        {
            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharactersUTF8(event->text.text);
            return true;
        }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        {
            int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
            io.KeysDown[key] = (event->type == SDL_KEYDOWN);
            io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
            io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
            return true;
        }
    }
    return false;
}

bool HUD::createFontsTexture()
{
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  // Upload texture to graphics system
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGenTextures(1, &g_fontTexture);
  glBindTexture(GL_TEXTURE_2D, g_fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void *)(intptr_t)g_fontTexture;

  // Restore state
  glBindTexture(GL_TEXTURE_2D, last_texture);

  return true;
}


bool HUD::createDevices()
{

  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  // Backup GL state
  GLint last_texture, last_array_buffer, last_vertex_array;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  CheckOpenGLError("Get state", 313);
  m_shader = = std::make_unique<Shader>("hudShader");

  g_shaderHandle = glCreateProgram();
  g_vertHandle = glCreateShader(GL_VERTEX_SHADER);
  g_fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(g_vertHandle, 1, &vertex_shader, 0);
  glShaderSource(g_fragHandle, 1, &fragment_shader, 0);
  glCompileShader(g_vertHandle);
  glCompileShader(g_fragHandle);
  CheckOpenGLError("Shader compile", 348);
  glAttachShader(g_shaderHandle, g_vertHandle);
  glAttachShader(g_shaderHandle, g_fragHandle);
  glLinkProgram(g_shaderHandle);
  GLint status;
  glGetProgramiv(g_shaderHandle, GL_LINK_STATUS, &status);
  if( status == GL_FALSE) printf("Program did not link\n");
  CheckOpenGLError("Program link", 352);

  g_attribLocationTex = glGetUniformLocation(g_shaderHandle, "Texture");
  g_attribLocationProjMtx = glGetUniformLocation(g_shaderHandle, "ProjMtx");
  g_attribLocationPosition = glGetAttribLocation(g_shaderHandle, "Position");
  g_attribLocationUV = glGetAttribLocation(g_shaderHandle, "UV");
  g_attribLocationColor = glGetAttribLocation(g_shaderHandle, "Color");
  CheckOpenGLError("Unis and attribs", 359);

  glGenBuffers(1, &g_vboHandle);
  glGenBuffers(1, &g_elementsHandle);

  glGenVertexArraysAPPLE(1, &g_vaoHandle);
  glBindVertexArrayAPPLE(g_vaoHandle);
  CheckOpenGLError("Bind VAO", 343);
  glBindBuffer(GL_ARRAY_BUFFER, g_vboHandle);
  glEnableVertexAttribArray(g_attribLocationPosition);
  glEnableVertexAttribArray(g_attribLocationUV);
  glEnableVertexAttribArray(g_attribLocationColor);
  CheckOpenGLError("Enable Attrib Arrays", 343);

  #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
  glVertexAttribPointer(g_attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(g_attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(g_attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
  CheckOpenGLError("Attrib pointer", 377);
  #undef OFFSETOF

  createFontsTexture();

  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindVertexArrayAPPLE(last_vertex_array);

  return true;
}

bool HUD::invalidateDevices()
{
    if (g_fontTexture)
    {
        glDeleteTextures(1, &g_fontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        g_fontTexture = 0;
    }
    return true;
}

bool HUD::shutdown()
{
    invalidateDevices();
    ImGui::Shutdown();
    return true;
}
}
