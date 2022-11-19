#pragma once

#include "Blitter.h"

DEFINE_RLE_BLITTER(RLEBlitTransXlat)
{
public:
	inline explicit RLEBlitTransXlat(T* data) noexcept
	{
		PaletteData = data;
	}

	virtual ~RLEBlitTransXlat() override final = default;

	virtual void Blit_Copy(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust)
	{
		auto dest = reinterpret_cast<T*>(dst);

		Process_Pre_Lines<false, false>(dest, src, len, line, zbuf, abuf);

		auto handler = [this](T& dest, byte srcv)
		{
			dest = PaletteData[srcv];
		};

		Process_Pixel_Datas<false, false, false>(dest, src, len, zbase, zbuf, abuf, alvl, alpha_idx, zadjust, handler);
	}

	virtual void Blit_Copy_Tinted(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust, int tint)
	{
		Blit_Copy(dst, src, len, line, zbase, zbuf, abuf, alvl, alpha_idx, zadjust);
	}

private:
	T* PaletteData;
};
