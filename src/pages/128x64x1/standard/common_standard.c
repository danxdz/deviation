/*
 This project is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Deviation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Deviation.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OVERRIDE_PLACEMENT
#include "common.h"
#include "gui/gui.h"
#include "config/model.h"
#include "../pages.h"

enum {
    //"Reverse", "Subtrim" and "Fail-safe" pages
    FIELD_X        = 63,
    FIELD_WIDTH    = 60,
    LABEL_X        = 0,
    LABEL_WIDTH    = FIELD_X - LABEL_X,
    //"Throttle curve" and "Pitch curve" pages XY-graph points
    #define LINE_Y LINE_SPACE
    WIDTH1         = 6,
    WIDTH2         = 32,
    WIDTH2_ADD     = 2,
    LINE_H         = 9,
    LINE_H_OFFS    = 1,
    M_LABEL_X      = 20,
    M_LEBEL_Y_OFFS = 0,
};
#endif //OVERRIDE_PLACEMENT

#if HAS_STANDARD_GUI
#include "standard.h"
#include "../../common/standard/_common_standard.c"

static struct stdchan_obj * const gui = &gui_objs.u.stdchan;

//"Reverse", "Subtrim" and "Fail-safe" pages
static int row_cb(int absrow, int relrow, int y, void *data)
{
    struct page_defs *page_defs = (struct page_defs *)data;
    (void)data;

    GUI_CreateLabelBox(&gui->name[relrow], LABEL_X, y,
            LABEL_WIDTH, LINE_HEIGHT, &LABEL_FONT, STDMIX_channelname_cb, NULL, (void *)(long)absrow);
    GUI_CreateTextSelectPlate(&gui->value[relrow], FIELD_X, y,
            FIELD_WIDTH, LINE_HEIGHT, &DEFAULT_FONT, page_defs->tgl, page_defs->value, (void *)(long)absrow);
    return 1;
}

void STANDARD_Init(const struct page_defs *page_defs)
{
    PAGE_SetModal(0);
    PAGE_RemoveAllObjects();
    PAGE_ShowHeader(_tr(page_defs->title));
    GUI_CreateScrollable(&gui->scrollable, 0, HEADER_HEIGHT, LCD_WIDTH, LCD_HEIGHT - HEADER_HEIGHT,
                     LINE_SPACE, Model.num_channels, row_cb, NULL, NULL, (void *)page_defs);
    GUI_SetSelected(GUI_ShowScrollableRowOffset(&gui->scrollable, 0));
}

//"Throttle curve" and "Pitch curve" pages XY-graph points
void STANDARD_DrawCurvePoints(guiLabel_t vallbl[], guiTextSelect_t val[],
        u8 selectable_bitmap,
        void (*press_cb)(guiObject_t *obj, void *data),
        const char *(*set_pointval_cb)(guiObject_t *obj, int value, void *data))
{
    u8 y  = LINE_Y;
    u8 w1 = WIDTH1;
    u8 w2 = WIDTH2;
    u8 x = 0;
    u8 height = LINE_H;
    GUI_CreateLabelBox(&vallbl[0], x, y,  w1, height, &TINY_FONT, NULL, NULL, "L");
    x += w1;
    GUI_CreateTextSelectPlate(&val[0], x, y, w2, height, &TINY_FONT, NULL, set_pointval_cb, (void *)(long)0);
    x += w2 + WIDTH2_ADD;
    GUI_CreateLabelBox(&vallbl[8], x, y,  w1, height, &TINY_FONT, NULL, NULL, "H");
    x += w1;
    GUI_CreateTextSelectPlate(&val[8], x, y, w2, height, &TINY_FONT, NULL, set_pointval_cb, (void *)(long)8);

    y += height + LINE_H_OFFS + M_LEBEL_Y_OFFS;
    x = M_LABEL_X;
    GUI_CreateLabelBox(&vallbl[4], x, y,  w1, height, &TINY_FONT, NULL, NULL, "M");
    x += w1;
    GUI_CreateTextSelectPlate(&val[4], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)4);

    y += height + LINE_H_OFFS + M_LEBEL_Y_OFFS;
    x = 0;
    GUI_CreateLabelBox(&vallbl[1], x, y,  w1, height, &TINY_FONT, NULL, NULL, "2");
    x += w1;
    GUI_CreateTextSelectPlate(&val[1], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)1);
    x += w2 + WIDTH2_ADD;
    GUI_CreateLabelBox(&vallbl[2], x, y,  w1, height, &TINY_FONT, NULL, NULL, "3");
    x += w1;
    GUI_CreateTextSelectPlate(&val[2], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)2);

    y += height + LINE_H_OFFS;
    x = 0;
    GUI_CreateLabelBox(&vallbl[3], x, y,  w1, height, &TINY_FONT, NULL, NULL, "4");
    x += w1;
    GUI_CreateTextSelectPlate(&val[3], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)3);
    x += w2 + WIDTH2_ADD;
    GUI_CreateLabelBox(&vallbl[5], x, y,  w1, height, &TINY_FONT, NULL, NULL, "6");
    x += w1;
    GUI_CreateTextSelectPlate(&val[5], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)5);

    y += height + LINE_H_OFFS;
    x = 0;
    GUI_CreateLabelBox(&vallbl[6], x, y,  w1, height, &TINY_FONT, NULL, NULL, "7");
    x += w1;
    GUI_CreateTextSelectPlate(&val[6], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)6);
    x += w2 + WIDTH2_ADD;
    GUI_CreateLabelBox(&vallbl[7], x, y,  w1, height, &TINY_FONT, NULL, NULL, "8");
    x += w1;
    GUI_CreateTextSelectPlate(&val[7], x, y, w2, height, &TINY_FONT, press_cb, set_pointval_cb, (void *)(long)7);

    //update_textsel_state();
    for (u8 i = 1; i < 8; i++) {
        GUI_TextSelectEnablePress(&val[i], 1);
        if (selectable_bitmap >> (i - 1) & 0x01) {
            GUI_TextSelectEnable(&val[i], 1);
        } else {
            GUI_TextSelectEnable(&val[i], 0);
        }
    }
}
#endif //HAS_STANDARD_GUI
