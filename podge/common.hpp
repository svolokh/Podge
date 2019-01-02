#include <sstream>
#include <stdexcept>

#define PODGE_STRINGIFY_(x) #x
#define PODGE_STRINGIFY(x) PODGE_STRINGIFY_(x)
#define PODGE_THROW_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": Error"; throw std::runtime_error(oss.str()); }
#define PODGE_THROW_SDL_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": " << SDL_GetError() << std::endl; throw std::runtime_error(oss.str()); }
#define PODGE_THROW_IMG_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": " << IMG_GetError() << std::endl; throw std::runtime_error(oss.str()); }
