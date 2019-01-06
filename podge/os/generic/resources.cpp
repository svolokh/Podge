#include "../resources.hpp"

#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

namespace podge {

std::string get_resource(const std::string &path) {
	std::ifstream ifs(path, std::ios::binary);
	return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
}

std::vector<std::string> list_resources(const std::string &path) {
	std::vector<std::string> res;
	fs::path p(path);
	for(const auto &p : fs::directory_iterator(fs::path(path))) {
		res.emplace_back(p.filename().string());
	}
	return res;
}

bool resource_exists(const std::string &path) {
	fs::path p(path);
	return fs::exists(p);
}

}
