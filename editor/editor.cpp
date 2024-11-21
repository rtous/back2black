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
#include <thread>
#include "IconsFontAwesome5.h" //see https://github.com/juliettef/IconFontCppHeaders
#include "face.h"


//#include <iostream>
//#include <unistd.h> //sleep funcion (debug)

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


//top menu bar
//params: 
//  - show_file_dialog to signal the need to call the fileDialog function
static void mainMenuBar(MyState &myState)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            //setting show_file_dialog will trigger fileDialog function (below)
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Open Image", "Ctrl+O")) {
                //*show_file_dialog = true;
                myState.show_file_dialog = true;
                myState.file_dialog_mode = FILE_DIALOG_LOAD_SINGLE_FILE;
                myState.clicked = false;//to avoid the click going through
            }
            if (ImGui::MenuItem("Open Video", "Ctrl+V")) {
                //*show_file_dialog = true;
                //myState.show_file_dialog_video = true;
                myState.show_file_dialog = true;
                myState.file_dialog_mode = FILE_DIALOG_LOAD_VIDEO;
                myState.clicked = false;//to avoid the click going through
            }
            if (ImGui::MenuItem("Save Video", "Ctrl+S")) {
                //*show_file_dialog = true;
                //myState.show_file_dialog_save_video = true;
                myState.show_file_dialog = true;
                myState.file_dialog_mode = FILE_DIALOG_SAVE_VIDEO;
                myState.clicked = false;//to avoid the click going through
            }
            if (ImGui::MenuItem("Exit", "Ctrl+F4"))
               myState.done = true;//this is detected in the main loop in main.cpp
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Propagate masks to next frames", "Ctrl+P")) {
                //myState.propagate = true;
                myState.propagate_dialog = true;
                myState.clicked = false;//to avoid the click go into the frame
                
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("About"))
        {
            //ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("Version information")) {
                myState.about_version_popup = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void drawAllMasks(MyState &myState, const ImGuiViewport* viewport, ImVec2 newPos, bool simplified) {

    //compute_mask_textures(sam_image_u8 img, const sam_params & params, sam_state & state, std::vector<GLuint> *maskTextures, int x, int y, std::vector<sam_image_u8> & storedMasks, std::vector<int> * mask_colors, int & last_color_id) {

    //DRAW MASKS
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    //printf("Found %d masks.\n", myState.aVideo.frames[myState.selected_frame].masks.size());
    //for (int j = 0; j < int(myState.aVideo.frames[myState.selected_frame].masks.size()); ++j) {
    //draw in reverse order (mask 0 is on top)
    for (int j =  int(myState.aVideo.frames[myState.selected_frame].masks.size())-1; j >= 0; --j) {

        Mask aMask = myState.aVideo.frames[myState.selected_frame].masks[j];

        if (aMask.visible) {
            const int r = aMask.color[0]*255;
            const int g = aMask.color[1]*255;
            const int b = aMask.color[2]*255;
            
            if (simplified)
                //ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x, mousePositionAbsolute.y - screenPositionAbsolute.y);
                draw_list->AddImage((void*)(intptr_t)aMask.simplifiedMaskTexture, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));
            else 
                draw_list->AddImage((void*)(intptr_t)aMask.maskTexture, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1), IM_COL32(r, g, b, 255));  
        }
    }
    if (simplified)
        if (myState.aVideo.frames[myState.selected_frame].faces_computed)
            draw_list->AddImage((void*)(intptr_t)myState.aVideo.frames[myState.selected_frame].facesTexture, ImVec2(newPos[0], newPos[1]), ImVec2(newPos[0]+myState.img.nx, newPos[1]+myState.img.ny), ImVec2(0,0), ImVec2(1,1));       
}



static void frameWindow(MyState &myState, const ImGuiViewport* viewport, bool use_work_area, ImGuiWindowFlags flags) 
{
    printf("frameWindow...\n");
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
    //ImGui::Begin("FRAME", show_myWindow, ImGuiWindowFlags_NoTitleBar);
    //ImGui::Begin("FRAME", show_myWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    ImGui::Begin("FRAME", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    //myState.img_frame_pos_x = ;
    //myState.img_frame_pos_y;

    /******* image ******/
    if (myState.img_loaded) {
        printf("myState.img_loaded...\n");
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

        printf("myState.img_loaded so adding aFrame.tex to drawlist\n");
        draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(viewport->WorkPos.x, viewport->WorkPos.y), ImVec2(viewport->WorkPos.x+myState.img.nx, viewport->WorkPos.y+myState.img.ny));
        //draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(newPos.x, newPos.y), ImVec2(myState.img.nx, myState.img.ny));
        //draw_list->AddImage((void*)(intptr_t)aFrame.tex, ImVec2(myState.img_frame_pos_x,myState.img_frame_pos_y), ImVec2(myState.img.nx, myState.img.ny));
        //draw_list->AddCircleFilled(ImVec2(100, 100), 5, IM_COL32(255, 0, 0, 255));
    } else {
        ImGui::Text("Load a video or an image from the File menu.");
    }
    /********************/

    if (myState.img_loaded) {  
        //capture click
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
        //draw all masks
        drawAllMasks(myState, viewport, viewport->WorkPos, false);
    }
    ImGui::End();
}

static void framesListWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.20f, viewport->WorkSize.y * 0.25f);
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.08f, viewport->WorkSize.y * 1.0f);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.08f, viewport->WorkSize.y * 0.25f);
    
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + 0, viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + 0, viewport->WorkPos.y + 0);
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("FRAMES", NULL, ImGuiWindowFlags_NoCollapse);
    //ImGui::Text("FRAMES WINDOW"); 

    //const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
    //const char* items[100];
    std::vector<std::string> items; 

    if (myState.aVideo.loaded) {
        for (Frame & aFrame : myState.aVideo.frames) {
            items.push_back("Frame "+std::to_string(aFrame.order));
            //items[aFrame.order] = std::to_string(aFrame.order);
        }
    }

    static int item_current_idx = 0; // Here we store our selection data as an index.
    ImGui::PushItemWidth(-1); //to occupy all width if no label
    //if (ImGui::BeginListBox(""))
    //if (ImGui::BeginListBox("##listbox frames", ImVec2(-FLT_MIN, size.y-100)))
    if (ImGui::BeginListBox("##listbox frames", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        int n = 0;
        for (std::string & item : items)
        {
            //ImGui::PushItemWidth(-1);
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
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.42f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.33f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.08f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.08f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f);
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("MASKS", NULL, ImGuiWindowFlags_NoCollapse);
    //ImGui::Text("MASKS WINDOW"); 
    
    std::vector<std::string> items; 
    std::vector<int> clickedDelete;
    std::vector<int> clickedUp; 
    std::vector<int> clickedDown;
    std::vector<int> clickedVisible; 

    //items.push_back("MASK 1");

    //First we put strings into "items" vector
    if (myState.aVideo.loaded) {
        int i = 0;
        for (Mask & aMask : myState.aVideo.frames[myState.selected_frame].masks) {
            items.push_back("Mask "+std::to_string(aMask.maskId));
            //items.push_back("Mask "+std::to_string(i));
            i++;
        }
    }

    //ImDrawList* draw_list = ImGui::GetWindowDrawList();
    //draw_list->ChannelsSplit(2);
    //draw_list->ChannelsSetCurrent(0);

    //More complex selectable with colors: https://github.com/ocornut/imgui/issues/4719
    ImGui::PushItemWidth(-1); //to occupy all width if no label
    //if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
    if (ImGui::BeginListBox("##listbox masks", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
    {
        //for the eye icon buttons
        ImGuiIO& io = ImGui::GetIO();
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;

        //ImGui::Columns(2);
        //ImGui::BeginTable("table1", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;);
        ImGui::BeginTable("table1", 2);
        //ImGui::AlignTextToFramePadding();
        ImGui::TableSetupColumn("AAA", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("BBB", ImGuiTableColumnFlags_WidthStretch);           
        int i = 0;
        for (std::string & item : items)
        {
            ImGui::TableNextColumn();
            //COLUMN 1: SELECTABLES
            const bool is_selected = (myState.selected_mask == i);
            //This shows a selectable string 
            //and sets the boolean is_selected to capture input
            //and returns true if the item is selected
            //if (ImGui::Selectable(item.c_str(), is_selected, ImGuiSelectableFlags_SpanAllColumns)) {
            ImGui::AlignTextToFramePadding();
            if (ImGui::Selectable(item.c_str(), is_selected)) {
                myState.selected_mask = i;
            }
            
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
            //In case you don't want a selectable replace above code by this line:
            //ImGui::Text(item.c_str());

            //COLUMN 2: BUTTONS



            //BUTTON 1 (color picker)
            //ImGui::NextColumn();
            ImGui::TableNextColumn();
            std::string color_picker_id = "color " + std::to_string(i);
            ImGui::ColorEdit4(color_picker_id.c_str(), myState.aVideo.frames[myState.selected_frame].masks[i].color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            //ImGui::SameLine();
            
            //"imgui selectable with buttons"
            //https://github.com/ocornut/imgui/issues/6574
            //ImGui::SetNextItemAllowOverlap();


            ImGui::SameLine();
            //ImGui::SameLine(0, 50);
            //draw_list->ChannelsSetCurrent(1);

            //BUTTON 2 (eye)
            clickedVisible.push_back(0);
            ImGui::SameLine();

            std::string buttonID = "visibleButton" + std::to_string(i);
            /*ImVec2 size = ImVec2(12.0f, 12.0f);                         // Size of the image we want to make visible
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);                            // UV coordinates for lower-left
            ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);    // UV coordinates for (32,32) in our texture
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);             // Black background
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);           // No tint
            */
            //if (ImGui::ImageButton(buttonID.c_str(), my_tex_id, size, uv0, uv1, bg_col, tint_col))
            char buf2[64];
            if (myState.aVideo.frames[myState.selected_frame].masks[i].visible)
                sprintf(buf2, ICON_FA_EYE"###%s", buttonID.c_str());
            else
                sprintf(buf2, ICON_FA_EYE_SLASH"###%s", buttonID.c_str());
            if (ImGui::Button(buf2))
                clickedVisible[i]++;
            if (clickedVisible[i] & 1) {
                myState.aVideo.frames[myState.selected_frame].masks[i].visible=!myState.aVideo.frames[myState.selected_frame].masks[i].visible;
                clickedVisible[i]++;
            }

            //BUTTON 3 (delete)
            ImGui::SameLine();

            //static int clicked2 = 0;
            clickedDelete.push_back(0);
            //The button has a label and a ID
            //By default both are the string passed in the first param
            //But we can differentiate them with the ### operator
            std::string deleteButtonID = "deleteButton" + std::to_string(i);
            char buf[64];
            sprintf(buf, "-###%s", deleteButtonID.c_str());
            if (ImGui::Button(buf))
                clickedDelete[i]++;
                //clicked2++;
            //if (clicked2 & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 0)
            if (clickedDelete[i] & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 0)
            {
                printf("Deleting\n");
                ImGui::SameLine();
                ImGui::Text("Erasing mask!");
                myState.aVideo.frames[myState.selected_frame].masks.erase(myState.aVideo.frames[myState.selected_frame].masks.begin() + i);//myState.selected_mask);
                /*if (myState.aVideo.frames[myState.selected_frame].masks.size() > 0)
                    myState.selected_mask = myState.selected_mask-1;  
                else  
                    myState.selected_mask = 0;*/
                //clicked2 = 0;
                clickedDelete[i] = 0;
            } else if (clickedDelete[i] > 0) {
                printf("Not deleting, clickedDelete[i]=%d and myState.aVideo.frames[myState.selected_frame].masks.size()=%d\n",clickedDelete[i], myState.aVideo.frames[myState.selected_frame].masks.size());
            }   

            //BUTTON 4
            clickedUp.push_back(0);
            if (i > 0) { //only allow if not the top selected
                ImGui::SameLine();
                //static int clicked3 = 0;
                //if (ImGui::Button("U"))
                std::string buttonID = "upButton" + std::to_string(i);
                if (ImGui::ArrowButton(buttonID.c_str(), ImGuiDir_Up))
                    clickedUp[i]++;
                if (clickedUp[i] & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 1)
                {
                    //ImGui::SameLine();
                    //ImGui::Text("Moving mask!");

                    //swap masks
                    Mask aboveMask = myState.aVideo.frames[myState.selected_frame].masks[i-1];
                    myState.aVideo.frames[myState.selected_frame].masks[i-1] = myState.aVideo.frames[myState.selected_frame].masks[i];
                    myState.aVideo.frames[myState.selected_frame].masks[i] = aboveMask;
                    if (myState.selected_mask == i)
                        myState.selected_mask = myState.selected_mask-1;    
                    clickedUp[i] = 0;
                }
            }

            //BUTTON 5
            clickedDown.push_back(0);
            if (i < myState.aVideo.frames[myState.selected_frame].masks.size()-1) { //only allow if not the top selected
                ImGui::SameLine();
                std::string buttonID = "downButton" + std::to_string(i);
                if (ImGui::ArrowButton(buttonID.c_str(), ImGuiDir_Down))
                    clickedDown[i]++;
                if (clickedDown[i] & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 1)
                {
                    //ImGui::SameLine();
                    //ImGui::Text("Moving mask!");

                    //swap masks
                    Mask belowMask = myState.aVideo.frames[myState.selected_frame].masks[i+1];
                    myState.aVideo.frames[myState.selected_frame].masks[i+1] = myState.aVideo.frames[myState.selected_frame].masks[i];
                    myState.aVideo.frames[myState.selected_frame].masks[i] = belowMask;
                    if (myState.selected_mask == i)
                        myState.selected_mask = myState.selected_mask+1;    
                    clickedDown[i] = 0;
                }
            }


            

            i++;
        }
        ImGui::EndTable();
        ImGui::EndListBox();
    }
    //BUTTONS THAT AFFECT ALL MASKS (OR THE SELECTED MASK)
    /*if (myState.aVideo.loaded) {
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
        

        //BUTTON 2
        ImGui::SameLine();
        static int clicked2 = 0;
        if (ImGui::Button("-"))
            clicked2++;
        if (clicked2 & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 0)
        {
            ImGui::SameLine();
            ImGui::Text("Erasing mask!");
            myState.aVideo.frames[myState.selected_frame].masks.erase(myState.aVideo.frames[myState.selected_frame].masks.begin() + myState.selected_mask);
            if (myState.aVideo.frames[myState.selected_frame].masks.size() > 0)
                myState.selected_mask = myState.selected_mask-1;  
            else  
                myState.selected_mask = 0;    
            clicked2 = 0;
        }

        //BUTTON 3
        if (myState.selected_mask > 0) { //only allow if not the top selected
            ImGui::SameLine();
            static int clicked3 = 0;
            //if (ImGui::Button("U"))
            if (ImGui::ArrowButton("MoveUp", ImGuiDir_Up))
                clicked3++;
            if (clicked3 & 1 && myState.aVideo.frames[myState.selected_frame].masks.size() > 1)
            {
                ImGui::SameLine();
                ImGui::Text("Moving mask!");

                //swap masks
                Mask aboveMask = myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask-1];
                myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask-1] = myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask];
                myState.aVideo.frames[myState.selected_frame].masks[myState.selected_mask] = aboveMask;
                myState.selected_mask = myState.selected_mask-1;    
                clicked3 = 0;
            }
        }
    }*/

    

    ImGui::End();
}

static void finishingWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.33f, viewport->WorkSize.y * 0.25f);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.75f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.66f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.5f), viewport->WorkPos.y + 0);
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    //ImGui::Begin("FINISHING PANEL", show_myWindow, ImGuiWindowFlags_NoTitleBar);
    bool show_myWindow = true;
    ImGui::Begin("FINISHING PANEL", &show_myWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
    if (myState.img_loaded) {  
        //Origin considering the bars 
        ImVec2 newPos = ImVec2(viewport->Pos.x + (viewport->WorkSize.x * 0.5f), viewport->Pos.y + 0);
        drawAllMasks(myState, viewport, ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.5f), viewport->WorkPos.y + 0), true);
    } else {
        ImGui::Text("No mask has yet been selected in the frame panel."); 
    }

    ImGui::End();
}

static void finishingConfigWindow(MyState &myState, const ImGuiViewport* viewport, ImGuiWindowFlags flags, bool use_work_area) 
{
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.50f, viewport->WorkSize.y * 0.25f);
    flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.33f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.50f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f));
    //ImVec2 newPos = ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x * 0.08f), viewport->WorkPos.y + (viewport->WorkSize.y * 0.75f);
    ImGui::SetNextWindowPos(newPos);
    ImGui::SetNextWindowSize(use_work_area ? size : viewport->Size);
    ImGui::Begin("FINISHING DETAILS", NULL, ImGuiWindowFlags_NoCollapse); 
    
    

    if (myState.img_loaded) {
        ImGui::Checkbox("Facial traits", &myState.aVideo.frames[myState.selected_frame].faces_check);
        if (myState.aVideo.frames[myState.selected_frame].faces_check && !myState.aVideo.frames[myState.selected_frame].faces_computed) {
            printf("Computing face...\n");
            //myState.aVideo.frames[myState.selected_frame].faces = //myState.aVideo.frames[myState.selected_frame].img.clone();    
            cv::Mat blankImageWithAlpha = cv::Mat(cv::Size(myState.aVideo.frames[myState.selected_frame].img.cols,myState.aVideo.frames[myState.selected_frame].img.rows), CV_8UC4, cv::Scalar(0,0,0,0));
            face(myState.aVideo.frames[myState.selected_frame].img, blankImageWithAlpha, cv::Scalar(118, 113, 168, 255), cv::Scalar(61, 71, 118, 255));
            myState.aVideo.frames[myState.selected_frame].faces = blankImageWithAlpha;
            //DEBUG
            cv::imwrite("borrar_faces.png", myState.aVideo.frames[myState.selected_frame].faces);
            sam_image_u8 sam_img_face;
            //opencv_image2sam(sam_img_face, myState.aVideo.frames[myState.selected_frame].faces);
            //sam_image2opencv_color(sam_img_face, myState.aVideo.frames[myState.selected_frame].faces);
            //sam_image_u8 sam_img_face_rgb = sam_image2color(sam_img_face);
            //GLuint newGLTextureFace = createGLTexture(sam_img_face, GL_RGBA);
            GLuint newGLTextureFace = createGLTextureOpenCV(blankImageWithAlpha, GL_RGBA);
            myState.aVideo.frames[myState.selected_frame].facesTexture = newGLTextureFace;
            myState.aVideo.frames[myState.selected_frame].faces_computed = true;
        }
    }

    ImGui::End();
}


//Checks fileDialog related actions:
//  - show file dialog
//  - process a file dialog action
void fileDialog(MyState &myState) {
    //Show file dialog
    if (myState.show_file_dialog) {
        //TODO: this function always does the same thing and sets myState.filePath and myState.openFile = true;
        //Maybe should be customized for each type of action
        show_file_dialog_f(myState);
    }

    //Check if opening single file
    if (myState.file_dialog_file_selected && myState.file_dialog_mode == FILE_DIALOG_LOAD_SINGLE_FILE) {
        //We will deal with a single file as if it is a video with one frame
        //myState.img has a copy of myState.aVideo.frames[myState.selected_frame].img_sam_format
        printf("OPENING IMAGE FILE\n");

        myState.file_dialog_file_selected = false;
        std::string image_path = myState.filePath + "/" + myState.filePathName;
        cv::Mat opencv_img = cv::imread(image_path, cv::IMREAD_COLOR);
        
        if(!opencv_img.empty()) {
            Frame aFrame;
            aFrame.img = opencv_img;
            opencv_image2sam(aFrame.img_sam_format, opencv_img);
            aFrame.order = 0;
            downscale_img_to_size(aFrame.img_sam_format, myState.img_frame_w, myState.img_frame_h);
            
            //load_image_samformat_from_file(image_path, aFrame.img_sam_format);


            aFrame.tex = createGLTexture(aFrame.img_sam_format, GL_RGB);
            myState.aVideo.frames.push_back(aFrame);
            myState.img_loaded = true;
            myState.selected_frame = 0;
            myState.selected_mask = 0;

            myState.img = myState.aVideo.frames[0].img_sam_format;
            printf("After reading video precompute first frame...\n");
            if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
                fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
                //return 1;
            }
            myState.frame_precomputed = 0;
            myState.aVideo.loaded = true;
        } else {
            printf("Error opening path: %s\n", image_path.c_str());
        }

        /*myState.file_dialog_file_selected = false;
        std::string fileName = myState.filePath + "/" + myState.filePathName;
        //from common lib
        //NOTE will be rendered in frameWindow()
        if (!load_image_samformat_from_file(fileName, myState.img)) {
            printf("failed to load image from '%s'\n", fileName.c_str());  
        } else {
            printf("successfully loaded image from '%s'\n", fileName.c_str());
            Frame aFrame;
            aFrame.img = frame;
            opencv_image2sam(aFrame.img_sam_format, frame);
            aFrame.order = 0;
            downscale_img_to_size(aFrame.img_sam_format, w, h);
            aFrame.tex = createGLTexture(aFrame.img_sam_format, GL_RGB);


            myState.aVideo.frames.push_back(aFrame);
            myState.img_loaded = true;
            myState.selected_frame = 0;
            myState.selected_mask = 0;
            myState.frame_precomputed = 0; 
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
        */
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

//asks information for the propagation
void propagateDialog(MyState &myState) {
    //printf("propagateDialog...\n");
    ImGui::OpenPopup("Propagate masks to next frames");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Propagate masks to next frames", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Masks in the reference frame will be propagated to next frames till the specified frame (included).\nThis operation is slow.");
        ImGui::Separator();
        int num_frames = myState.aVideo.frames.size();
        if (myState.start_frame == -1 && myState.selected_frame < num_frames-1) {
            //If first time.
            myState.start_frame = myState.selected_frame;
            myState.end_frame = myState.selected_frame+1;
        } else if (myState.selected_frame == num_frames-1) {
            //If first time and the last frame is selected then 
            myState.start_frame = 0;
            myState.end_frame = 1;
        }
        
        ImGui::SliderInt("Reference frame", &myState.start_frame, 0, num_frames-2);
        if (myState.start_frame >= myState.end_frame) {
            myState.end_frame = myState.start_frame+1;
        } 
        ImGui::SliderInt("Till frame", &myState.end_frame, myState.start_frame+1, num_frames-1);

        /* --------- */

        if (ImGui::Button("OK", ImVec2(120, 0))) { 
            myState.propagate = true;
            myState.propagate_dialog = false;
            myState.propagate_cancel = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
            myState.start_frame = -1;
            myState.end_frame = -1;
            myState.propagate_dialog = false;
            ImGui::CloseCurrentPopup(); 
        }

        
        ImGui::EndPopup();
    } else {
        printf("NO BeginPopupModal\n");
    }
}

//shows the progress bar during propagation
void propagatingDialog(MyState &myState) {
    ImGui::OpenPopup("Propagating masks to next frames...");
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Propagating masks to next frames...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::Button("STOP", ImVec2(120, 0))) { 
            myState.propagate_cancel = true;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();

        //PROGRESS BAR
        if (myState.propagating) {
            ImGui::ProgressBar(myState.progress, ImVec2(0.0f, 0.0f));
        }
        
        ImGui::EndPopup();
    } else {
        printf("NO BeginPopupModal\n");
    }
}

//Async task launched as an independent thread from checkActions
//It allows to display a progress bar during propagation and to cancel it before the end
void asynch_task(MyState &myState)
{    
    propagate_masks(myState.aVideo.frames, *myState.a_sam_state, myState.params.n_threads, myState.start_frame, myState.end_frame, myState.progress, myState.propagate_cancel);
    myState.propagated = true; 
}

void about_version_popup(MyState &myState) {
    ImGui::OpenPopup("Version information.");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    //The name in the BeginPopupModal need to be the same as the name in OpenPopup
    if (ImGui::BeginPopupModal("Version information.", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Lester version 1.0.");
        ImGui::Separator();
        ImGui::Text("Designed and programmed by Ruben Tous.");
        ImGui::Separator();
        ImGui::Text("First release: January 2025.");
        ImGui::Separator();
        ImGui::Text("Current release: January 2025.");
        if (ImGui::Button("OK", ImVec2(120, 0))) { 
            myState.about_version_popup = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    } else {
        printf("NO BeginPopupModal\n");
    }
}

//checks user actions (e.g. open a file, etc.)
void checkActions(MyState &myState) 
{
    if (myState.propagate_dialog) {
        propagateDialog(myState);
    }

    if (myState.img_loaded && myState.propagate) {
        printf("PROPAGATING...\n");
		
		//testing
        myState.propagate = false;
        myState.propagating = true;
        
        bool using_threads = true; //CURRENTLY DISABLED
        if (using_threads) { 
            std::thread thread_1(asynch_task, std::ref(myState));
		    thread_1.detach(); //to avoid crashing when the thread ends
        } else {
            propagate_masks(myState.aVideo.frames, *myState.a_sam_state, myState.params.n_threads, myState.start_frame, myState.end_frame, myState.progress, myState.propagate_cancel);
            myState.propagated = true;
        }
    }
    //once the propagation finishes
    if (myState.propagated) {
        printf("THREAD END CAPTURED.\n");
        myState.propagated = false;
        myState.propagating = false;
        //review textures for all frame (only the propated will take effect)
        compute_mask_textures_all_frames(myState.aVideo.frames);
        //review facial textures for all frames (only the propagated will take effect)
        //TODO: never recomputed, maybe change this if allow color changes
        compute_facial_textures_all_frames(myState.aVideo.frames);
        myState.start_frame = -1;
        myState.end_frame = -1;
        printf("PROPAGATED.\n");
        //Precompute actual frame to keep things as they were
        if (!sam_compute_embd_img(myState.img, myState.params.n_threads, *myState.a_sam_state)) {
            fprintf(stderr, "%s: failed to compute encoded image\n", __func__);
            //return 1;
        }
        myState.frame_precomputed = myState.selected_frame; 
        myState.propagate_dialog = false;      
    }
    if (myState.propagating) {
        propagatingDialog(myState);
    }
    if (myState.about_version_popup) {
        about_version_popup(myState);
    }
}

//Main editor method (called within the main loop)
void editor(MyState &myState) //WARNING: this is executed within the main loop
{
    //Check user actions
    checkActions(myState);

    //Main Menu
    mainMenuBar(myState);

    static bool use_work_area = true;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
    //ImVec2 size = ImVec2(viewport->WorkSize.x * 0.5f, viewport->WorkSize.y * 0.5f);
    

    //printf("viewport->WorkSize.x=%f\n", viewport->WorkSize.x);
    //printf("viewport->WorkSize.y=%f\n", viewport->WorkSize.y);
    ImVec2 size = ImVec2(viewport->WorkSize.x * 0.6f, viewport->WorkSize.y * 0.6f);
    
    //IMAGE window
    frameWindow(myState, viewport, use_work_area, flags);
    
    //FILE dialog 
    fileDialog(myState);

    //FRAMES LIST window
    framesListWindow(myState, viewport, flags, use_work_area);
    
    //MASKS LIST window
    masksListWindow(myState, viewport, flags, use_work_area);
    
    //FINISHING window
    finishingWindow(myState, viewport, flags, use_work_area);

    //FINISHING window
    finishingConfigWindow(myState, viewport, flags, use_work_area);
}
