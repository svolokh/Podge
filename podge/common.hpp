#include <sstream>
#include <stdexcept>

#define PODGE_STRINGIFY_(x) #x
#define PODGE_STRINGIFY(x) PODGE_STRINGIFY_(x)
#define PODGE_CONCAT_(LEFT, MIDDLE, RIGHT) LEFT ## MIDDLE ## RIGHT
#define PODGE_CONCAT(LEFT, MIDDLE, RIGHT) PODGE_CONCAT_(LEFT, MIDDLE, RIGHT)
#define PODGE_THROW_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": Error"; throw std::runtime_error(oss.str()); }
#define PODGE_THROW_SDL_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": " << SDL_GetError() << std::endl; throw std::runtime_error(oss.str()); }
#define PODGE_THROW_MIX_ERROR() { std::ostringstream oss; oss << __FILE__ ":" PODGE_STRINGIFY(__LINE__) << ": " << Mix_GetError() << std::endl; throw std::runtime_error(oss.str()); }

// number of channels to allocate for Podge BGM + SFX
#define PODGE_MIX_NUM_CHANNELS 16

// the channel on which to play the BGM
#define PODGE_MIX_MUSIC_CHANNEL 0
