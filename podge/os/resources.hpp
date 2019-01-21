#if !defined(OS_RESOURCES_HPP_)
#define OS_RESOURCES_HPP_

#include <string>
#include <vector>

namespace podge {

std::string get_resource(const std::string &path);
std::vector<std::string> list_resources(const std::string &path);
bool resource_exists(const std::string &path);

}

#endif
