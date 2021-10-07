#pragma once

#include "lvgl.h"
#include <functional>
#include <utility>

namespace lvgl {

class timer {
    lv_timer_t *_timer;

    static void callback(lv_timer_t *tmr) {
        auto self = reinterpret_cast<timer *>(tmr->user_data);
        if (tmr->repeat_count == 0) {
            // if repeat_count reaches 0 -> timer gets deleted
            self->_timer = nullptr;
        }
    }

  public:
    timer() : _timer{lv_timer_create_basic()} {}

    ~timer() { lv_timer_del(_timer); }
};

static inline void init() { lv_init(); }

enum class state {
    def = 0,
    checked = 1,
    focused = 2,
    focus_key = 4,
    edited = 8,
    hovered = 16,
    pressed = 32,
    scrolled = 64,
    disabled = 128,
    user1 = 0x1000,
    user2 = 0x2000,
    user3 = 0x4000,
    user4 = 0x8000
};

class style {
    friend class object;
    lv_style_t _obj;

  public:
    struct prop_value {
        lv_style_value_t value;

        prop_value(lv_color_t color) : value{.color = color} {}

        template <typename T,
                  typename = std::enable_if_t<std::is_integral_v<T>>>
        prop_value(T val)
            : value{.num = static_cast<decltype(lv_style_value_t::num)>(val)} {}

        template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>,
                  typename = void>
        prop_value(T val) : value{.ptr = val} {}
    };

    using property = lv_style_prop_t;
    // using prop_value = lv_style_value_t;

    style() : _obj{} { lv_style_init(&_obj); }

    void set_property(property prop, prop_value val) {
        lv_style_set_prop(&_obj, prop, val.value);
    }

    void remove_property(property prop) { lv_style_remove_prop(&_obj, prop); }

    void set_bg_color(lv_color_t color) { lv_style_set_bg_color(&_obj, color); }

    void set_width(lv_coord_t width) { lv_style_set_width(&_obj, width); }

    void set_height(lv_coord_t height) { lv_style_set_height(&_obj, height); }

    void reset() { lv_style_reset(&_obj); }
};

struct coord {
    lv_coord_t x;
    lv_coord_t y;
};

struct size {
    lv_coord_t width;
    lv_coord_t height;
};

enum class alignment {
    top_left = LV_ALIGN_TOP_LEFT,
    top_mid = LV_ALIGN_TOP_MID,
    top_right = LV_ALIGN_TOP_RIGHT,
    left_mid = LV_ALIGN_LEFT_MID,
    center = LV_ALIGN_CENTER,
    right_mid = LV_ALIGN_RIGHT_MID,
    bottom_left = LV_ALIGN_BOTTOM_LEFT,
    bottom_mid = LV_ALIGN_BOTTOM_MID,
    bottom_right = LV_ALIGN_BOTTOM_RIGHT,
};

class object {
    lv_obj_t *_obj;

  protected:
    object(lv_obj_t *obj) : _obj{obj} {}

    auto get_object() { return _obj; }

    auto get_object() const { return _obj; }

    //~object() { lv_obj_del(_obj); }

  public:
    template <typename CTOR, typename... Args>
    object(CTOR ctor, object *parent, Args... args)
        : _obj{ctor(parent ? parent->_obj : nullptr,
                    std::forward<Args>(args)...)} {}

    void set_width(lv_coord_t w) { lv_obj_set_width(_obj, w); }
    void set_height(lv_coord_t h) { lv_obj_set_height(_obj, h); }
    void set_size(lvgl::size s) { lv_obj_set_size(_obj, s.width, s.height); }
    void set_size(lv_coord_t width, lv_coord_t height) {
        lv_obj_set_size(_obj, width, height);
    }

    void set_x(lv_coord_t x) { lv_obj_set_x(_obj, x); }
    void set_y(lv_coord_t y) { lv_obj_set_y(_obj, y); }
    void set_pos(int x, int y) { lv_obj_set_pos(_obj, x, y); }
    void set_pos(coord pos) { lv_obj_set_pos(_obj, pos.x, pos.y); }

    lv_coord_t get_x() const { return lv_obj_get_x(_obj); }
    lv_coord_t get_y() const { return lv_obj_get_y(_obj); }
    coord get_pos() const { return {get_x(), get_y()}; }

    lv_coord_t get_width() const { return lv_obj_get_width(_obj); }
    lv_coord_t get_height() const { return lv_obj_get_height(_obj); }
    size get_size() const { return {get_width(), get_height()}; }

    void align(alignment a) {
        lv_obj_set_align(_obj, static_cast<lv_align_t>(a));
    }

    void align(alignment a, coord pos) {
        lv_obj_align(_obj, static_cast<lv_align_t>(a), pos.x, pos.y);
    }

    void add_style(style &s, state st = state::def) {
        lv_obj_add_style(_obj, &s._obj, static_cast<lv_style_selector_t>(st));
    }
};

class screen : public object {

    screen(lv_obj_t *scr) : object{scr} {}

  public:
    screen() : object{lv_obj_create, nullptr} {}

    static screen get_current() { return screen{lv_scr_act()}; }

    static void load(screen &s) { lv_scr_load(s.get_object()); }
};

class button : public object {

  public:
    button(object *parent) : object{lv_btn_create, parent} {}
};

class label : public object {
  public:
    label(object *parent) : object{lv_label_create, parent} {}

    void set_text(const char *txt) { lv_label_set_text(get_object(), txt); }
};

class bar : public object {
  public:
    bar(object *parent) : object{lv_bar_create, parent} {}

    auto get_value() const { lv_bar_get_value(get_object()); }

    void set_value(int32_t value) {
        lv_bar_set_value(get_object(), value, LV_ANIM_OFF);
    }
};

class meter : public object {
  public:
    class scale {
        friend class meter;
        lv_meter_scale_t *_obj;
        scale(lv_meter_scale_t *obj) : _obj{obj} {}
    };

    class indicator {
        friend class meter;
        lv_meter_indicator_t *_obj;

        indicator(lv_meter_indicator_t *obj) : _obj{obj} {}

      public:
        indicator() : _obj{nullptr} {}
    };

    meter(object *parent) : object{lv_meter_create, parent} {}

    scale add_scale() { return scale{lv_meter_add_scale(this->get_object())}; }

    indicator add_indicator_arc(scale s, lv_color_t color) {
        return indicator{lv_meter_add_arc(get_object(), s._obj, 3, color, 0)};
    }

    indicator add_indicator_lines(scale s, lv_color_t color, lv_color_t color2,
                                  bool local, int16_t mod) {
        return indicator{lv_meter_add_scale_lines(get_object(), s._obj, color,
                                                  color2, local, mod)};
    }

    indicator add_indicator_needle(scale s, uint16_t width, lv_color_t color,
                                   int16_t mod) {
        return indicator{
            lv_meter_add_needle_line(get_object(), s._obj, width, color, mod)};
    }

    void set_indicator_start_value(indicator i, int32_t start) {
        lv_meter_set_indicator_start_value(get_object(), i._obj, start);
    }

    void set_indicator_end_value(indicator i, int32_t end) {
        lv_meter_set_indicator_end_value(get_object(), i._obj, end);
    }

    void set_indicator_value(indicator i, int32_t value) {
        lv_meter_set_indicator_value(get_object(), i._obj, value);
    }

    void set_scale_ticks(scale s) {
        lv_meter_set_scale_ticks(get_object(), s._obj, 41, 2, 10,
                                 lv_palette_main(LV_PALETTE_GREY));
    }

    void set_scale_major_ticks(scale s) {
        lv_meter_set_scale_major_ticks(get_object(), s._obj, 8, 4, 15,
                                       lv_color_black(), 10);
    }
};

class led : public object {

  public:
    led(object *parent) : object{lv_led_create, parent} {}

    void set_color(lv_color_t color) { lv_led_set_color(get_object(), color); }

    void set_brightness(uint8_t brightness) {
        lv_led_set_brightness(get_object(), brightness);
    }

    void on() { lv_led_on(get_object()); }

    void off() { lv_led_off(get_object()); }

    void toggle() { lv_led_toggle(get_object()); }
};

template <size_t N> class msg_box_base {
    std::array<const char *, N + 1> _button_texts;

    template <size_t I>
    static constexpr std::array<const char *, I + 1>
    add_null(std::array<const char *, I> inp) {
        std::array<const char *, I + 1> out;
        for (auto i = 0; i < I; i++) {
            out[i] = inp[i];
        }

        out[I] = "";
        return out;
    }

  protected:
    msg_box_base(const std::array<const char *, N> &texts)
        : _button_texts{add_null(texts)} {}

    const char **get_button_texts() { return &_button_texts[0]; }
};

template <size_t N> class msg_box : private msg_box_base<N>, public object {

    // std::array<const char *, N + 1> _button_texts;

  public:
    template <size_t I>
    static constexpr std::array<const char *, I + 1>
    add_null(std::array<const char *, I> inp) {
        std::array<const char *, I + 1> out;
        for (auto i = 0; i < I; i++) {
            out[i] = inp[i];
        }

        out[I] = "";
        return out; // std::array<const char *, N + 1>{inp, nullptr};
    }

    // msg_box(object *parent, const char *title, const char *txt,
    //        const char *btn_texts[], bool add_close)
    //    : object{lv_msgbox_create, parent, title, txt, btn_texts, add_close}
    //    {}

#if 1
    // template <size_t I>
    msg_box(object *parent, const char *title, const char *txt,
            const std::array<const char *, N> &btns, bool add_close)
        : msg_box_base<N>{btns}, object{lv_msgbox_create,
                                        parent,
                                        title,
                                        txt,
                                        msg_box_base<N>::get_button_texts(),
                                        add_close} {}
#else
    template <size_t N>
    msg_box(object *parent, const char *title, const char *txt,
            std::array<const char *, N> &btns, bool add_close)
        : msg_box{parent, title, txt, &btns[0], add_close} {}
#endif
};

class checkbox : public object {
  public:
    checkbox(object *parent) : object{lv_checkbox_create, parent} {}

    void set_text(const char *text) {
        lv_checkbox_set_text(get_object(), text);
    }

    auto get_text() { lv_checkbox_get_text(get_object()); }

    bool is_checked() const {
        return lv_obj_get_state(get_object()) == LV_STATE_CHECKED;
    }

    void set_checked(bool checked = true) {
        if (checked)
            lv_obj_add_state(get_object(), LV_STATE_CHECKED);
        else
            lv_obj_clear_state(get_object(), LV_STATE_CHECKED);
    }
};

class lv_switch : public object {

  public:
    lv_switch(object *parent) : object{lv_switch_create, parent} {}

    bool is_on() const {
        return lv_obj_get_state(get_object()) == LV_STATE_CHECKED;
    }

    void set_on() { lv_obj_add_state(get_object(), LV_STATE_CHECKED); }
    void set_off() { lv_obj_clear_state(get_object(), LV_STATE_CHECKED); }
};

class text_field : public object {

  public:
    text_field(object *parent) : object{lv_textarea_create, parent} {}

    void add_char(char c) { lv_textarea_add_char(get_object(), c); }

    auto get_placeholder_text() const {
        return lv_textarea_get_placeholder_text(get_object());
    }

    void set_placeholder_text(const char *txt) {
        lv_textarea_set_placeholder_text(get_object(), txt);
    }

    auto get_text() const { return lv_textarea_get_text(get_object()); }

    void set_text(const char *text) {
        lv_textarea_set_text(get_object(), text);
    }
};

class calendar : public object {
  public:
    using date = lv_calendar_date_t;

    calendar(object *parent) : object{lv_calendar_create, parent} {}

    auto get_date() const {
        auto d = lv_calendar_get_showed_date(get_object());
        return date{*d};
    }

    void set_day_names(std::array<const char *, 7> &names) {
        lv_calendar_set_day_names(get_object(), &names[0]);
    }
};

class animation {
    using callback_type = std::function<void(int32_t)>;

    lv_anim_t _obj;
    callback_type cb;

  public:
    animation(std::function<void(int32_t)> fn) : cb{fn} {
        lv_anim_init(&_obj);

        lv_anim_set_exec_cb(&_obj, [](void *ctx, int32_t val) {
            auto a = reinterpret_cast<animation::callback_type *>(ctx);
            (*a)(val);
        });

        // WARNING: do not set 'this' as variable! if the var has the same
        // address as lv_anim_t, the behaviour is somehow different (see
        // lv_anim.c:97)
        lv_anim_set_var(&_obj, &cb);
    }

    void set_range(int32_t from, int32_t to) {
        lv_anim_set_values(&_obj, from, to);
    }

    void set_time(uint32_t ms) { lv_anim_set_time(&_obj, ms); }

    void set_playback_time(uint32_t ms) {
        lv_anim_set_playback_time(&_obj, ms);
    }

    void set_playback_delay(uint32_t ms) {
        lv_anim_set_playback_delay(&_obj, ms);
    }

    void set_repeat_delay(uint32_t delay) {
        lv_anim_set_repeat_delay(&_obj, delay);
    }

    void start() { lv_anim_start(&_obj); }

    void stop() {}

    void set_repeat(uint16_t count) { lv_anim_set_repeat_count(&_obj, count); }
};

} // namespace lvgl