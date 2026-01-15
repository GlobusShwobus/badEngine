#pragma once


namespace badEngine {

	////TODO:: FULLY ISOLATE TARGETABLE TEXTURE INTO IT'S OWN THING
	//class TargetTexture {
	//	using Texture_type2 = std::unique_ptr<SDL_Texture, SDLDeleter<SDL_Texture, SDL_DestroyTexture>>;
	//public:
	//
	//	TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx)
	//	{
	//		SDL_Texture* txtr = gfx.create_texture_targetable(w, h);
	//		assert(txtr != nullptr);
	//		mTexture.reset(txtr);
	//	}
	//	TargetTexture(Uint32 w, Uint32 h, const GraphicsSys& gfx, SDL_Texture* copy_from, AABB* src = nullptr, AABB* dest = nullptr)
	//	{
	//		SDL_Texture* txtr = gfx.create_texture_targetable(w, h, copy_from, src, dest);
	//		assert(txtr != nullptr);
	//		mTexture.reset(txtr);
	//	}
	//
	//	Texture_type2 mTexture;
	//};
	////kinda not finished, but not sure what utility it needs untill i actually use it more than just a dumb canvas
	//class Canvas
	//{
	//public:
	//	Canvas(const TargetTexture& texture)
	//		:temporary_ass(texture.mTexture.get())
	//	{
	//	}
	//
	//	bool start_drawing(const GraphicsSys& gfx)const noexcept
	//	{
	//		return true; //gfx.set_render_target(mTexture);
	//	}
	//
	//	bool end_drawing(const GraphicsSys& gfx)const noexcept
	//	{
	//		return true; //gfx.set_render_target(nullptr);
	//	}
	//	SDL_Texture* temporary_ass;
	//};

}
