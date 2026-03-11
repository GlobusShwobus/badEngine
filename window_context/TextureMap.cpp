#include "pch.h"
#include "TextureMap.h"

namespace badWindow
{
	void TextureMap::clear()
	{
		mTextures.clear();
	}

	bool TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, SDL_Surface* const surface)
	{
		Texture texture = make_texture(renderer, surface);
		return insert(std::move(texture), tag);
	}

	bool TextureMap::insert(SDL_Renderer* const renderer, const std::string& tag, const std::string& file_path)
	{
		Texture texture = make_texture(renderer, file_path);
		return insert(std::move(texture), tag);
	}

	bool TextureMap::insert(Texture&& texture, const std::string& tag) {
		if (texture) {
			auto find = mTextures.find(tag);
			if (find == mTextures.end()) {
				mTextures.emplace(tag, std::move(texture));
				return true;
			}
		}
		return false;
	}

	void TextureMap::remove(const std::string& tag)noexcept
	{
		mTextures.erase(tag);
	}

	SDL_Texture* TextureMap::get_texture(const std::string& tag)const noexcept
	{
		auto it = mTextures.find(tag);
		return it == mTextures.end() ? nullptr : it->second.get();
	}

	bool TextureMap::has(const std::string& tag)const noexcept
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