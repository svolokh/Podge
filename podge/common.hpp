#include <sstream>
#include <stdexcept>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define THROW_ERROR() { std::ostringstream oss; oss << __FILE__ ":" STRINGIFY(__LINE__) << ": Error"; throw std::runtime_error(oss.str()); }
#define THROW_SDL_ERROR() { std::ostringstream oss; oss << __FILE__ ":" STRINGIFY(__LINE__) << ": " << SDL_GetError() << std::endl; throw std::runtime_error(oss.str()); }
#define THROW_IMG_ERROR() { std::ostringstream oss; oss << __FILE__ ":" STRINGIFY(__LINE__) << ": " << IMG_GetError() << std::endl; throw std::runtime_error(oss.str()); }

#define PODGE_FUNCTION() EASY_FUNCTION()
