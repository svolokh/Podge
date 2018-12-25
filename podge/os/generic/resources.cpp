#include "../resources.hpp"

#include <fstream>

namespace podge {

std::string get_resource(const std::string &path) {
	std::ifstream ifs(path, std::ios::binary);
	return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}

bool resource_exists(const std::string &path) {
	std::ifstream ifs(path, std::ios::binary);
	return !ifs.fail();
}

}
