#include "pch.h"
#include "TextureMap.h"

namespace badWindow
{
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

	badCore::Sequence<std::string> TextureMap::get_tags()const 
	{
		badCore::Sequence<std::string> tags;
		tags.set_capacity(mTextures.size());
		for (const auto& pair : mTextures) {
			tags.push_back(pair.first);
		}
		return tags;
	}
}