#include "init.h"
#include "lvgl/lvgl.h"
#include <array>
#include <stdio.h>
#include <thread>
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
#include "lvgl_display_driver.hpp"
#include "lvgl_driver.hpp"

class my_screen : public lvgl::screen {
  public:
    lvgl::button btn{this};
    lvgl::label lbl{&btn};
    lvgl::bar bar{this};
    lvgl::slider slider{this};

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

        slider.set_pos(10, 400);

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

#include <SDL2/SDL.h>

class sdl_input_handler {

    struct mouse_data {
        lv_coord_t x;
        lv_coord_t y;
        bool pressed;
    };

    mouse_data last_mouse_data;
    class sdl_mouse_input_driver
        : public lvgl::drivers::input_pointer_driver<sdl_mouse_input_driver> {
        mouse_data &data;

      public:
        sdl_mouse_input_driver(mouse_data &data) : data{data} {}

        void get_pointer_state(lv_coord_t &x, lv_coord_t &y,
                               lv_indev_state_t &state) {
            x = data.x;
            y = data.y;
            state =
                data.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
        }
    };

    sdl_mouse_input_driver mouse_driver;

    void event_handler() {
        /*Refresh handling*/
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // mouse_handler(&event);
            // mousewheel_handler(&event);
            // keyboard_handler(&event);

            if ((&event)->type == SDL_WINDOWEVENT) {
                switch ((&event)->window.event) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
                case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
                case SDL_WINDOWEVENT_EXPOSED:
                    // window_update();
                    break;
                default:
                    break;
                }
            }
        }

        /*Run until quit event not arrives*/
        // if (sdl_quit_qry) {
        //    monitor_sdl_clean_up();
        //    exit(0);
        //}
    }

    static void event_handler(lv_timer_t *t) {
        (void)t;
        auto self = reinterpret_cast<sdl_input_handler *>(t->user_data);

        self->event_handler();
    }

  public:
    sdl_input_handler() : mouse_driver{last_mouse_data} {}
};

template <lv_coord_t Hor, lv_coord_t Ver, unsigned int Zoom = 100>
class dummy_display_driver : public lvgl::drivers::display_driver<
                                 dummy_display_driver<Hor, Ver, Zoom>>,
                             public lvgl::drivers::input_pointer_driver<
                                 dummy_display_driver<Hor, Ver, Zoom>>,
                             public lvgl::drivers::input_keyboard_driver<
                                 dummy_display_driver<Hor, Ver, Zoom>> {

    struct monitor_t {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        volatile bool sdl_refr_qry;
        uint32_t *tft_fb;
    };

    monitor_t monitor;
    volatile bool sdl_quit_qry;

    static int quit_filter(void *userdata, SDL_Event *event) {
        auto self =
            reinterpret_cast<dummy_display_driver<Hor, Ver, Zoom> *>(userdata);

        if (event->type == SDL_WINDOWEVENT) {
            if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
                self->sdl_quit_qry = true;
            }
        } else if (event->type == SDL_QUIT) {
            self->sdl_quit_qry = true;
        }

        return 1;
    }

    void window_create() {

        int flag = 0;

        monitor.window = SDL_CreateWindow(
            "TFT Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            Hor * Zoom / 100.0, Ver * Zoom / 100.0,
            flag); /*last param. SDL_WINDOW_BORDERLESS to hide borders*/

        monitor.renderer =
            SDL_CreateRenderer(monitor.window, -1, SDL_RENDERER_SOFTWARE);
        monitor.texture =
            SDL_CreateTexture(monitor.renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STATIC, Hor, Ver);
        SDL_SetTextureBlendMode(monitor.texture, SDL_BLENDMODE_BLEND);

        /*Initialize the frame buffer to gray (77 is an empirical value) */
        monitor.tft_fb = (uint32_t *)malloc(sizeof(uint32_t) * Hor * Ver);
        memset(monitor.tft_fb, 0x44, Hor * Ver * sizeof(uint32_t));

        monitor.sdl_refr_qry = true;
    }

    bool left_button_down;
    lv_coord_t last_x, last_y;
    char buf[SDL_TEXTINPUTEVENT_TEXT_SIZE];

    void monitor_sdl_clean_up() {
        SDL_DestroyTexture(monitor.texture);
        SDL_DestroyRenderer(monitor.renderer);
        SDL_DestroyWindow(monitor.window);
        SDL_Quit();
    }

    static uint32_t keycode_to_ctrl_key(SDL_Keycode sdl_key) {
        /*Remap some key to LV_KEY_... to manage groups*/
        switch (sdl_key) {
        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            return LV_KEY_RIGHT;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            return LV_KEY_LEFT;

        case SDLK_UP:
            return LV_KEY_UP;

        case SDLK_DOWN:
            return LV_KEY_DOWN;

        case SDLK_ESCAPE:
            return LV_KEY_ESC;

        case SDLK_BACKSPACE:
            return LV_KEY_BACKSPACE;

        case SDLK_DELETE:
            return LV_KEY_DEL;

        case SDLK_KP_ENTER:
        case '\r':
            return LV_KEY_ENTER;

        case SDLK_TAB:
        case SDLK_PAGEDOWN:
            return LV_KEY_NEXT;

        case SDLK_PAGEUP:
            return LV_KEY_PREV;

        default:
            return '\0';
        }
    }

    void mouse_handler(SDL_Event *event) {
        switch (event->type) {
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT)
                left_button_down = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT) {
                left_button_down = true;
                last_x = event->motion.x / (Zoom / 100.0);
                last_y = event->motion.y / (Zoom / 100.0);
            }
            break;
        case SDL_MOUSEMOTION:
            last_x = event->motion.x / (Zoom / 100.0);
            last_y = event->motion.y / (Zoom / 100.0);
            break;

        case SDL_FINGERUP:
            left_button_down = false;
            last_x = Hor * event->tfinger.x / (Zoom / 100.0);
            last_y = Ver * event->tfinger.y / (Zoom / 100.0);
            break;
        case SDL_FINGERDOWN:
            left_button_down = true;
            last_x = Hor * event->tfinger.x / (Zoom / 100.0);
            last_y = Ver * event->tfinger.y / (Zoom / 100.0);
            break;
        case SDL_FINGERMOTION:
            last_x = Hor * event->tfinger.x / (Zoom / 100.0);
            last_y = Ver * event->tfinger.y / (Zoom / 100.0);
            break;
        }
    }

    void keyboard_handler(SDL_Event *event) {
        /* We only care about SDL_KEYDOWN and SDL_TEXTINPUT events */
        switch (event->type) {
        case SDL_KEYDOWN: /*Button press*/
        {
            const uint32_t ctrl_key =
                keycode_to_ctrl_key(event->key.keysym.sym);
            if (ctrl_key == '\0')
                return;
            const size_t len = strlen(buf);
            if (len < SDL_TEXTINPUTEVENT_TEXT_SIZE - 1) {
                buf[len] = ctrl_key;
                buf[len + 1] = '\0';
            }
            break;
        }
        case SDL_TEXTINPUT: /*Text input*/
        {
            const size_t len = strlen(buf) + strlen(event->text.text);
            if (len < SDL_TEXTINPUTEVENT_TEXT_SIZE - 1)
                strcat(buf, event->text.text);
        } break;
        default:
            break;
        }
    }

    void sdl_event_handler() {
        /*Refresh handling*/
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            mouse_handler(&event);
            // mousewheel_handler(&event);
            keyboard_handler(&event);

            if ((&event)->type == SDL_WINDOWEVENT) {
                switch ((&event)->window.event) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
                case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
                case SDL_WINDOWEVENT_EXPOSED:
                    window_update();
#if SDL_DUAL_DISPLAY
                    window_update(&monitor2);
#endif
                    break;
                default:
                    break;
                }
            }
        }

        /*Run until quit event not arrives*/
        if (sdl_quit_qry) {
            monitor_sdl_clean_up();
            exit(0);
        }
    }

    static void sdl_event_handler(lv_timer_t *t) {
        (void)t;
        auto self = reinterpret_cast<dummy_display_driver<Hor, Ver, Zoom> *>(
            t->user_data);

        self->sdl_event_handler();
    }

    static int tick_thread(void *data) {
        (void)data;

        while (1) {
            SDL_Delay(5);
            lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
        }

        return 0;
    }

  public:
    template <typename Buffer>
    dummy_display_driver(lvgl::drivers::draw_buffer<Buffer> &buffer)
        : lvgl::drivers::display_driver<
              dummy_display_driver<Hor, Ver, Zoom>>{buffer},
          lvgl::drivers::input_pointer_driver<
              dummy_display_driver<Hor, Ver, Zoom>>{} {

        /*Initialize the SDL*/
        SDL_Init(SDL_INIT_VIDEO);

        SDL_SetEventFilter(quit_filter, this);

        window_create();

        // sdl_inited = true;

        SDL_StartTextInput();

        /* Tick init.
         * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL
         * about how much time were elapsed Create an SDL thread to do this*/
        SDL_CreateThread(tick_thread, "tick", NULL);

        lv_timer_create(sdl_event_handler, 10, this);
    }

    auto get_x_res() const { return Hor; }

    auto get_y_res() const { return Ver; }

    void window_update() {
        SDL_UpdateTexture(monitor.texture, NULL, monitor.tft_fb,
                          Hor * sizeof(uint32_t));

        SDL_RenderClear(monitor.renderer);

        /*Update the renderer with the texture containing the rendered image*/
        SDL_RenderCopy(monitor.renderer, monitor.texture, NULL, NULL);
        SDL_RenderPresent(monitor.renderer);
    }

    void monitor_sdl_refr(void *t) {
        (void)t;

        /*Refresh handling*/
        if (monitor.sdl_refr_qry != false) {
            monitor.sdl_refr_qry = false;
            window_update();
        }
    }

    void flush_display(const lvgl::area_t &area, lvgl::color_t *color_p) {

        lv_coord_t hres = Hor;
        lv_coord_t vres = Ver;

        /*Return if the area is out the screen*/
        if (area.x2 < 0 || area.y2 < 0 || area.x1 > hres - 1 ||
            area.y1 > vres - 1) {
            this->flush_ready();
            // flush_ready();
            // lv_disp_flush_ready(disp_drv);
            return;
        }

        int32_t y;
        uint32_t w = area.get_width();
        for (y = area.y1; y <= area.y2 && y < Ver; y++) {
            memcpy(&monitor.tft_fb[y * Hor + area.x1], color_p,
                   w * sizeof(lv_color_t));
            color_p += w;
        }

        monitor.sdl_refr_qry = true;

        /* TYPICALLY YOU DO NOT NEED THIS
         * If it was the last part to refresh update the texture of the
         * window.*/
        if (this->flush_is_last()) {
            monitor_sdl_refr(NULL);
        }

        /*IMPORTANT! It must be called to tell the system the flush is ready*/
        this->flush_ready();
    }

    void get_pointer_state(lv_coord_t &x, lv_coord_t &y,
                           lv_indev_state_t &state) {
        x = last_x;
        y = last_y;
        state =
            left_button_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    }

    bool get_keyboard_state(uint32_t &key, lv_indev_state_t &state) {
        static bool dummy_read = false;
        const size_t len = strlen(buf);

        /*Send a release manually*/
        if (dummy_read) {
            dummy_read = false;
            state = LV_INDEV_STATE_RELEASED;
            return len > 0;
        } else if (len > 0) {
            dummy_read = true;
            state = LV_INDEV_STATE_PRESSED;
            key = buf[0];
            memmove(buf, buf + 1, len);
            return true;
        }

        return false;
    }
};

#if 0
class base {
    void *ptr;

  protected:
    base(void *p) : ptr(p) {}
    void release() { ptr = nullptr; }

  public:
    base() : ptr{malloc(1024)} {}

    ~base() {
        if (ptr) {
            printf("freeing in base\n");
            free(ptr);
        }
    }

    // base(const base &b) : ptr{b.ptr} {}
};

class derived : public base {

  public:
    derived() : base{} {}

    ~derived() {}
};

template <typename T> class non_owning_wrapper : public T {

  public:
    template <typename... Args>
    non_owning_wrapper(Args... args) : T{std::forward<Args>(args)...} {}

    // non_owning_wrapper(const non_owning_wrapper &o) = default;
    ~non_owning_wrapper() { this->release(); }

    auto operator=(const T &o) {
        T::operator=(o);
        return *this;
    }
};

struct c_obj {
    int i;
};

struct c_obj *create_obj() {
    return (struct c_obj *)malloc(sizeof(struct c_obj));
}

void del_obj(struct c_obj *o) { free(o); }

static c_obj *current;
struct c_obj *get_current() {
    static c_obj _inst;
    return &_inst;
};

class obj {
    friend class non_owning_wrapper<obj>;

    c_obj *o;

    obj(c_obj *i) : o{i} {}

  protected:
    void release() { o = nullptr; }

  public:
    obj() : o{create_obj()} {}
    ~obj() {
        if (o)
            del_obj(o);
    }

    obj(obj &&o) {
        using std::swap;
        swap(this->o, o.o);
    }

    obj &operator=(obj &&o) {
        using std::swap;
        swap(this->o, o.o);
        return *this;
    }

    static auto get_current() {
        return non_owning_wrapper<obj>(::get_current());
    }
};

template <typename T> struct store {
    T val;
    store(T value) : val{value} {}

    void operator()() { val(); }
};

struct queue {
    int count;
    int element_size;
    uint8_t buffer[];
};

struct S {
    int &ref;
};
#endif

class demo_screen : public lvgl::screen,
                    public lvgl::click_handler,
                    public lvgl::event_handler {
  public:
    lvgl::button btn{this};
    lvgl::label lbl{&btn};

    lvgl::checkbox chkbox{this};
    lvgl::lv_switch sw{this};
    lvgl::spinbox sb{this};

    demo_screen() : lvgl::screen{} {

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

        chkbox.set_text("enable foo");
        this->chkbox.set_pos(40, 500);

        sw.set_pos(40, 550);

        // btn.add_click_handler(this);
        btn.add_event_handler(this, lvgl::event::LV_EVENT_CLICKED);
    }

    void on_event(object &sender, lvgl::event ev) override {
        printf("event %d\n", static_cast<int>(ev));
        if (sender == btn) {
            printf("button click\n");
            lbl.set_text("click");
        }
    }

    void on_object_clicked(object &sender) override {
        //
        if (sender == btn) {
            printf("button\n");
            lbl.set_text("click");
        }
        printf("click\n");
    }
};

int main() {

    // lv_init();
    // init_lvgl_hal();

    lvgl::init();

    lvgl::drivers::static_buffer<800, 600> disp_buffer;
    dummy_display_driver<800, 600> disp_driver{disp_buffer};

    auto disp = disp_driver.get_display();

    lvgl::theme theme{lv_palette_main(LV_PALETTE_BLUE),
                      lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT};

    disp.apply_theme(theme);

    lv_obj_add_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);

    demo_screen scr;
    lvgl::screen::load(scr, lvgl::screen::load_anim::none, 1000);

#if 0
    {
        auto top = disp.get_top_layer();

        auto top_label = new lvgl::label{&top};
        top_label->set_text("I'm on top");
        top_label->align(lvgl::alignment::center);
        top_label->move_background();

        // lvgl::label top_label{&top};
        // top_label.set_text("I'm on top");
    }

    //{
    my_screen scr;
    lvgl::screen::load(scr, lvgl::screen::load_anim::none, 1000);

    lvgl::animation anim{
        [&scr](int32_t val) { scr.meter.set_indicator_value(scr.indic, val); }};

    anim.set_range(0, 100);
    anim.set_time(5000);
    anim.set_repeat(lvgl::animation::repeat_indef);

    anim.set_playback_delay(1000);
    anim.set_playback_time(1000);

    anim.set_repeat_delay(1000);
    anim.start();
    //}
#endif

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