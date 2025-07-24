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
#include "file_dialog.h"

#ifdef _WIN32 //WIN32
#else
	#include <pwd.h>
	#include <unistd.h>
#endif


//To determine the initial path in the filedialog.
//TODO: NOT in windows??
std::string get_homedir() 
{
   #ifdef __APPLE__
      printf("Setting default filedialog path for Macos...\n");
      const char *homeDir = getenv("HOME");

      if (!homeDir) {
           struct passwd* pwd = getpwuid(getuid());
           if (pwd)
              homeDir = pwd->pw_dir;
      }
      return std::string(homeDir);
   #else
      printf("Setting default filedialog path for Windows...\n");
      const char *homeDir = getenv("USERPROFILE");
      return std::string(homeDir);
    #endif
} 

void show_file_dialog_f(MyState & myState)
{
    //Testing this:
    int32_t display_w = 2500;
    int32_t display_h = 2000;
    //glfwGetFramebufferSize(window, &display_w, &display_h);
    IGFD::FileDialogConfig config;

    

    config.path = ".";
    
    std::string home_dir = get_homedir();
    config.filePathName = home_dir+"/"; 
    
    //config.filePathName = "/Users/rtous/";

    
    //printf("opening dialog..."); 

    //by default FILE_DIALOG_LOAD_SINGLE_FILE
    //const char *filters = "Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},.md";
    const char *filters = "Image files (*.png *.gif *.jpg *.jpeg){.png,.gif,.jpg,.jpeg},Source files (*.cpp *.h *.hpp){.cpp,.h,.hpp},.md";
    
    //if (myState.show_file_dialog_video) 
    if (myState.file_dialog_mode == FILE_DIALOG_LOAD_VIDEO) 
       filters = "Video files (*.mp4){.mp4}";

    if (myState.file_dialog_mode == FILE_DIALOG_SAVE_VIDEO) {
       //config.filePathName = "output.mp4";
       config.fileName = "result.mp4"; //this way keeps the extension
       filters = "Video files (*.mp4){.mp4}";
       //filters = nullptr; //TO SELECT A DIRECTORY
    }

    if (myState.file_dialog_mode == FILE_DIALOG_SAVE_VIDEO_FRAMES) {
       //config.filePathName = "output.mp4";
       //config.fileName = "result.mp4"; //this way keeps the extension
       filters = nullptr; //TO SELECT A DIRECTORY
    }

    if (myState.file_dialog_mode == FILE_DIALOG_SAVE_FRAME) {
       config.fileName = "result.png"; //this way keeps the extension
       //config.path = "./output.png";
       filters = "Video files (*.png){.png}";
    }
     
    //ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose a File", filters, config);
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
    
    //if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
          std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName(IGFD_ResultMode_AddIfNoFileExt);
          //std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName(IGFD_ResultMode_AddIfNoFileExt);
          //std::string fileName = ImGuiFileDialog::Instance()->GetFileName(IGFD_ResultMode_AddIfNoFileExt);
          std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
          //std::print("filePathName: {}", filePathName);
          //std::print("filePath: {}", filePath);
          //std::cout << filePath;
          
          //To bypass a strange behavior in which the filePathName is the full path:
          if (filePathName.find_last_of("/\\") != -1)
            filePathName = filePathName.substr(filePathName.find_last_of("/\\") + 1);

          myState.filePathName = filePathName;
          myState.filePath = filePath;
          printf("myState.filePathName=%s\n",myState.filePathName.c_str());
          printf("myState.filePath=%s\n",myState.filePath.c_str());
          myState.file_dialog_file_selected = true;
          // action
        }
        printf("closing dialog..."); 
        // close
        ImGuiFileDialog::Instance()->Close();
        myState.show_file_dialog = false;
        myState.clicked = false;//to avoid the click going through
    }
}
