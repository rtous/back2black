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
#include "simplify.h"

/*
    MyState &myState contains the state of the editor.
    myState.aVideo contains the information about frames, masks, masks, etc.
    (defined in common/data_structures.h)

    Concepts:

    For a given video frame:
        - N masks. An mask = set of masks with the same color.
            - N masks. A mask = a single binary image but can have multiple segments (contours)
    
    
    Panels:

    The left panel is for adding or removing masks for the SELECTED mask.
    When the user clicks on the image masks their related texures are computed, assigned to the SELECTED mask and displayed.
    When the user selects a different mask the mask textures of the new mask are displayed (but not recomputed)
    The OpenGL textures of the masks are stored in myState.masksMaskTextures instead of in the mask to avoid adding SDL dependency to projects that use commons.h  

    Workflow:

    When the user loads the video the first frame is precomputed.
    When the user clicks on the frame one mask is computed and added/removed from the mask.
    If a mask is added the simplify algorithm is applied to it.
    When the user selects a different frame and clicks it the frame is precomputed.
*/




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
                myState.file_dialog_mode = FILE_DIALOG_LOAD_SINGLE_FILE;
                myState.clicked = false;//to avoid the click going through
            }
            if (ImGui::MenuItem("Open Video", "Ctrl+J")) {
                *show_file_dialog = true;
                //myState.show_file_dialog_video = true;
                myState.file_dialog_mode = FILE_DIALOG_LOAD_VIDEO;
                myState.clicked = false;//to avoid the click going through
            }
            if (ImGui::MenuItem("Save Video", "Ctrl+J")) {
                *show_file_dialog = true;
                //myState.show_file_dialog_save_video = true;
                myState.file_dialog_mode = FILE_DIALOG_SAVE_VIDEO;
                myState.clicked = false;//to avoid the click going through
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
        if (ImGui::BeginMenu("Tools"))
        {
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Propagate masks to all frames", "Ctrl+P")) {
                myState.propagate = true;
                myState.clicked = false;//to avoid the click go into the frame
                
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

/*
//NOT USED. Textures are computed at the same time masks are computed
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
*/

static void drawAllMasks(MyState &myState, const ImGuiViewport* viewport, ImVec2 newPos, bool simplified) {

    //compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {

    //DRAW MASKS
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    //printf("Found %d masks.\n", myState.aVideo.frames[myState.selected_frame].masks.size());
    for (int j = 0; j < int(myState.aVideo.frames[myState.selected_frame].masks.size()); ++j) {
        //printf("Found %d masks for mask %d.\n", myState.aVideo.frames[myState.selected_frame].masks[j].maskTextures.size(), j);
        //printf("Drawing mask.\n");
        /*const int r = i == 0 ? 255 : 0;
        const int g = i == 1 ? 255 : 0;
        const int b = i == 2 ? 255 : 0;*/

        /*const int r = (125 + i * 50) % 256;
        const int g = (50 + i * 50) % 256;
        const int b = (200 + i * 50) % 256;*/

        //int color_id = myState.masks_colors[i];
        //const int r = myState.colors_palette[color_id].r;
        //const int g = myState.colors_palette[color_id].g;
        //const int b = myState.colors_palette[color_id].b;

        Mask aMask = myState.aVideo.frames[myState.selected_frame].masks[j];

        const int r = aMask.color[0]*255;
        const int g = aMask.color[1]*255;
        const int b = aMask.color[2]*255;
        //myState.aVideo.frames[0].masks[0].color[3] = 0.0f;/

        //draw_list->AddImage((void*)(intptr_t)myState.maskTextures[i], ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));
        //draw_list->AddImage((void*)(intptr_t)myState.masksMaskTextures[j][i], ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));
        //draw_list->AddImage(maskTextures, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));

        //void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col) 

        if (simplified)
            //ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
            draw_list->AddImage((void*)(intptr_t)aMask.simplifiedMaskTexture, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));
        else 
            draw_list->AddImage((void*)(intptr_t)aMask.maskTexture, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));  
    }
}



static void frameWindow(MyState &myState, bool *show_myWindow, const ImGuiViewport* viewport, bool use_work_area, ImGuiWindowFlags flags) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.75f);
    
    myState.img_frame_w = size.x;
    myState.img_frame_h = size.y;

    //static bool use_work_area = true;
    //static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + 0, viewport->WorkPos.y + 0);
    //ImVec2 newPos = ImVec2(use_work_area ? viewport->WorkPos : viewport->Pos);
    
    //printf("use_work_area  = %d\n", use_work_area);
    ImVec2 newPos = ImVec2(use_work_area ? viewport->WorkPos : viewport->Pos);
    /*
    printf("newPos.x  = %f\n", newPos.x);
    printf("newPos.y  = %f\n", newPos.y);
    printf("viewport->Pos.x  = %f\n", viewport->Pos.x);
    printf("viewport->Pos.y  = %f\n", viewport->Pos.y);
    printf("viewport->WorkPos.x  = %f\n", viewport->WorkPos.x);
    printf("viewport->WorkPos.y  = %f\n", viewport->WorkPos.y);
    */

    ImGui::SetNextWindowPos(newPos);
    //ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    //Create EDITOR window
    //ImGui::Begin("FRAME", show_myWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Begin("FRAME", show_myWindow, ImGuiWindowFlags_NoTitleBar);
    //myState.img_frame_pos_x = ;
    //myState.img_frame_pos_y;

    /******* image ******/
    if (myState.img_loaded) {
        //Draw input image (previously loaded)
        //printf("Redrawing image\n");
        //opencv_image2sam(myState.img, myState.aVideo.frames[(rand()%6)+1].img);

        //GLuint tex = createGLTexture(myState.img, GL_RGB);//Done just once at loading
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        Frame & aFrame = myState.aVideo.frames[myState.selected_frame];

        //testing relative pos
        //ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
        //ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
        //ImGui::GetWindowPos()
        //
        //int absolute_x = myState.img_frame_pos_x;//ImGui::GetWindowWidth()
        //int absolute_y = myState.img_frame_pos_y;

        draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImVec2(viewport->WorkPos.x+myState.img.nx, viewport->WorkPos.y+myState.img.ny));
        //draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(newPos.x, newPos.y), ImVec2(myState.img.nx, myState.img.ny));
        //draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(myState.img_frame_pos_x,myState.img_frame_pos_y), ImVec2(myState.img.nx, myState.img.ny));
        //draw_list->AddCircleFilled(ImVec2(100, 100), 5, IM_COL32(255, 0, 0, 255));
    } else {
        ImGui::Text("Load an image from the File menu.");
    }
    /********************/

    if (myState.img_loaded) {  
        if (myState.clicked && ImGui::IsWindowFocused()) {
            printf("Mouse clicked at (%d, %d)\n", myState.clickedX, myState.clickedY);
            printf("Image x = %d, y = %d\n", myState.img.nx, myState.img.ny);

            //Mask & selectedMask = myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask];

            //TODO make the position relative to the window (now works because image is displayed at 0,0)
            if (myState.clickedX > viewport->WorkPos.x && myState.clickedX < viewport->WorkPos.x+myState.img.nx && myState.clickedY > viewport->WorkPos.y && myState.clickedY < viewport->WorkPos.y+myState.img.ny) {
                
                //When click, need to ensure that the precomputed frame is this one
                if (myState.frame_precomputed != myState.selected_frame) {
                    printf("clicked but frame_precomputed (%d) != selected_frame (%d) changed so precomputing first frame...\n", myState.frame_precomputed, myState.selected_frame);
                    if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                        fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                        //return 1;
                    }
                    myState.frame_precomputed = myState.selected_frame;
                }

                //compute_masks(myState.img, myState.params, *myState.a_sam_state, &myState.maskTextures, myState.clickedX, myState.clickedY, myState.masks, &myState.masks_colors, myState.last_color_id);
                
                //Each time the user clicks we compute the masks and OpenGL textures
                //int R = selectedMask.color[0]*255;
                //int G = selectedMask.color[1]*255;
                //int B = selectedMask.color[2]*255;
                int R = 100;
                int G = 100;
                int B = 100;
                //printf("myState.selected_frame=%d,myState.selected_mask=%d\n", myState.selected_frame, myState.selected_mask);
                printf("R=%d,G=%d,B=%d\n", R, G, B);
                //compute_masks(myState.img, myState.params, *myState.a_sam_state, &selectedMask.maskTextures, myState.clickedX, myState.clickedY, selectedMask.masks, &myState.masks_colors, myState.last_color_id, R, G, B, &selectedMask.simplifiedMaskTextures);
                int absoluteX = myState.clickedX-viewport->WorkPos.x;
                int absoluteY = myState.clickedY-viewport->WorkPos.y;
                compute_mask_and_textures(myState.aVideo.frames[myState.selected_frame], myState.params, *myState.a_sam_state, absoluteX, absoluteY, R, G, B);

                //printf("Computed masks. selectedMask.maskTextures.size()=%d\n", selectedMask.maskTextures.size());

                //Compute the textures for the mask masks
                //compute_mask_textures(myState.img, myState.params, *myState.a_sam_state, &myState.masksMaskTextures[myState.selected_mask], myState.clickedX, myState.clickedY, myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask].masks, &myState.masks_colors, myState.last_color_id);


            }
            myState.clicked = false;
        }
    
        //viewport->Pos origin without menu bars, etc.
        //viewport->WorkPos origin after menu bars, etc.
        //The window is located in WorkPos

        //Only draw masks of selected mask
        //drawMasks(myState, viewport, viewport->Pos);
        
        //Draw all masks
        
        /*printf("viewport->Pos.x  = %f\n", viewport->Pos.x);
        printf("viewport->Pos.y  = %f\n", viewport->Pos.y);
        printf("viewport->WorkPos.x  = %f\n", viewport->WorkPos.x);
        printf("viewport->WorkPos.y  = %f\n", viewport->WorkPos.y);
        */
        //ImVec2 newPos = ImVec2(use_work_area ? viewport->WorkPos : viewport->Pos);
        
        drawAllMasks(myState, viewport, viewport->WorkPos, false);
        //drawAllMasks(myState, viewport, newPos, false);

    }


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
                if (myState.selected_frame != n) {
                    printf("Changing image %s\n", item.c_str());
                    printf("opencv_image2sam(myState.img, myState.aVideo.frames[%d].img);\n", n);
                    //opencv_image2sam(myState.img, myState.aVideo.frames[n].img);
                    myState.img = myState.aVideo.frames[n].img_sam_format;
                    //myState.tex = createGLTexture(myState.img, GL_RGB);
                    myState.selected_frame = n;
                    myState.selected_mask = 0;
                    //The first time we precompute the frame 0
                    //later we wait till a click
                    if (myState.frame_precomputed == -1) {
                        printf("First time so precomputing first frame...\n");
                        if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                            fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                            //return 1;
                        }
                        printf("First frame precomputed.\n");
                        myState.frame_precomputed = 0;
                    }
                    //drawAllMasks(myState, viewport, viewport->Pos, false);
                }
            }
            n++;
        }
        ImGui::EndListBox();
    }
    ImGui::End();
}

static void masksListWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.33f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("MASKS");
    ImGui::Text("MASKS WINDOW"); 
    
    std::vector<std::string> items; 

    //items.push_back("MASK 1");

    if (myState.aVideo.loaded) {
        int i = 0;
        for (Mask & aMask : myState.aVideo.frames[myState.selected_frame].masks) {
            items.push_back(std::to_string(i));
            //items[aFrame.order] = std::to_string(aFrame.order);
            i++;
        }
    }

    
    if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
    {
        int i = 0;
        for (std::string & item : items)
        {
            const bool is_selected = (myState.selected_mask == i);
            if (ImGui::Selectable(item.c_str(), is_selected)) {
                //Selectable returns true if item is clicked
                //item_current_idx = i;
                myState.selected_mask = i;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
            i++;
        }
        ImGui::EndListBox();
    }
    /*
    float colorP[4];
    const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD"};
    static int item_selected_idx = 0; // Here we store our selected data as an index.
    static bool item_highlight = false;
    int item_highlighted_idx = -1;
    if (ImGui::BeginListBox(""))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            
            const bool is_selected = (item_selected_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                item_selected_idx = n;
            //ImGui::SameLine();
            //ImGui::ColorEdit4("color 2", colorP, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            //ImGui::Text("a text"); 
            if (item_highlight && ImGui::IsItemHovered())
                item_highlighted_idx = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
            ImGui::EndChild();
            ImGui::BeginChild("", ImVec2(0.0f, 100), ImGuiChildFlags_ResizeY); 
            //ImGui::SameLine();
            ImGui::ColorEdit4("color 2", colorP, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            ImGui::EndChild();
        }
        ImGui::EndListBox();
    }
    ImGui::SameLine();
    if (ImGui::BeginListBox(""))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            const bool is_selected = (item_selected_idx == n);
            if (ImGui::Selectable(items[n], is_selected))
                item_selected_idx = n;
            ImGui::Text("a text"); 
            if (item_highlight && ImGui::IsItemHovered())
                item_highlighted_idx = n;
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    */
    
    //ADDING A NEW MASK IF THE USER CLICKS +
     if (myState.aVideo.loaded) {
        //BUTTON
        static int clicked = 0;
        if (ImGui::Button("+"))
            clicked++;
        if (clicked & 1)
        {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
            Mask aMask;
            //aMask.color[0] = 0.4f;
            //aMask.color[1] = 0.7f;
            //aMask.color[2] = 0.0f;
            //aMask.color[3] = 0.0f;//
            myState.aVideo.frames[myState.selected_frame].masks.push_back(aMask);
            
            /*
            //Add a vector of textures to the mask (not directly in the mask to avoid mixing the common lib with SDL)
            std::vector<GLuint> maskTextures;
            myState.masksMaskTextures.push_back(maskTextures);
            std::vector<GLuint> simplifiedMaskTextures;
            myState.masksSimplifiedMaskTextures.push_back(simplifiedMaskTextures);
            */

            //When a new mask is added automatically select it
            myState.selected_mask = myState.aVideo.frames[myState.selected_frame].masks.size()-1;       
            //drawMasks(myState, viewport, newPos);//Necessary??

            clicked = false;
        }

        //COLOR
        //static float col[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
        /*col[0] = myState.aVideo.frames[myState.selected_frame].masks[item_current_idx].color[0];
        col[1] = myState.aVideo.frames[myState.selected_frame].masks[item_current_idx].color[0];
        col[2] = myState.aVideo.frames[myState.selected_frame].masks[item_current_idx].color[0];
        col[3] = myState.aVideo.frames[myState.selected_frame].masks[item_current_idx].color[0];
        */
        //If there are mask then show color picker
        if (myState.aVideo.frames[myState.selected_frame].masks.size()>0) {
            ImGui::SameLine(); HelpMarker(
                "Click on the color square to open a color picker.\n"
                "Click and hold to use drag and drop.\n"
                "Right-click on the color square to show options.\n"
                "CTRL+click on individual component to input value.\n");
            ImGui::ColorEdit4("color 2", myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            //ImGui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

            //ImGui::ColorEdit4("color 2", col);
        }
    //
    }
    

    ImGui::End();
}

static void finishingWindow(MyState &myState, bool *show_myWindow, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.75f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.66f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.5f), viewport->WorkPos.y + 0);
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("FINISHING", show_myWindow, ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("FINISHING WINDOW"); 

    
    if (myState.img_loaded) {  
        //Origin considering the bars 
        ImVec2 newPos = ImVec2(viewport->Pos.x + (viewport->WorkSize.x * 0.5f), viewport->Pos.y + 0);
        drawAllMasks(myState, viewport, ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.5f), viewport->WorkPos.y + 0), true);
    }

    ImGui::End();
}

/*
    Show file dialog or process a file related request 
*/
/*void propagate(MyState &myState) {
    //Show file dialog
    //if (myState.propagate) {
    propagate_masks2(myState.aVideo.frames, *myState.a_sam_state, myState.params.n_threads);
        myState.propagate = false;
    }
}*/

/*
    Show file dialog or process a file related request 
*/
void fileDialog(MyState &myState, bool *show_file_dialog) {
    //Show file dialog
    if (*show_file_dialog) {
        //TODO: this function always does the same thing and sets myState.filePath and myState.openFile = true;
        //Maybe should be customized for each type of action
        show_file_dialog_f(show_file_dialog, myState);
    }

    //Check if opening file
    if (myState.file_dialog_file_selected && myState.file_dialog_mode == FILE_DIALOG_LOAD_SINGLE_FILE) {
        printf("OPENING IMAGE FILE\n");
        myState.file_dialog_file_selected = false;
        std::string fileName = myState.filePath + "/" + myState.filePathName;
        //from common lib
        //NOTE will be rendered in frameWindow()
        if (!load_image_samformat_from_file(fileName, myState.img)) {
            printf("failed to load image from '%s'\n", fileName.c_str());  
        } else {
            printf("successfully loaded image from '%s'\n", fileName.c_str());
            myState.img_loaded = true;
            if (!myState.a_sam_state) {
                fprintf(stderr, "%s: failed to load model\n", __func__);
                //return 1;
            } else {
                printf("t_load_ms = %d ms\n", myState.a_sam_state->t_load_ms);
                //First SAM computes the embedding of the whole image
                printf("(single photo) Precomputing first frame...\n");
                if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                    fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                    //return 1;
                }
                printf("t_compute_img_ms = %d ms\n", myState.a_sam_state->t_compute_img_ms);
            }
        }
    //Check if opening video file
    } else if (myState.file_dialog_file_selected && myState.file_dialog_mode == FILE_DIALOG_LOAD_VIDEO) {
        printf("OPENING VIDEO FILE\n");
        myState.file_dialog_file_selected = false;
        std::string fullPath = myState.filePath + "/" + myState.filePathName;
        std::vector<cv::Mat> frames;
        //Video aVideo;
        //const ImGuiViewport* viewport = ImGui::GetMainViewport();
        //ImVec2 win_size =ImGui::GetWindowSize();
        read_video(fullPath, myState.aVideo, myState.img_frame_w, myState.img_frame_h);
        printf("Video read.\n");
        //opencv_image2sam(myState.img, myState.aVideo.frames[0].img);
        myState.img = myState.aVideo.frames[0].img_sam_format;
        //myState.tex = createGLTexture(myState.img, GL_RGB);
        myState.selected_frame = 0;
        myState.img_loaded = true;
        //Create a single default mask
        //Mask aMask;
        //myState.aVideo.frames[0].masks.push_back(aMask);
        myState.selected_mask = 0;
        /*
        //Add a vector of textures for this first mask
        std::vector<GLuint> maskTextures;
        myState.masksMaskTextures.push_back(maskTextures);
        std::vector<GLuint> simplifiedMaskTextures;
        myState.masksSimplifiedMaskTextures.push_back(simplifiedMaskTextures);
        */
        //Assign a default color to this first mask
        //myState.aVideo.frames[0].masks[0].color[0] = 0.4f;
        //myState.aVideo.frames[0].masks[0].color[1] = 0.7f;
        //myState.aVideo.frames[0].masks[0].color[2] = 0.0f;
        //myState.aVideo.frames[0].masks[0].color[3] = 0.0f;//no transparency
        printf("After reading video precompute first frame...\n");
        if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
            fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
            //return 1;
        }
        myState.frame_precomputed = 0;
    } else if (myState.file_dialog_file_selected && myState.file_dialog_mode == FILE_DIALOG_SAVE_VIDEO) {
        printf("Saving video...UNDER CONSTRUCTION\n");
        myState.file_dialog_file_selected = false;
        //std::string fullPath = myState.filePath + "/" + myState.filePathName;
        //std::vector<cv::Mat> frames;
        //Video aVideo;
        //const ImGuiViewport* viewport = ImGui::GetMainViewport();
        //ImVec2 win_size =ImGui::GetWindowSize();
        save_video(myState.filePathName, myState.aVideo);
        printf("Video saved.\n");
    }
}

/*
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
        
        
        ImGui::End();
    }
}*/

//checks user actions (e.g. open a file, etc.)
void checkActions(MyState &myState) 
{
    if (myState.img_loaded && myState.propagate) {
        printf("PROPAGATING...\n");
        propagate_masks(myState.aVideo.frames, *myState.a_sam_state, myState.params.n_threads);
        compute_mask_textures_all_frames(myState.aVideo.frames);
        myState.propagate = false;
        printf("PROPAGATED.\n");
    }
}

//Main editor method (called within the main loop)
void editor(bool *show_myWindow, bool *show_file_dialog, MyState &myState) //WARNING: this is executed within the main loop
{
    //auto& io = ImGui::GetIO();
    //if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
    //    return;
    //}

    //Check user actions
    checkActions(myState);

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
    //printf("frameWindow...\n");
    frameWindow(myState, show_myWindow, viewport, use_work_area, flags);
    
    //FILE dialog
    //printf("fileDialog...\n");
    fileDialog(myState, show_file_dialog);

    //MASKS window
    //masksWindow(myState, viewport, flags, use_work_area);

    //FRAMES LIST window
    //printf("framesListWindow...\n");
    framesListWindow(myState, viewport, flags, use_work_area);
    
    //MASKS LIST window
    //printf("masksListWindow...\n");
    masksListWindow(myState, viewport, flags, use_work_area);
    
    //FINISHING window
    //printf("finishingWindow...\n");
    finishingWindow(myState, show_myWindow, viewport, flags, use_work_area);

    //PROPAGATE
    //printf("propagate...\n");
    //propagate(myState);
    
    //ImGui::End();

    /* 
        If the user clicks on the image then the masksWindow(...) methods
        computes the masks and add them to the myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask].masks
        myState.selected_mask=0 by default
        myState.selected_frame=-1 by default but it's set to zero when the video is loaded
    */
}
