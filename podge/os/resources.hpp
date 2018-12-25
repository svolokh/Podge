#if !defined(OS_RESOURCES_HPP_)
#define OS_RESOURCES_HPP_

#include <string>

namespace podge {

std::string get_resource(const std::string &path);
bool resource_exists(const std::string &path);

}

#endif