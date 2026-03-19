#include "pch.h"
#include "TextureMap.h"


bool bad::TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, SDL_Surface* const surface)
{
	Texture texture = make_texture(renderer, surface);
	return insert(std::move(texture), tag);
}

bool bad::TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, const std::string& file_path)
{
	Texture texture = make_texture(renderer, file_path);
	return insert(std::move(texture), tag);
}

bool bad::TextureMap::insert(Texture&& texture, const std::string& tag) {
	if (texture) {
		auto find = mTextures.find(tag);
		if (find == mTextures.end()) {
			mTextures.emplace(tag, std::move(texture));
			return true;
		}
	}
	return false;
}

void bad::TextureMap::remove(const std::string& tag)noexcept
{
	mTextures.erase(tag);
}

SDL_Texture* bad::TextureMap::get_texture(const std::string& tag)const noexcept
{
	auto it = mTextures.find(tag);
	return it == mTextures.end() ? nullptr : it->second.get();
}

bool bad::TextureMap::has(const std::string& tag)const noexcept
{
	return mTextures.find(tag) != mTextures.end();
}

std::vector<std::string> bad::TextureMap::get_tags()const
{
	std::vector<std::string> tags;
	tags.reserve(mTextures.size());
	for (const auto& pair : mTextures) {
		tags.push_back(pair.first);
	}
	return tags;
}