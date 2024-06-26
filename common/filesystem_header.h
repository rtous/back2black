/*
   Sets the include and namespace for the filesystem library (fs) which requires
   different code for Windows and MacOS.
*/


#ifndef __FILESYSTEM_HEADER__
#define __FILESYSTEM_HEADER__

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    warning "fs = std::filesystem;"
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    warning "fs = std::experimental::filesystem;"
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    warning "fs = boost::filesystem;"
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  elif __APPLE__
#    warning "APPLE detected"
#    include <filesystem>
     namespace fs = std::__fs::filesystem;
#  endif
#endif

#endif