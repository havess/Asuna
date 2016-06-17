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
#include "../../includes/Asuna/HUD/HUD.hpp"

namespace Asuna{

HUD::HUD(const vec2 dimensions,SDL_Window* window, const std::string title){
  m_hudData = new HUD_Data(dimensions,window, title);
}

HUD::~HUD(){
  delete m_hudData;
  shutdown();
}

void RenderDrawLists(ImDrawData* draw_data){
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if(fb_width == 0 || fb_height == 0){std::cout<< "frame buffer was zero" << std::endl; return;}
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // We are using the OpenGL fixed pipeline to make the example code simpler to read!
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context

    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);


    if(glGetError() != GL_NO_ERROR){std::cout << "houston we have a problem" << std::endl;}

}

bool HUD::init(){
  ImGuiIO& io = ImGui::GetIO();
  m_hudData->initialized = true;

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

  io.RenderDrawListsFn = RenderDrawLists;
  io.SetClipboardTextFn = ImGui::SetClipboardText;
  io.GetClipboardTextFn = ImGui::GetClipboardText;

  return true;
}


bool show_test_window = true;
bool show_another_window = false;

void HUD::beginHUD(){
  if(!m_hudData->fontTexture){
    std::cout << "Creating devices" << std::endl;
    createDevices();
  }
  ImGuiIO& io = ImGui::GetIO();

  int w, h, display_w, display_h;

  SDL_GetWindowSize(m_hudData->window, &w, &h);
  SDL_GL_GetDrawableSize(m_hudData->window, &display_w, &display_h);

  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

  Uint32	time = SDL_GetTicks();
  double current_time = time / 1000.0;
  io.DeltaTime = m_hudData->time > 0.0 ? (float)(current_time - m_hudData->time) : (float)(1.0f/60.0f);
  m_hudData->time = current_time;

  int mx, my;
  Uint32 MouseMask = SDL_GetMouseState(&mx, &my);
  if (SDL_GetWindowFlags(m_hudData->window) & SDL_WINDOW_MOUSE_FOCUS)
      io.MousePos = ImVec2((float)mx, (float)my);
  else
      io.MousePos = ImVec2(-1,-1);

  io.MouseDown[0] = m_hudData->mousePressed[0] || (MouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  io.MouseDown[2] = m_hudData->mousePressed[2] || (MouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  m_hudData->mousePressed[0] = m_hudData->mousePressed[1] = m_hudData->mousePressed[2] = false;

  io.MouseWheel = m_hudData->mouseWheel;
  m_hudData->mouseWheel = 0.0f;

  SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

  ImGui::NewFrame();
}

void HUD::endHUD(){
  ImGui::Render();
}

bool HUD::processEvent(SDL_Event* event){
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
        {
            if (event->wheel.y > 0)
                m_hudData->mouseWheel = 1;
            if (event->wheel.y < 0)
                m_hudData->mouseWheel = -1;
            return true;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT) m_hudData->mousePressed[0] = true;
            if (event->button.button == SDL_BUTTON_RIGHT) m_hudData->mousePressed[1] = true;
            if (event->button.button == SDL_BUTTON_MIDDLE) m_hudData->mousePressed[2] = true;
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

bool HUD::createFontsTexture(){
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  // Upload texture to graphics system
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGenTextures(1, &m_hudData->fontTexture);
  glBindTexture(GL_TEXTURE_2D, m_hudData->fontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void *)(intptr_t)m_hudData->fontTexture;

  // Restore state
  glBindTexture(GL_TEXTURE_2D, last_texture);

  return true;
}


bool HUD::createDevices(){

  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  // Backup GL state
  GLint last_texture, last_array_buffer, last_vertex_array;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

  const GLchar *vertex_shader =
      "#version 330\n"
      "uniform mat4 ProjMtx;\n"
      "in vec2 Position;\n"
      "in vec2 UV;\n"
      "in vec4 Color;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "	Frag_UV = UV;\n"
      "	Frag_Color = Color;\n"
      "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* fragment_shader =
      "#version 330\n"
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
      "}\n";

  m_hudData->shaderHandle = glCreateProgram();
  m_hudData->vertHandle = glCreateShader(GL_VERTEX_SHADER);
  m_hudData->fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(m_hudData->vertHandle, 1, &vertex_shader, 0);
  glShaderSource(m_hudData->fragHandle, 1, &fragment_shader, 0);
  glCompileShader(m_hudData->vertHandle);
  glCompileShader(m_hudData->fragHandle);
  glAttachShader(m_hudData->shaderHandle, m_hudData->vertHandle);
  glAttachShader(m_hudData->shaderHandle, m_hudData->fragHandle);
  glLinkProgram(m_hudData->shaderHandle);

  m_hudData->attribLocationTex = glGetUniformLocation(m_hudData->shaderHandle, "Texture");
  m_hudData->attribLocationProjMtx = glGetUniformLocation(m_hudData->shaderHandle, "ProjMtx");
  m_hudData->attribLocationPosition = glGetAttribLocation(m_hudData->shaderHandle, "Position");
  m_hudData->attribLocationUV = glGetAttribLocation(m_hudData->shaderHandle, "UV");
  m_hudData->attribLocationColor = glGetAttribLocation(m_hudData->shaderHandle, "Color");

  glGenBuffers(1, &m_hudData->vertHandle);
  glGenBuffers(1, &m_hudData->elementsHandle);

  glGenVertexArrays(1, &m_hudData->vaoHandle);
  glBindVertexArray(m_hudData->vaoHandle);
  glBindBuffer(GL_ARRAY_BUFFER, m_hudData->vertHandle);
  glEnableVertexAttribArray(m_hudData->attribLocationPosition);
  glEnableVertexAttribArray(m_hudData->attribLocationUV);
  glEnableVertexAttribArray(m_hudData->attribLocationColor);

  #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
  glVertexAttribPointer(m_hudData->attribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(m_hudData->attribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(m_hudData->attribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
  #undef OFFSETOF

  ImGui_ImplSdlGL3_CreateFontsTexture();

  // Restore modified GL state
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindVertexArray(last_vertex_array);

  return true;
}

bool HUD::invalidateDevices()
{
    if (m_hudData->fontTexture)
    {
        glDeleteTextures(1, &m_hudData->fontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        m_hudData->fontTexture = 0;
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
