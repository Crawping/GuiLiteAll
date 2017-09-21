#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include "../core_include/resource_type.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include "../gui_include/font.h"
#include "../gui_include/label.h"

void c_label::pre_create_wnd()
{
	m_style = GLT_ATTR_VISIBLE | ALIGN_LEFT | ALIGN_VCENTER;
	m_font_color = GLT_RGB(255,255,255);

	m_font_type = FONT_ENG_SMB_AA();
	m_bg_color = get_parent()->get_bg_color();
}

void c_label::on_paint(void)
{
	c_rect rect;
	get_screen_rect(rect);

	if (m_bitmap)
	{
		fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom,m_bg_color);
		draw_bitmap_in_rect(m_bitmap, rect, m_style);
	}

	if (m_str_id)
	{
		fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom,m_bg_color);
		c_word *p = c_word::get_instance(m_z_order, m_surface);
		p->set_font(m_font_type);
		p->set_color(m_font_color, COLOR_TRANPARENT);
		p->draw_string_in_rect(m_str_id, rect, m_style);
	}
}
