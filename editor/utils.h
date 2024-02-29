static bool get_screen_size(SDL_DisplayMode &dm, SDL_Window* window);
static sam_image_u8 downscale_img(sam_image_u8 &img , float scale);
static bool downscale_img_to_screen(sam_image_u8 &img, SDL_Window* window);
bool load_image_from_file(const std::string & fname, sam_image_u8 & img);
//void test(const std::string & fname, sam_image_u8 & img);