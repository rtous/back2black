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

#include "sam.h"
#include "utils.h"
#include "state.h"
#include "ImGuiFileDialog.h"
#include "file_dialog.h"
#include "sam_utils.h"
#include <string>


//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------
// - ShowExampleAppMainMenuBar()
// - ShowExampleMenuFile()
//-----------------------------------------------------------------------------

// Demonstrate creating a "main" fullscreen menu bar and populating it.
// Note the difference between BeginMainMenuBar() and BeginMenuBar():
// - BeginMenuBar() = menu-bar inside current window (which needs the ImGuiWindowFlags_MenuBar flag!)
// - BeginMainMenuBar() = helper to create menu-bar-sized window at the top of the main viewport + call BeginMenuBar() into it.
static void ShowExampleAppMainMenuBar(sam_image_u8 *img, bool *img_loaded, bool *show_file_dialog)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                *show_file_dialog = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}



void editor(bool *show_myWindow, sam_image_u8 *img, bool *img_loaded, bool *show_file_dialog, MyState &myState) //WARNING: this is executed within the main loop
{
    //Main Menu
    ShowExampleAppMainMenuBar(img, img_loaded, show_file_dialog);

    //An example window
    static bool use_work_area = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);

    ImGui::Begin("EDITOR", show_myWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    
    /******* image ******/
    //load_image_from_file("img.jpg", *img);
    if (*img_loaded) {
        //printf("drawing image\n");
        
        GLuint tex = createGLTexture(*img, GL_RGB);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddImage((void*)(intptr_t)tex, ImVec2(0,0), ImVec2(img->nx, img->ny));
        draw_list->AddCircleFilled(ImVec2(100, 100), 5, IM_COL32(255, 0, 0, 255));

        /******* check click ******/

        /**************************/
        
    } else {
        //printf("not drawing image\n");
    }
    /********************/
    
    //Show file dialog
    if (*show_file_dialog) {
            show_file_dialog_f(show_file_dialog, myState);
    }

    //Check if opening file
    if (myState.openFile) {
        printf("OPENING FILE\n");
        myState.openFile = false;
        std::string fileName = myState.filePath + "/" + myState.filePathName;
        if (!load_image_from_file(fileName, *img)) {
            printf("failed to load image from '%s'\n", fileName.c_str());  
        } else {
            printf("successfully loaded image from '%s'\n", fileName.c_str());
            *img_loaded = true;
            // init SDL video subsystem to get the screen size
            /*if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                fprintf(stderr, "Error: %s\n", SDL_GetError());
            }
            // resize img when exceeds the screen
            downscale_img_to_screen(img, NULL);*/
            //set_params(&myState.params, fileName);
            //std::shared_ptr<sam_state> state = sam_load_model(myState.params);
            if (!myState.a_sam_state) {
                fprintf(stderr, "%s: failed to load model\n", __func__);
                //return 1;
            } else {
                printf("t_load_ms = %d ms\n", myState.a_sam_state->t_load_ms);
                if (!sam_compute_embd_img(*img, myState.params.n_threads, *myState.a_sam_state)) {
                    fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                    //return 1;
                }
                printf("t_compute_img_ms = %d ms\n", myState.a_sam_state->t_compute_img_ms);
                compute_masks(*img, myState.params, *myState.a_sam_state, &myState.maskTextures, 100, 100);
                //draw masks
                //ImGui::Begin("EDITOR", show_myWindow);
                
                
                /*ImGui::Text("MASKS WINDOW");
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                compute_masks(*img, myState.params, *myState.a_sam_state, &myState.maskTextures);
                for (int i = 0; i < int(myState.maskTextures.size()); ++i) {
                    printf("Drawing mask.\n");
                    const int r = i == 0 ? 255 : 0;
                    const int g = i == 1 ? 255 : 0;
                    const int b = i == 2 ? 255 : 0;
                    draw_list->AddImage((void*)(intptr_t)myState.maskTextures[i], ImVec2(0,0), ImVec2(img->nx, img->ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 172));
                }*/
                
                //ImGui::End();
            }
        }
    }



    /******* masks ******/
    if (*img_loaded && myState.clicked) {
        compute_masks(*img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY);
        myState.clicked = false;
    }
    if (*img_loaded) {
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        ImVec2 newPos = ImVec2(viewport->WorkPos.x + 500, viewport->WorkPos.y + 0);
        ImGui::SetNextWindowPos(newPos);
        ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y);
        ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
        ImGui::Begin("MASKS");
        ImGui::Text("MASKS WINDOW");
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        //compute_masks(*img, myState.params, *myState.a_sam_state, &myState.maskTextures);
        for (int i = 0; i < int(myState.maskTextures.size()); ++i) {
            printf("Drawing mask.\n");
            const int r = i == 0 ? 255 : 0;
            const int g = i == 1 ? 255 : 0;
            const int b = i == 2 ? 255 : 0;
            draw_list->AddImage((void*)(intptr_t)myState.maskTextures[i], ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+img->nx, newPos[1]+img->ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 172));
        }
        ImGui::End();
    }
    /********************/
    
    
    /****************************/

    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
        *show_myWindow = false;
    ImGui::End();

}
