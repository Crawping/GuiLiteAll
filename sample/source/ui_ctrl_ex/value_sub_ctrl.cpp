#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/surface.h"
#include "../core_include/resource_type.h"
#include "../core_include/word.h"
#include "../gui_include/shape_resource.h"
#include "../source/manager/value_manager.h"

#include "value_ctrl.h"
#include "value_sub_ctrl.h"

void c_value_sub_ctrl::on_paint(void)
{
	c_rect rect;
	get_screen_rect(rect);
	fill_rect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom,m_bg_color);

	c_word *p = c_word::get_instance(m_z_order, m_surface);

	//show name
	p->set_font(m_name_font_type);
	p->set_color(m_name_color, m_bg_color);
	p->draw_string(m_name_id, rect.m_left + 1, rect.m_top);
	
	//show value
	p->set_font(m_value_font_type);
	p->set_color(m_name_color, m_bg_color);
	
	m_value_rect.m_left = rect.m_left + 32;
	m_value_rect.m_top = rect.m_top + (rect.Height() - p->get_font_ysize(m_value_font_type)) / 2;
	if (m_value_rect.m_top < rect.m_top)
	{
		m_value_rect.m_top = rect.m_top + 2;
	}

	int strLen = p->get_str_pixel_length(p->get_string(m_high_limit, m_limit_dot_position));
	m_value_rect.m_right = m_value_rect.m_left + strLen;

	if (m_value_rect.m_right > rect.m_right)
	{
		m_value_rect.m_right = rect.m_right - 2;
	}
	m_value_rect.m_bottom = m_value_rect.m_top + p->get_font_ysize(m_value_font_type);
	if (m_value_rect.m_bottom > rect.m_bottom)
	{
		m_value_rect.m_bottom = rect.m_bottom - 2;
	}

	value_2_string(m_value, m_value_in_str, m_limit_dot_position);
	m_max_value_rect = m_value_rect;
	p->draw_string_in_rect(m_value_in_str, m_value_rect, m_value_align_type);
}
