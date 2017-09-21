#include "../core_include/api.h"
#include "../core_include/resource_type.h"
#include "../core_include/rect.h"
#include "../core_include/bitmap.h"
#include "../core_include/surface.h"

int c_bitmap::ms_z_order;
c_surface* c_bitmap::ms_surface;

void c_bitmap::draw_bitmap(const GUI_BITMAP *pBitmap, int x, int y)
{
	if (0 == pBitmap)
	{
		return;
	}

	draw_bitmap_565(x, y, pBitmap->XSize, pBitmap->YSize,
		(unsigned char const *)pBitmap->pData);
}

void c_bitmap::draw_bitmap_in_rect(const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type)
{
	if (0 == pBitmap)
	{
		return;
	}
	int x, y;
	get_bitmap_pos(pBitmap, rect, align_type, x, y);
	draw_bitmap_565_inrect(rect.m_left + x, rect.m_top + y,
			(rect.m_right - rect.m_left + 1), (rect.m_bottom - rect.m_top + 1),
			pBitmap->XSize, pBitmap->YSize, (unsigned char const *)pBitmap->pData);
}

void c_bitmap::get_bitmap_pos(const GUI_BITMAP *pBitmap, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size = pBitmap->XSize;
	int y_size = pBitmap->YSize;

	int height = rect.m_bottom - rect.m_top + 1;
	int width  = rect.m_right - rect.m_left + 1;

	x = y = 0;

	switch (align_type & ALIGN_HMASK)
	{
	case ALIGN_HCENTER:
		if (width > x_size)
		{
			x = (width - x_size)/2;
		}
		break;
	case ALIGN_LEFT:
		x = 0;
		break;
	case ALIGN_RIGHT:
		if (width > x_size)
		{
			x = width - x_size;
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	switch (align_type & ALIGN_VMASK)
	{
	case ALIGN_VCENTER:
		if (height > y_size)
		{
			y = (height - y_size)/2;
		}
		break;
	case ALIGN_TOP:
		y = 0;
		break;
	case ALIGN_BOTTOM:
		if (height > y_size)
		{
			y = height - y_size;
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

void c_bitmap::draw_bitmap_565(int x, int y, int xsize, int ysize, const unsigned char* pPixel)
{
	const unsigned short* pData;
	pData = (const unsigned short*)pPixel;
	int BytesPerLine;
	BytesPerLine = xsize;
	for (int j = 0; j < ysize; j++) 
	{
		const unsigned short * p = pData;
		for (int i = 0; i < xsize; i++) 
		{
			ms_surface->set_pixel(x + i, y + j, *p++, ms_z_order);
		}
		pData += BytesPerLine;
	}
}

void c_bitmap::draw_bitmap_565_inrect(int x, int y, int width, int height, int xsize, int ysize, const unsigned char* pPixel)
{
	const unsigned short* pData;
	pData = (const unsigned short*)pPixel;
	int BytesPerLine;
	BytesPerLine = xsize;
	for (int j = 0; j < ysize; j++)
	{
		if(j >= height)break;
		const unsigned short * p = pData;
		for (int i = 0; i < xsize; i++)
		{
			if(i >= width)break;
			ms_surface->set_pixel(x + i, y + j, *p++, ms_z_order);
		}
		pData += BytesPerLine;
	}
}
