#include "../core_include/api.h"
#include "../core_include/rect.h"
#include "../core_include/cmd_target.h"
#include "../core_include/wnd.h"
#include "../core_include/msg.h"
#include "../gui_include/button.h"
#include "../gui_include/font.h"
#include "../gui_include/dialog.h"
#include "../core_include/surface.h"
#include "../gui_include/shape_resource.h"
#include "../core_include/resource_type.h"
#include "../core_include/bitmap.h"
#include "../core_include/word.h"
#include <string.h>

DIALOG_ARRAY c_dialog::ms_the_dialogs[MAX_DIALOG];
void c_dialog::pre_create_wnd()
{
	m_style |= GLT_ATTR_VISIBLE|GLT_ATTR_FOCUS;
	m_z_order = Z_ORDER_LEVEL_1;
	m_bg_color = GLT_RGB(33,33,33);
}

void c_dialog::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);

	m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_bg_color, m_z_order);

	int start_y = 29;
	int interval = 41;
	//draw border
	if ( m_divider_lines )
	{
		for ( unsigned int i = 0; i < m_divider_lines; i++ )
		{
			m_surface->draw_hline( rect.m_left + 35, rect.m_right - 35, rect.m_top + start_y + (i * interval), GLT_RGB(70, 73, 76), m_z_order);
		}
	}

	if (m_str_id)
	{
		c_word *p = c_word::get_instance(m_z_order, m_surface);
		p->set_font(FONT_ENG_MB_AA());
		p->set_color(GLT_RGB(255,255,255), COLOR_TRANPARENT);
		p->draw_string(m_str_id, rect.m_left+35, rect.m_top, ALIGN_LEFT);
	}
}

c_dialog* c_dialog::get_the_dialog(c_surface* surface)
{
	for(int i = 0; i < MAX_DIALOG; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			return ms_the_dialogs[i].dialog;
		}
	}
	return NULL;
}

int c_dialog::open_dialog(c_dialog* p_dlg)
{
	if (NULL == p_dlg)
	{
		ASSERT(FALSE);
		return 0;
	}
	c_dialog* cur_dlg = get_the_dialog(p_dlg->get_surface());
	if (cur_dlg == p_dlg)
	{
		return 1;
	}

	if(cur_dlg)
	{
		cur_dlg->show_window(GLT_WIN_HIDE);
		cur_dlg->modify_style(0, GLT_ATTR_VISIBLE);
	}

	c_rect rc;
	p_dlg->get_screen_rect(rc);
	p_dlg->get_surface()->set_frame_layer(rc, Z_ORDER_LEVEL_1);

	p_dlg->modify_style(GLT_ATTR_VISIBLE, 0);
	p_dlg->show_window();
	p_dlg->set_me_the_dialog();
	return 1;
}

int c_dialog::close_dialog()
{
	c_dialog* dlg = get_the_dialog(get_surface());

	if (NULL == dlg)
	{
		return 0;
	}
	c_rect rc;
	c_surface* surface = dlg->get_surface();
	dlg->show_window(GLT_WIN_HIDE);
	dlg->modify_style(0, GLT_ATTR_VISIBLE);
	surface->set_frame_layer(rc, dlg->m_z_order);

	//clear the dialog
	for(int i = 0; i < MAX_DIALOG; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = NULL;
			return 1;
		}
	}
	ASSERT(FALSE);
	return -1;
}

void c_dialog::on_touch_down(int x, int y)
{
	c_wnd *child = m_top_child;
	c_rect rect;
	get_wnd_rect(rect);

	if ( NULL != child )
	{
		while ( child )
		{
			if (child->m_z_order > m_z_order)
			{
				x -= rect.m_left;
				y -= rect.m_top;
				child->on_touch_down(x, y);
				return;
			}
			child = child->m_next_sibling;
		}
	}
	c_wnd::on_touch_down(x, y);
}

void c_dialog::on_touch_up(int x, int y)
{
	c_wnd *child = m_top_child;
	c_rect rect;
	get_wnd_rect(rect);
	if ( NULL != child )
	{
		while ( child )
		{
			if (child->m_z_order > m_z_order)
			{
				x -= rect.m_left;
				y -= rect.m_top;
				return child->on_touch_up(x, y);
			}
			child = child->m_next_sibling;
		}
	}
	c_wnd::on_touch_up(x, y);
}

int c_dialog::set_me_the_dialog()
{
	c_surface* surface = get_surface();
	for(int i = 0; i < MAX_DIALOG; i++)
	{
		if(ms_the_dialogs[i].surface == surface)
		{
			ms_the_dialogs[i].dialog = this;
			return 0;
		}
	}

	for(int i = 0; i < MAX_DIALOG; i++)
	{
		if(ms_the_dialogs[i].surface == NULL)
		{
			ms_the_dialogs[i].dialog = this;
			if(this)
			{
				ms_the_dialogs[i].surface = surface;
			}
			return 1;
		}
	}
	ASSERT(FALSE);
	return -2;
}
