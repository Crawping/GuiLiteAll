#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/resource_type.h"
#include "../core_include/word.h"
#include "../core_include/surface.h"
#include <string.h>
#include <stdio.h>

#define MERGE(high, low)	(low | (((unsigned short)high) << 8))

const MULTI_LAN_STRINGS * c_word::m_lang_map;
E_LANGUAGE c_word::m_lang_type;
c_word c_word::s_wordunit_array[MAX_TASK];

c_word::c_word()
{
	m_z_order = 0;
	m_pfont = 0;
	m_font_color = 0x0000ff; //BLUE COLOR
	m_back_color  = 0x000000; //BLACK COLOR
	
	m_lang_map = NULL;

	memset(m_buff, 0 , sizeof(m_buff));
	m_lang_type = ENGLISH;
	m_rect.m_left = 0;
	m_rect.m_top = 0;
	m_rect.m_right = 0;
	m_rect.m_bottom = 0;
	m_task_id = 0;
}

extern const MULTI_LAN_STRINGS *get_multi_lan_pointer();
extern const MULTI_LAN_UNICODES *get_unicode_lan_pointer();
void c_word::initiallize(E_LANGUAGE language)
{
    m_lang_map = get_multi_lan_pointer();
}

void c_word::draw_value_in_rect(int value, int dot_position, c_rect rect, unsigned int align_type)
{
	value_2_string(value, dot_position);
	draw_string_in_rect(m_buff, rect, align_type);
}

void c_word::draw_value(int value, int dot_position, int x, int y, unsigned int align_type)
{
	value_2_string(value, dot_position);
	draw_string(m_buff, x, y, align_type);
}

void c_word::draw_string_in_rect(unsigned long str_id, c_rect rect, unsigned int align_type)
{
	const char* s = get_string(str_id);
	if (NULL == s)
	{
		return;
	}
	draw_string_in_rect(s, rect, align_type);
}

void c_word::draw_string_in_rect(const char *s, c_rect rect, unsigned int align_type)
{
	if(0 == s)
	{
		return;
	}
	int x, y;
	get_string_pos(s, rect, align_type, x, y);
	draw_string(s, rect.m_left + x, rect.m_top + y, ALIGN_LEFT);
}

void c_word::draw_string(unsigned long str_id, int x, int y, unsigned int align_type)
{
	const char* s = get_string(str_id);
	if (NULL == s)
	{
		return;
	}
	draw_string(s, x, y, align_type);
}

void c_word::draw_string(const char *s, int x, int y, unsigned int align_type)
{
	if(0 == s)
	{
		return;
	}

	m_rect.m_left = 0;
	m_rect.m_top = 0;
	m_rect.m_right = get_dev_x_pixels();
	m_rect.m_bottom = get_dev_y_pixels();

	for (; *s; s++)
	{
		int line_len = get_char_cnt_in_single_line(s);
		int str_pixel_length = get_str_pixel_length(s);
		if (align_type == ALIGN_RIGHT)
		{
			x = x - str_pixel_length + 1;
		}
		else if (align_type == ALIGN_HCENTER)
		{
			x = x - str_pixel_length / 2;
		}
		draw_single_line_string(s, x, y , line_len);
		s += line_len;
		if (*s == '\n')
		{
			y += m_pfont->YSize;			
		}
		if (*s==0)
			break;
	}
}

const char* c_word::get_string(unsigned long str_id)
{
	if(NULL == m_lang_map)
	{
		return NULL;
	}

	if(str_id != m_lang_map[str_id].index)
	{
		ASSERT(FALSE);
		return NULL;
	}
	return m_lang_map[str_id].text[m_lang_type];
}

const char* c_word::get_string(int value, int dot_position)
{
	value_2_string( value, dot_position);
	return m_buff;
}

void c_word::set_font(const GUI_FONT* font_type)
{
	if (NULL == font_type)
	{
		ASSERT(0);
		return;
	}
	m_pfont = font_type;
}

const int c_word::get_font_ysize(const GUI_FONT* font_type)
{
	if (!font_type)
	{
		return 0;
	}
	return font_type->YSize;
}

void c_word::set_color(const unsigned int font_color, const unsigned int back_color, const unsigned int is_transparent_aa)
{
	m_font_color = font_color;
	m_back_color = back_color;
	m_is_transparent = is_transparent_aa;
}

void c_word::value_2_string(int value, int dot_position)
{
	sprintf(m_buff,"%d",value);
	int len = strlen(m_buff);

	if (len > (BUFFER_LEN - 1))
	{
		ASSERT(0);
		return ;
	}

	int i, point_pos;
	if (value >= 0)
	{
		if (len > dot_position)
		{		
			if (dot_position)
			{
				if (len + 1 > (BUFFER_LEN - 1))
				{
					ASSERT(0);
					return ;
				}

				point_pos = len - dot_position;

				for (i = len; i >= point_pos; i--)
				{
					m_buff[i + 1] = m_buff[i];
				}

				m_buff[point_pos] = '.';
				m_buff[len + 1] = '\0';
			}
		}
		else
		{
			if ((dot_position + 2) >= BUFFER_LEN)
			{
				ASSERT(0);
			}

			for (i = len; i >= 0; i--)
			{
				m_buff[i + dot_position + 2 - len] = m_buff[i];
			}
			for (i = 0; i< (dot_position + 2 - len); i++)
			{
				m_buff[i] = '0';
			}

			m_buff[1] = '.';
		}
	}
	else
	{//value < 0
		if ((len - 1) > dot_position)
		{		
			if (dot_position)
			{
				if (len + 1 > (BUFFER_LEN -1))
				{
					ASSERT(0);
					return ;
				}

				point_pos = len - dot_position;

				for (i = len; i >= point_pos; i--)
				{
					m_buff[i + 1] = m_buff[i];
				}

				m_buff[point_pos] = '.';
				m_buff[len + 1] = '\0';
			}
		}
		else
		{
			if ((dot_position + 2) >= BUFFER_LEN)
			{
				ASSERT(0);
			}

			for (i = len; i >= 0; i--)
			{
				m_buff[i + dot_position + 3 - len] = m_buff[i];
			}
			for (i = 0; i< (dot_position + 4 - len); i++)
			{
				m_buff[i] = '0';
			}

			m_buff[0] = '-';
			m_buff[2] = '.';
		}
	}
}

int c_word::get_char_cnt_in_single_line(const char *s) 
{
	int len = 0;
	unsigned char code_high;
	while (((code_high = *(unsigned char*)s) != 0)) 
	{
		if (code_high > 127) 
		{
			len++; s++;
		} 
		else 
		{
			switch (code_high) 
			{
			case '\n': return len;
			}
		}
		len++;
		s++;
	}
	return len;
}

void c_word::draw_single_line_string(const char *s, int x, int y, int len) 
{
	int offset = 0;
	while (--len >= 0) 
	{
		offset = draw_single_char(*s++, x, y );
		x += offset;
	}
}

const GUI_FONT_PROP* c_word::find_char(const GUI_FONT_PROP* pProp, unsigned short code) 
{
	for (; pProp; pProp=(GUI_FONT_PROP*) pProp->pNext) 
	{
		if ((code>=pProp->First) && (code<=pProp->Last))
			break;
	}
	return pProp;
}

int c_word::draw_single_char(unsigned short code, int x, int y ) 
{
	int bytes_per_line;
	int last_char_xdist = 0;

	if (0 == m_pfont)
	{
		return last_char_xdist;
	}

	const GUI_FONT_PROP* pProp = find_char(m_pfont->p.pProp, code);
	if (pProp) 
	{
		const GUI_CHARINFO* pCharInfo = pProp->paCharInfo+(code - pProp->First);
		bytes_per_line = pCharInfo->BytesPerLine;
		draw_lattice(x, y, pCharInfo->XSize, m_pfont->YSize,
			bytes_per_line, (unsigned char const *)pCharInfo->pData, is_AA_font(pCharInfo));
		last_char_xdist = pCharInfo->XDist;
	}
	return last_char_xdist;
}

void c_word::draw_lattice(int x, int y, int width, int height, 
						int bytes_per_line, const unsigned char* pData, unsigned int font_aa_flag)
{
	int i;
	if (!font_aa_flag)
	{
		for (i = 0; i < height; i++)
		{
			draw_bit_line(x, i + y, pData, width );
			pData += bytes_per_line;
		}
	}
	else
	{
		for (i = 0; i < height; i++)
		{
			draw_bit_line_AA(x, i + y, pData, width);
			pData += bytes_per_line;
		}
	}
}

void  c_word::draw_bit_line(int x, int y, unsigned char const*p, int width ) 
{
	unsigned int color;
	int diff = 0;
	do 
	{
		color = (*p & (0x80>>diff)) ? m_font_color : m_back_color;

		if (COLOR_TRANPARENT != color)
		{
			if ((m_rect.m_left <= x) && (x <= m_rect.m_right) && (m_rect.m_top <= y) && (y <= m_rect.m_bottom))
			{
				m_surface->set_pixel(x, y, color, m_z_order);
			}
		}

		x++;
		if (++diff == 8) 
		{
			diff = 0;
			p++;
		}
	} while (--width);
}

void  c_word::draw_bit_line_AA(int x, int y, unsigned char const*p, int width)
{
	unsigned int color,bk_color;
	unsigned int b, g, r;

	if(COLOR_TRANPARENT == m_back_color)
	{
		bk_color = m_surface->get_pixel(x, y, m_z_order);
	}
	else
	{
		bk_color = m_back_color;
	}
	do 
	{
		if (0x00 == *p)
		{
			if(!m_is_transparent)
			{
				if ((m_rect.m_left <= x) && (x <= m_rect.m_right) && (m_rect.m_top <= y) && (y <= m_rect.m_bottom))
				{
					m_surface->set_pixel(x, y, bk_color, m_z_order);
				}
			}
		}
		else
		{
			b = (GLT_RGB_B(m_font_color) * (*p) + GLT_RGB_B(bk_color) * (255 - *p)) >> 8;
			g = (GLT_RGB_G(m_font_color) * (*p) + GLT_RGB_G(bk_color) * (255 - *p)) >> 8;
			r = (GLT_RGB_R(m_font_color) * (*p) + GLT_RGB_R(bk_color) * (255 - *p)) >> 8;
			color = GLT_RGB(r, g, b);
			if ((m_rect.m_left <= x) && (x <= m_rect.m_right) && (m_rect.m_top <= y) && (y <= m_rect.m_bottom))
			{
				m_surface->set_pixel(x, y, color, m_z_order);
			}
		}
		x++;
		p ++;
	} while (--width);
}

int c_word::get_str_pixel_length(const char *s)
{
	int ret = 0;
	if(0 == s)
	{
		return ret;
	}

	for (; *s; s++)
	{
		int len = get_char_cnt_in_single_line(s);

		unsigned char code_high;
		int last_char_xdist = 0;

		while (--len >= 0) 
		{
			const GUI_FONT_PROP* pProp = find_char(m_pfont->p.pProp, *s);
			if (pProp) 
			{
				const GUI_CHARINFO* pCharInfo = pProp->paCharInfo+(*s - pProp->First);

				last_char_xdist = pCharInfo->XDist;
			}
			ret += last_char_xdist;
			s++;
		}
		if (*s==0)
			break;
	}

	return ret;
}

void c_word::get_string_pos(const char *s, c_rect rect, unsigned int align_type, int &x, int &y)
{
	int x_size = get_str_pixel_length(s);
	int y_size = get_font_ysize(m_pfont);

	int height = rect.m_bottom - rect.m_top + 1;
	int width  = rect.m_right - rect.m_left + 1;

	x = y = 0;

	switch (align_type & ALIGN_HMASK)
	{
	case ALIGN_HCENTER:
		//m_text_org_x=0
		if (width > x_size)
		{
			x = (width - x_size)/2;
		}
		break;

	case ALIGN_LEFT:
		x = 0;
		break;

	case ALIGN_RIGHT:
		//m_text_org_x=0
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
		//m_text_org_y=0
		if (height > y_size)
		{
			y = (height - y_size)/2;
		}
		break;

	case ALIGN_TOP:
		y = 0;
		break;

	case ALIGN_BOTTOM:
		//m_text_org_y=0
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

bool c_word::is_AA_font(const GUI_CHARINFO* pCharInfo)
{
	return  (pCharInfo->XSize == pCharInfo->BytesPerLine) ? true : false;
}

c_word* c_word::get_instance(unsigned int z_order, c_surface* gal)
{
	unsigned int task_id = get_current_task_id();

	//search in store.
	int i = 0;
	while (i < MAX_TASK)
	{
		if (task_id == s_wordunit_array[i].m_task_id)
		{
			s_wordunit_array[i].set_z_oder(z_order);
			s_wordunit_array[i].set_surface(gal);
			return &s_wordunit_array[i];
		}
		i++;
	}

	//new instance.
	i = 0;
	while (i < MAX_TASK)
	{
		if (0 == s_wordunit_array[i].m_task_id)
		{
			s_wordunit_array[i].m_task_id = task_id;
			s_wordunit_array[i].set_z_oder(z_order);
			s_wordunit_array[i].set_surface(gal);
			return &s_wordunit_array[i];
		}
		i++;
	}
	ASSERT(FALSE);
	return NULL;
}
