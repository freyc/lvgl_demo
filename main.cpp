#include "init.h"
#include "lvgl/lvgl.h"
#include <array>
#include <stdio.h>
#include <unistd.h>

static void button_cb(lv_event_t *ev) { printf("Click\n"); }

static void drag_event_handler(lv_event_t *e) {
    lv_obj_t *obj = lv_event_get_target(e);

    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    lv_obj_set_pos(obj, x, y);
    printf("drag %p to (%d, %d)\n", obj, x, y);
}

static void win_drag_event_handler(lv_event_t *e) {
    lv_obj_t *obj = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    lv_indev_t *indev = lv_indev_get_act();
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    if (vect.x || vect.y) {
        lv_coord_t x = lv_obj_get_x(obj) + vect.x;
        lv_coord_t y = lv_obj_get_y(obj) + vect.y;
        lv_obj_set_pos(obj, x, y);
        printf("drag %p to (%d, %d)\n", obj, x, y);
    }
}

static void dbg_event_handler(lv_event_t *e) {
    // printf("event %d\n", e->code);
}

static void ta_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta = lv_event_get_target(e);
    lv_obj_t *kb = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));
    if (code == LV_EVENT_FOCUSED) {
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }

    if (code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}

void lv_example_keyboard_1(void) {
    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    /*Create a keyboard to use it with an of the text areas*/
    lv_obj_t *kb = lv_keyboard_create(lv_scr_act());

    /*Create a text area. The keyboard will write here*/
    lv_obj_t *ta;
    ta = lv_textarea_create(lv_scr_act());
    lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
    // lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_placeholder_text(ta, "Hello");
    lv_obj_set_size(ta, 140, 80);

    ta = lv_textarea_create(lv_scr_act());
    lv_obj_align(ta, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
    lv_obj_set_size(ta, 140, 80);

    lv_keyboard_set_textarea(kb, ta);
}

//#include "examples/lv_examples.h"
#include "lvgl.hpp"
struct c_my_screen {

    lv_obj_t *screen;

    lv_obj_t *btn;
    lv_obj_t *lbl;
    lv_obj_t *bar;
    lv_obj_t *meter;

    lv_meter_scale_t *scale;
    lv_obj_t *led;
};

static void create_c_screen(c_my_screen *scr) {
    scr->screen = lv_obj_create(NULL);
    scr->btn = lv_btn_create(scr->screen);
    scr->lbl = lv_label_create(scr->btn);
}

class my_screen : public lvgl::screen {
  public:
    lvgl::button btn{this};
    lvgl::label lbl{&btn};
    lvgl::bar bar{this};

    lvgl::meter meter{this};
    lvgl::meter::scale scale{meter.add_scale()};
    lvgl::meter::indicator indic;

    lvgl::led led{this};
    lvgl::checkbox chkbox{this};
    lvgl::lv_switch sw{this};

    my_screen() : lvgl::screen{} {

        static lvgl::style btn_style;

        const void *p = nullptr;

        // lvgl::style::prop_value val{lv_color_black()};
        // lvgl::style::prop_value val{1u};
        lvgl::style::prop_value val{p};

        // btn_style.set_bg_color(lv_palette_main(LV_PALETTE_RED));
        btn_style.set_property(LV_STYLE_BG_COLOR,
                               lv_palette_main(LV_PALETTE_RED));
        btn.add_style(btn_style, lvgl::state::pressed);

        lbl.set_text("Hello world");
        btn.align(lvgl::alignment::center);
        // lbl.set_pos({50, 50});

        bar.set_pos(10, 10);
        bar.set_value(50);

        meter.set_pos(50, 50);
        meter.set_size(200, 200);
        meter.set_scale_ticks(scale);
        meter.set_scale_major_ticks(scale);

        auto i =
            meter.add_indicator_arc(scale, lv_palette_main(LV_PALETTE_BLUE));
        meter.set_indicator_start_value(i, 0);
        meter.set_indicator_end_value(i, 20);

        i = meter.add_indicator_lines(scale, lv_palette_main(LV_PALETTE_BLUE),
                                      lv_palette_main(LV_PALETTE_BLUE), false,
                                      0);
        meter.set_indicator_start_value(i, 0);
        meter.set_indicator_end_value(i, 20);

        i = meter.add_indicator_arc(scale, lv_palette_main(LV_PALETTE_RED));
        meter.set_indicator_start_value(i, 80);
        meter.set_indicator_end_value(i, 100);

        i = meter.add_indicator_lines(scale, lv_palette_main(LV_PALETTE_RED),
                                      lv_palette_main(LV_PALETTE_RED), false,
                                      0);
        meter.set_indicator_start_value(i, 80);
        meter.set_indicator_end_value(i, 100);

        i = meter.add_indicator_needle(scale, 4,
                                       lv_palette_main(LV_PALETTE_GREY), -10);

        meter.set_indicator_value(i, 50);

        indic = i;

        led.set_pos(20, 100);
        led.set_color(lv_palette_main(LV_PALETTE_RED));
        led.off();

        chkbox.set_text("enable foo");
        this->chkbox.set_pos(40, 500);

        sw.set_pos(40, 550);
    }
};

#include <array>

int main() {

    if (std::is_const_v<std::remove_pointer_t<const void *>>) {
        printf("is const\n");
    }

    // printf("c:   %d\n", sizeof(c_my_screen));
    // printf("c++: %d\n", sizeof(my_screen));

    lv_init();

    init_lvgl_hal();

    lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

    // MyScreen scr;
    // Screen::Load(scr);

    my_screen scr;
    lvgl::screen::load(scr);

    lvgl::animation anim{
        [&scr](int32_t val) { scr.meter.set_indicator_value(scr.indic, val); }};

    // lvgl::animation anim{[](int32_t val) {
    //
    //    ::printf("%d\n", val);
    //}};

    anim.set_range(0, 100);
    anim.set_time(5000);
    anim.set_repeat(0xffff);

    anim.set_playback_delay(1000);
    anim.set_playback_time(1000);

    anim.set_repeat_delay(1000);
    anim.start();

    while (1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task
         * too.*/
        lv_timer_handler();
        usleep(5 * 1000);
    }
}

#if 1

#include "lv_drv_conf.h"
/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */

#if USE_GTK
static void hal_init(void) {
    /* Use the 'monitor' driver which creates window on PC's monitor to
     * simulate a display*/

    gtkdrv_init();

    /*Create a display buffer*/
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[800 * 600];
    static lv_color_t buf1_2[800 * 600];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, buf1_2, 800 * 600);

    /*Create a display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf1;
    disp_drv.flush_cb = gtkdrv_flush_cb;
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 600;
    disp_drv.antialiasing = 1;
    disp_drv.full_refresh = 1;

    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_default_init(
        disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
        LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);

    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);

    /* Add the mouse as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    // mouse_init();

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;

    /*This function will be called periodically (by the library) to get
     * the mouse position and state*/
    indev_drv_1.read_cb = gtkdrv_mouse_read_cb;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    // keyboard_init();
    static lv_indev_drv_t indev_drv_2;
    lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
    indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_2.read_cb = gtkdrv_keyboard_read_cb;
    lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
    lv_indev_set_group(kb_indev, g);

    // mousewheel_init();
    // static lv_indev_drv_t indev_drv_3;
    // lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
    // indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
    // indev_drv_3.read_cb = gtkdrv_mouse_read_cb;

    // lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
    // lv_indev_set_group(enc_indev, g);

    /*Set a cursor for the mouse*/
    // LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
    // lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an
    // image object for the cursor */ lv_img_set_src(cursor_obj,
    // &mouse_cursor_icon);
    // /*Set the image source*/ lv_indev_set_cursor(mouse_indev,
    // cursor_obj);
    // /*Connect the image  object to the driver*/
}
#endif

#endif

#if 0

static int tick_thread(void *data) {
  (void)data;

  while(1) {
    SDL_Delay(5);
    lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
  }

  return 0;
}

#endif

#if 0
class ObjectBase {
  protected:
    lv_obj_t *_obj;

  protected:
    ObjectBase(lv_obj_t *obj) : _obj{obj} {}

    static auto getObject(ObjectBase &o) { return o._obj; }
};

template <typename T> class Object : public ObjectBase {
  private:
    friend T;

  protected:
    Object(lv_obj_t *obj) : ObjectBase{obj} {}

    template <typename CTOR, typename PT>
    Object(CTOR ctor, Object<PT> &parent)
        : ObjectBase{ctor(ObjectBase::getObject(parent))} {}

    auto getObject() { return ObjectBase::_obj; }

  public:
    void setPos(int x, int y) { lv_obj_set_pos(getObject(), x, y); }
};

class Screen : public Object<Screen> {
  private:
    Screen(lv_obj_t *obj) : Object{obj} {}

  public:
    Screen() : Object{lv_obj_create(NULL)} {}

    static Screen getCurrent() { return Screen{lv_scr_act()}; }

    static void Load(Screen &s) { lv_scr_load(s.getObject()); }
};

class Button : public Object<Button> {
  private:
  protected:
    static void eventCallback(lv_event_t *ev) {
        auto btn{reinterpret_cast<Button *>(lv_event_get_user_data(ev))};
        btn->onClick();
    }

    void onClick() { printf("Click\n"); }

  public:
    Button(const Button &) = delete;

    template <typename T>
    Button(Object<T> parent) : Object<Button>{lv_btn_create, parent} {

        lv_obj_add_event_cb(getObject(), Button::eventCallback,
                            LV_EVENT_CLICKED, this);
    }

    Button() : Button{Screen::getCurrent()} {}
};

class Label : public Object<Label> {

  public:
    template <typename T>
    Label(Object<T> parent) : Object<Label>{lv_label_create, parent} {
        auto r = getObject();
        lv_label_set_text(r, "Hello");
        lv_obj_center(r);
    }

    Label() : Label{Screen::getCurrent()} {}
};

class MyScreen : public Screen {

    Button btn1;
    Label lbl;

  public:
    MyScreen() : btn1{*this}, lbl{*this} { btn1.setPos(50, 50); }
};
#endif