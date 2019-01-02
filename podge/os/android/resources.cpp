#define BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS

#include "../../common.hpp"
#include "../resources.hpp"

#include <SDL.h>
#include <SDL_system.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <boost/scope_exit.hpp>
#include <jni.h>

namespace podge {

struct jni_local_scope {
	jni_local_scope(int capacity) {
		auto env(static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv()));
		if(env->PushLocalFrame(capacity) < 0) {
			PODGE_THROW_ERROR();
		}
	}

	~jni_local_scope() {
		auto env(static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv()));
		env->PopLocalFrame(nullptr);
	}
};

// uses the existing scope
static AAssetManager *get_asset_manager() {
	auto env(static_cast<JNIEnv *>(SDL_AndroidGetJNIEnv()));
	auto activity(static_cast<jobject>(SDL_AndroidGetActivity()));
	auto activity_cls(env->GetObjectClass(activity));
	auto activity_getResources_mid(env->GetMethodID(activity_cls, "getResources", "()Landroid/content/res/Resources;"));
	auto resources(env->CallObjectMethod(activity, activity_getResources_mid));
	auto resources_cls(env->GetObjectClass(resources));
	auto resources_getAssets_mid(env->GetMethodID(resources_cls, "getAssets", "()Landroid/content/res/AssetManager;"));
	auto assetmgr_(env->CallObjectMethod(resources, resources_getAssets_mid));
	return AAssetManager_fromJava(env, assetmgr_);
}

std::string get_resource(const std::string &path) {
	SDL_Log("Loading resource %s", path.c_str());
	jni_local_scope scope(16);
	auto assetmgr(get_asset_manager());
	auto asset(AAssetManager_open(assetmgr, path.c_str(), AASSET_MODE_BUFFER));
	BOOST_SCOPE_EXIT(asset) {
		AAsset_close(asset);
	} BOOST_SCOPE_EXIT_END
	auto buf(AAsset_getBuffer(asset));
	return std::string(static_cast<const char *>(buf), static_cast<const char *>(buf) + AAsset_getLength(asset));
}

bool resource_exists(const std::string &path) {
	jni_local_scope scope(16);
	auto assetmgr(get_asset_manager());
	auto asset(AAssetManager_open(assetmgr, path.c_str(), AASSET_MODE_STREAMING));
	if(asset != nullptr) {
		AAsset_close(asset);
		return true;
	} else {
		return false;
	}
}

}
