#define BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS
#include "../resources.hpp"

#include <cstring>
#include <fstream>
#include <dirent.h>

namespace podge {

std::string get_resource(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}

std::vector<std::string> list_resources(const std::string &path) {
    std::vector<std::string> res;
    auto dp(opendir(path.c_str()));
    struct dirent *ent;
    while((ent = readdir(dp)) != nullptr) {
        if(std::strcmp(ent->d_name, ".") == 0 || std::strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        res.emplace_back(ent->d_name);
    }
    return res;
}

bool resource_exists(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    return !ifs.fail();
}

}
