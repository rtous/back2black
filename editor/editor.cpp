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
static void ShowExampleAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                //load image
                sam_image_u8 new_img;
                //test("a", new_img);
                if (!load_image_from_file("img.jpg", new_img)) {
                    printf("failed to load image from '%s'\n", "img.jpg");
            }
    /*else {
        SDL_SetWindowTitle(window, "Encoding new img...");
        downscale_img_to_screen(new_img, window);
        if (!sam_compute_embd_img(new_img, params.n_threads, state)) {
            printf("failed to compute encoded image\n");
        }
        printf("t_compute_img_ms = %d ms\n", state.t_compute_img_ms);

        tex = createGLTexture(new_img, GL_RGB);

        SDL_SetWindowSize(window, new_img.nx, new_img.ny);
        SDL_SetWindowTitle(window, title);
        img = std::move(new_img);
        computeMasks = true;
    }*/
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



void editor(bool *show_myWindow)
{
    //Main Menu
    ShowExampleAppMainMenuBar();

    //An example window
    static bool use_work_area = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);

    ImGui::Begin("EDITOR", show_myWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    
    


    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
        *show_myWindow = false;
    ImGui::End();

}
