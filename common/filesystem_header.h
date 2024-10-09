/*
   Sets the include and namespace for the filesystem library (fs) which requires
   different code for Windows and MacOS.
*/


#ifndef __FILESYSTEM_HEADER__
#define __FILESYSTEM_HEADER__

#if __cplusplus == 202002L
//#	warning "C++20"
#	 pragma message ("C++20")
#elif __cplusplus == 201703L
//#    warning "C++17"
#	 pragma message ("C++17")
#elif __cplusplus == 201402L
//#    warning "C++14"
#	 pragma message ("C++14")
#elif __cplusplus == 201103L
//#    warning "C++11"
#	 pragma message ("C++11")
#elif __cplusplus == 199711L
//#    warning "C++98"
#	 pragma message ("C++98")
#endif


#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
//#    warning "fs = std::filesystem;"
#	 pragma message ("fs = std::filesystem;")
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
//#    warning "fs = std::experimental::filesystem;"
#	 pragma message ("fs = std::experimental::filesystem;")
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
//#    warning "fs = boost::filesystem;"
#	 pragma message ("fs = boost::filesystem;")
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  elif __APPLE__
//#    warning "APPLE detected"
#	 pragma message ("APPLE detected")
#    include <filesystem>
     namespace fs = std::__fs::filesystem;
#  endif
#endif


/*
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
//#  if __cplusplus >= 201703L && __has_include(<filesystem>)
//#    warning "fs = std::filesystem;"
//#    include <filesystem>
//     namespace fs = std::filesystem;
//#  elif __has_include(<experimental/filesystem>)
#  if __has_include(<experimental/filesystem>)
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
*/

#endif