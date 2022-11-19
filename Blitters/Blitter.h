#pragma once

#include <YRPPCore.h>

#include <Drawing.h>

// All Westwood fucking jesus blitters goes here!
class Blitter
{
public:
	virtual ~Blitter() = default;
	virtual void Blit_Copy(void* dst, byte* src, int len, int zval, WORD* zbuf, WORD* abuf, int alvl, int warp) = 0;
	virtual void Blit_Copy_Tinted(void* dst, byte* src, int len, int zval, WORD* zbuf, WORD* abuf, int alvl, int warp, int tint) = 0;
	virtual void Blit_Move(void* dst, byte* src, int len, int zval, WORD* zbuf, WORD* abuf, int alvl, int warp) = 0;
	virtual void Blit_Move_Tinted(void* dst, byte* src, int len, int zval, WORD* zbuf, WORD* abuf, int alvl, int warp, int tint) = 0;
};

// And those are compressed one :(
class RLEBlitter
{
public:
	virtual ~RLEBlitter() = default;
	virtual void Blit_Copy(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust) = 0;
	virtual void Blit_Copy_Tinted(void* dst, byte* src, int len, int line, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust, int tint) = 0;

protected:
	template<bool UseZBuffer, bool UseABuffer, typename T>
	inline static void Process_Pre_Lines(T*& dest, byte*& src, int& len, const int& line, WORD* zbuf, WORD* abuf)
	{
		if (line > 0)
		{
			int off = -line;
			do
			{
				if (*src++)
					++off;
				else
					off += *src++;
			}
			while (off < 0);

			dest += off;
			len -= off;

			if constexpr (UseZBuffer)
			{
				zbuf += off;
				ZBuffer::Instance->AdjustPointer(zbuf);
			}
			if constexpr (UseABuffer)
			{
				abuf += off;
				ABuffer::Instance->AdjustPointer(abuf);
			}
		}
	}

	template<bool UseZBuffer, bool UseABuffer, bool UseZAdjust, typename T, typename Fn>
	inline static void Process_Pixel_Datas(T* dest, byte* src, int len, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust, Fn f)
	{
		if (len < 0)
			return;

		while (len > 0)
		{
			if (byte srcv = *src++)
			{
				if constexpr (UseZBuffer && UseABuffer)
				{
					if constexpr (UseZAdjust)
						f(*dest, srcv, zbase, *zbuf++, *abuf++, alvl, alpha_idx, *zadjust++);
					else
						f(*dest, srcv, zbase, *zbuf++, *abuf++, alvl, alpha_idx);
				}
				else if constexpr (UseZBuffer && !UseABuffer)
				{
					if constexpr (UseZAdjust)
						f(*dest, srcv, zbase, *zbuf++, *zadjust++);
					else
						f(*dest, srcv, zbase, *zbuf++);
				}
				else if constexpr (!UseZBuffer && UseABuffer)
					f(*dest, srcv, *abuf++, alvl, alpha_idx);
				else // !UseZBuffer && !UseABuffer
					f(*dest, srcv);

				++dest;
				--len;
			}
			else
			{
				byte off = *src++;
				len -= off;
				dest += off;

				if constexpr (UseZBuffer)
				{
					zbuf += off;
					zadjust += off;
				}
				if constexpr (UseABuffer)
					abuf += off;
			}

			if constexpr (UseZBuffer)
				ZBuffer::Instance->AdjustPointer(zbuf);
			if constexpr (UseABuffer)
				ABuffer::Instance->AdjustPointer(abuf);
		}
	}

	template<bool UseZBuffer, bool UseABuffer, bool UseZAdjust, bool UseTint, typename T, typename Fn>
	inline static void Process_Pixel_Datas_Tinted(T* dest, byte* src, int len, int zbase, WORD* zbuf, WORD* abuf, int alvl, int alpha_idx, byte* zadjust, int tint, Fn f)
	{
		if (len < 0)
			return;

		while (len > 0)
		{
			if (byte srcv = *src++)
			{
				if constexpr (UseTint)
				{
					if constexpr (UseZBuffer && UseABuffer)
					{
						if constexpr (UseZAdjust)
							f(*dest, srcv, zbase, *zbuf++, *zadjust++, *abuf++, alvl, alpha_idx);
						else
							f(*dest, srcv, zbase, *zbuf++, *abuf++, alvl, alpha_idx);
					}
					else if constexpr (UseZBuffer && !UseABuffer)
					{
						if constexpr (UseZAdjust)
							f(*dest, srcv, zbase, *zbuf++, *zadjust++);
						else
							f(*dest, srcv, zbase, *zbuf++);
					}
					else if constexpr (!UseZBuffer && UseABuffer)
						f(*dest, srcv, *abuf++, alvl, alpha_idx);
					else // !UseZBuffer && !UseABuffer
						f(*dest, srcv);
				}
				else
				{
					if constexpr (UseZBuffer && UseABuffer)
					{
						if constexpr (UseZAdjust)
							f(*dest, srcv, zbase, *zbuf++, *zadjust++, *abuf++, alvl, alpha_idx, tint);
						else
							f(*dest, srcv, zbase, *zbuf++, *abuf++, alvl, alpha_idx, tint);
					}
					else if constexpr (UseZBuffer && !UseABuffer)
					{
						if constexpr (UseZAdjust)
							f(*dest, srcv, zbase, *zbuf++, *zadjust++, tint);
						else
							f(*dest, srcv, zbase, *zbuf++, tint);
					}
					else if constexpr (!UseZBuffer && UseABuffer)
						f(*dest, srcv, *abuf++, alvl, alpha_idx, tint);
					else // !UseZBuffer && !UseABuffer
						f(*dest, srcv, tint);
				}
				++dest;
				--len;
			}
			else
			{
				byte off = *src++;
				len -= off;
				dest += off;

				if constexpr (UseZBuffer)
				{
					zbuf += off;
					zadjust += off;
				}
				if constexpr (UseABuffer)
					abuf += off;
			}

			if constexpr (UseZBuffer)
				ZBuffer::Instance->AdjustPointer(zbuf);
			if constexpr (UseABuffer)
				ABuffer::Instance->AdjustPointer(abuf);
		}
	}
};

#define DEFINE_BLITTER(x) \
template<typename T> \
class x final : public Blitter

#define DEFINE_RLE_BLITTER(x) \
template<typename T> \
class x final : public RLEBlitter
