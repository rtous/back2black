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
#include "gui_utils.h"
#include "state.h"
#include "ImGuiFileDialog.h"
#include "file_dialog.h"
#include "sam_utils.h"
#include <string>
#include "common1.h"
#include "video.h"
#include "data_structures.h"

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


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
static void ShowExampleAppMainMenuBar(bool *show_file_dialog, MyState &myState)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                *show_file_dialog = true;
            }
            if (ImGui::MenuItem("Open Video", "Ctrl+J")) {
                *show_file_dialog = true;
                myState.show_file_dialog_video = true;
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

void compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {
    maskTextures->clear();
    for (auto& mask : storedMasks) {
        sam_image_u8 mask_rgb = { mask.nx, mask.ny, };
        mask_rgb.data.resize(4*mask.nx*mask.ny);
        for (int i = 0; i < mask.nx*mask.ny; ++i) {
            mask_rgb.data[4*i+0] = mask.data[i];
            mask_rgb.data[4*i+1] = mask.data[i];
            mask_rgb.data[4*i+2] = mask.data[i];
            mask_rgb.data[4*i+3] = mask.data[i];
        }        
        //assign color
        int color_id = (last_color_id+1)%256;
        last_color_id = color_id;
        printf("Assigned color id: %d\n", color_id);
        mask_colors->push_back(color_id);
        GLuint newGLTexture = createGLTexture(mask_rgb, GL_RGBA);
        maskTextures->push_back(newGLTexture);
    }
}

static void drawMasks(MyState &myState, const ImGuiViewport* viewport, ImVec2 newPos) {

    //compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {

    //DRAW MASKS
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    for (int i = 0; i < int(myState.maskTextures.size()); ++i) {
        //printf("Drawing mask.\n");
        /*const int r = i == 0 ? 255 : 0;
        const int g = i == 1 ? 255 : 0;
        const int b = i == 2 ? 255 : 0;*/

        /*const int r = (125 + i * 50) % 256;
        const int g = (50 + i * 50) % 256;
        const int b = (200 + i * 50) % 256;*/

        int color_id = myState.masks_colors[i];
        const int r = myState.colors_palette[color_id].r;
        const int g = myState.colors_palette[color_id].g;
        const int b = myState.colors_palette[color_id].b;

        //draw_list->AddImage((void*)(intptr_t)myState.maskTextures[i], ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 172));
        draw_list->AddImage((void*)(intptr_t)myState.maskTextures[i], ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));
    }
}

static void frameWindow(MyState &myState, bool *show_myWindow, const ImGuiViewport* viewport, bool use_work_area, ImGuiWindowFlags flags) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.75f);
    //static bool use_work_area = true;
    //static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    //Create EDITOR window
    ImGui::Begin("FRAME", show_myWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    
    /******* image ******/
    if (myState.img_loaded) {
        //Draw input image (previously loaded)
        //printf("Redrawing image\n");
        //opencv_image2sam(myState.img, myState.aVideo.frames[(rand()%6)+1].img);

        //GLuint tex = createGLTexture(myState.img, GL_RGB);//Done just once at loading
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        
        draw_list->AddImage((void*)(intptr_t)myState.tex, ImVec2(0,0), ImVec2(myState.img.nx, myState.img.ny));
        draw_list->AddCircleFilled(ImVec2(100, 100), 5, IM_COL32(255, 0, 0, 255));
    } else {
        ImGui::Text("Load an image from the File menu.");
    }
    /********************/
    ImGui::End();
}

static void framesListWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + 0, viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("FRAMES");
    ImGui::Text("FRAMES WINDOW"); 

    //const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
    //const char* items[100];
    std::vector<std::string> items; 

    if (myState.aVideo.loaded) {
        for (Frame & aFrame : myState.aVideo.frames) {
            items.push_back(std::to_string(aFrame.order));
            //items[aFrame.order] = std::to_string(aFrame.order);
        }
    }

    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox("listbox 1"))
    {
        int n = 0;
        for (std::string & item : items)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(item.c_str(), is_selected))
                item_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
                //printf("Selected item %s\n", item.c_str());
                if (myState.displayed_frame_num != n) {
                    printf("Changing image %s\n", item.c_str());
                    printf("opencv_image2sam(myState.img, myState.aVideo.frames[%d].img);\n", n);
                    //opencv_image2sam(myState.img, myState.aVideo.frames[n].img);
                    myState.img = myState.aVideo.frames[n].img_sam_format;
                    myState.tex = createGLTexture(myState.img, GL_RGB);
                    myState.displayed_frame_num = n;
                    if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                        //return 1;
                    }
                }
            }
            n++;
        }
        ImGui::EndListBox();
    }
    ImGui::End();
}

static void objectsListWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.33f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("OBJECTS");
    ImGui::Text("OBJECTS WINDOW"); 

    std::vector<std::string> items; 

    //items.push_back("OBJECT 1");

    if (myState.aVideo.loaded) {
        int i = 0;
        for (Object & anObject : myState.aVideo.frames[myState.displayed_frame_num].objects) {
            items.push_back(std::to_string(i));
            //items[aFrame.order] = std::to_string(aFrame.order);
            i++;
        }
    }

    static int item_current_idx = 0; // Here we store our selection data as an index.
    //if (ImGui::BeginListBox("listbox 2"))
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        int n = 0;
        for (std::string & item : items)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(item.c_str(), is_selected)) {
                item_current_idx = n;
                myState.selected_object = item_current_idx;
                //compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {
                compute_mask_textures(myState.img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY, myState.aVideo.frames[myState.displayed_frame_num].objects[myState.selected_object].masks, &myState.masks_colors, myState.last_color_id);

                drawMasks(myState, viewport, newPos);

            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();


                /*//compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {
                compute_mask_textures(myState.img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY, myState.aVideo.frames[myState.displayed_frame_num].objects[myState.selected_object].masks, &myState.masks_colors, myState.last_color_id);

                drawMasks(myState, viewport, newPos);*/


                //printf("Selected item %s\n", item.c_str());
                /*if (myState.displayed_frame_num != n) {
                    printf("Changing image %s\n", item.c_str());
                    printf("opencv_image2sam(myState.img, myState.aVideo.frames[%d].img);\n", n);
                    //opencv_image2sam(myState.img, myState.aVideo.frames[n].img);
                    myState.img = myState.aVideo.frames[n].img_sam_format;
                    myState.tex = createGLTexture(myState.img, GL_RGB);
                    myState.displayed_frame_num = n;
                    if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                        //return 1;
                    }
                }*/
            }
            n++;
        }
        ImGui::EndListBox();
    }
    
     if (myState.aVideo.loaded) {
        //BUTTON
        static int clicked = 0;
        if (ImGui::Button("+"))
            clicked++;
        if (clicked & 1)
        {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
            Object anObject;
            anObject.color[0] = 0.4f;
            anObject.color[1] = 0.7f;
            anObject.color[2] = 0.0f;
            anObject.color[3] = 0.0f;//
            myState.aVideo.frames[myState.displayed_frame_num].objects.push_back(anObject);
            clicked = false;
        }

        //COLOR
        //static float col[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
        /*col[0] = myState.aVideo.frames[myState.displayed_frame_num].objects[item_current_idx].color[0];
        col[1] = myState.aVideo.frames[myState.displayed_frame_num].objects[item_current_idx].color[0];
        col[2] = myState.aVideo.frames[myState.displayed_frame_num].objects[item_current_idx].color[0];
        col[3] = myState.aVideo.frames[myState.displayed_frame_num].objects[item_current_idx].color[0];
        */
        ImGui::SameLine(); HelpMarker(
            "Click on the color square to open a color picker.\n"
            "Click and hold to use drag and drop.\n"
            "Right-click on the color square to show options.\n"
            "CTRL+click on individual component to input value.\n");
        ImGui::ColorEdit4("color 2", myState.aVideo.frames[myState.displayed_frame_num].objects[item_current_idx].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        //ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

        //ImGui::ColorEdit4("color 2", col);
    
    //
    }
    

    ImGui::End();
}

static void finishingWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.66f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("FINISHING");
    ImGui::Text("FINISHING WINDOW"); 

    
    

    ImGui::End();
}

void fileDialog(MyState &myState, bool *show_file_dialog) {
    //Show file dialog
    if (*show_file_dialog) {
        show_file_dialog_f(show_file_dialog, myState);
    }

    //Check if opening file
    if (myState.openFile && !myState.show_file_dialog_video) {
        printf("OPENING IMAGE FILE\n");
        myState.openFile = false;
        std::string fileName = myState.filePath + "/" + myState.filePathName;
        if (!load_image_samformat_from_file(fileName, myState.img)) {
            printf("failed to load image from '%s'\n", fileName.c_str());  
        } else {
            printf("successfully loaded image from '%s'\n", fileName.c_str());
            myState.img_loaded = true;
            //Create the OpenGL texture here, just once.
            //Otherwise you will get a memory leak
            myState.tex = createGLTexture(myState.img, GL_RGB);
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
                //First SAM computes the embedding of the whole image
                if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                    fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                    //return 1;
                }
                printf("t_compute_img_ms = %d ms\n", myState.a_sam_state->t_compute_img_ms);
            }
        }
    } else if (myState.openFile && myState.show_file_dialog_video) {
        printf("OPENING VIDEO FILE\n");
        myState.openFile = false;
        std::string fullPath = myState.filePath + "/" + myState.filePathName;
        std::vector<cv::Mat> frames;
        //Video aVideo;
        read_video(fullPath, myState.aVideo);
        //opencv_image2sam(myState.img, myState.aVideo.frames[0].img);
        myState.img = myState.aVideo.frames[0].img_sam_format;
        myState.tex = createGLTexture(myState.img, GL_RGB);
        myState.displayed_frame_num = 0;
        myState.img_loaded = true;
        //Create a single default object
        Object anObject;
        myState.aVideo.frames[0].objects.push_back(anObject);
        myState.aVideo.frames[0].objects[0].color[0] = 0.4f;
        myState.aVideo.frames[0].objects[0].color[1] = 0.7f;
        myState.aVideo.frames[0].objects[0].color[2] = 0.0f;
        myState.aVideo.frames[0].objects[0].color[3] = 0.0f;//no transparency
        if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
            fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
            //return 1;
        }
    }
}







static void masksWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    if (myState.img_loaded) {
        ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.75f);
        static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
        ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.5f), viewport->WorkPos.y + 0);
        ImGui::SetNextWindowPos(newPos);
        ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
        ImGui::Begin("MASKS");
        ImGui::Text("MASKS WINDOW");
        
        //CHECK MOUSE CLICKS
        if (myState.clicked) {
            printf("Mouse clicked at (%d, %d)\n", myState.clickedX, myState.clickedY);
            printf("Image x = %d, y = %d\n", myState.img.nx, myState.img.ny);

            //TODO make the position relative to the window (now works because image is displayed at 0,0)
            if (myState.clickedX > 0 && myState.clickedX < myState.img.nx && myState.clickedY > 0 && myState.clickedY < myState.img.ny) {
                //compute_masks(myState.img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY, myState.masks, &myState.masks_colors, myState.last_color_id);
                compute_masks(myState.img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY, myState.aVideo.frames[myState.displayed_frame_num].objects[myState.selected_object].masks, &myState.masks_colors, myState.last_color_id);
            }
            myState.clicked = false;
        }

        drawMasks(myState, viewport, newPos);
        
        ImGui::End();
    }
}
//Main editor method (called within the main loop)
void editor(bool *show_myWindow, bool *show_file_dialog, MyState &myState) //WARNING: this is executed within the main loop
{
    //Main Menu
    ShowExampleAppMainMenuBar(show_file_dialog, myState);

    static bool use_work_area = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f);
    

    //printf("viewport->WorkSize.x=%f\n", viewport->WorkSize.x);
    //printf("viewport->WorkSize.y=%f\n", viewport->WorkSize.y);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.6f, viewport->WorkSize.y * 0.6f);
    
    //IMAGE window
    frameWindow(myState, show_myWindow, viewport, use_work_area, flags);
    
    //FILE dialog
    fileDialog(myState, show_file_dialog);

    //MASKS window
    masksWindow(myState, viewport, flags, use_work_area);

    //FRAMES LIST window
    framesListWindow(myState, viewport, flags, use_work_area);
    
    //OBJECTS LIST window
    objectsListWindow(myState, viewport, flags, use_work_area);
    
    //FINISHING window
    finishingWindow(myState, viewport, flags, use_work_area);
    
    //ImGui::End();

    /* 
        If the user clicks on the image then the masksWindow(...) methods
        computes the masks and add them to the myState.aVideo.frames[myState.displayed_frame_num].objects[myState.selected_object].masks
        myState.selected_object=0 by default
        myState.displayed_frame_num=-1 by default but it's set to zero when the video is loaded
    */
}
