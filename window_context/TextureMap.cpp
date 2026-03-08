#include "pch.h"
#include "TextureMap.h"

namespace badWindow
{
	bool TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, SDL_Surface* const surface)
	{
		Texture texture = make_texture(renderer, surface);
		return insert(std::move(texture), tag);
	}

	bool TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, const char* file_path)
	{
		Texture texture = make_texture(renderer, file_path);
		return insert(std::move(texture), tag);
	}

	bool TextureMap::insert(Texture&& textr, const std::string& tag) {
		if (textr) {
			auto find = mTextures.find(tag);
			if (find == mTextures.end()) {
				mTextures.emplace(tag, std::move(textr));
				return true;
			}
		}
		return false;
	}
	void TextureMap::clear()
	{
		mTextures.clear();
	}

	void TextureMap::remove(const char* tag)noexcept
	{
		mTextures.erase(tag);
	}

	SDL_Texture* TextureMap::get_texture(const char* tag)const noexcept
	{
		auto it = mTextures.find(tag);
		return it == mTextures.end() ? nullptr : it->second.get();
	}

	bool TextureMap::has(const char* tag)const noexcept
	{
		return mTextures.find(tag) != mTextures.end();
	}

	std::vector<std::string> TextureMap::get_tags()const 
	{
		std::vector<std::string> tags;
		tags.reserve(mTextures.size());
		for (const auto& pair : mTextures) {
			tags.push_back(pair.first);
		}
		return tags;
	}
}