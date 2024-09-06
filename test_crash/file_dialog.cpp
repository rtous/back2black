#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

//#include "sam.h"
//#include "utils.h"
#include "ImGuiFileDialog.h"
#include <iostream>

#include "state.h"


void show_file_dialog_f(bool *show_file_dialog, MyState & myState)
{
    //Testing this:
    int32_t display_w = 2000;
    int32_t display_h = 500;
    //glfwGetFramebufferSize(window, &display_w, &display_h);
    IGFD::FileDialogConfig config;
    config.path = ".";
    //printf("opening dialog..."); 

    //const char *filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
    const char *filters = "Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},.md";
    
    if (myState.show_file_dialog_video) 
       filters = "Video files (*.mp4){.mp4}";
     
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a File", filters, config);
    //ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp,.png,.jpg", config);
    // display
    ImVec2 minSize = ImVec2(0, 0);
    ImVec2 maxSize = ImVec2(FLT_MAX, FLT_MAX);
    maxSize = ImVec2((int)display_w* 0.7f, (int)display_h* 0.7f);
    minSize = ImVec2((int)display_w* 0.25f, (int)display_h* 0.25f);
    //maxSize = ImVec2((float)200, (float)200) * 0.7f;
    //minSize = maxSize * 0.25f;
    //maxSize = ImVec2(300, 300);
    //minSize = ImVec2(100, 100);
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
          std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
          std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
          //std::print("filePathName: {}", filePathName);
          //std::print("filePath: {}", filePath);
          std::cout << filePath;
          myState.filePathName = filePathName;
          myState.filePath = filePath;
          myState.openFile = true;
          // action
        }
        printf("closing dialog..."); 
        // close
        ImGuiFileDialog::Instance()->Close();
        *show_file_dialog = false;
    }
}
