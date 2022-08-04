#pragma once

#include "lvgl.h"
#include <functional>
#include <utility>

namespace lvgl {

enum class event : int {
    LV_EVENT_ALL = 0,

    /** Input device events*/
    LV_EVENT_PRESSED,    /**< The object has been pressed*/
    LV_EVENT_PRESSING,   /**< The object is being pressed (called continuously
                            while pressing)*/
    LV_EVENT_PRESS_LOST, /**< The object is still being pressed but slid
                            cursor/finger off of the object */
    LV_EVENT_SHORT_CLICKED, /**< The object was pressed for a short period of
                               time, then released it. Not called if scrolled.*/
    LV_EVENT_LONG_PRESSED,  /**< Object has been pressed for at least
                               `long_press_time`.  Not called if scrolled.*/
    LV_EVENT_LONG_PRESSED_REPEAT, /**< Called after `long_press_time` in every
                                     `long_press_repeat_time` ms.  Not called if
                                     scrolled.*/
    LV_EVENT_CLICKED, /**< Called on release if not scrolled (regardless to long
                         press)*/
    LV_EVENT_RELEASED,     /**< Called in every cases when the object has been
                              released*/
    LV_EVENT_SCROLL_BEGIN, /**< Scrolling begins*/
    LV_EVENT_SCROLL_END,   /**< Scrolling ends*/
    LV_EVENT_SCROLL,       /**< Scrolling*/
    LV_EVENT_GESTURE,      /**< A gesture is detected. Get the gesture with
                              `lv_indev_get_gesture_dir(lv_indev_get_act());` */
    LV_EVENT_KEY,          /**< A key is sent to the object. Get the key with
                              `lv_indev_get_key(lv_indev_get_act());`*/
    LV_EVENT_FOCUSED,      /**< The object is focused*/
    LV_EVENT_DEFOCUSED,    /**< The object is defocused*/
    LV_EVENT_LEAVE,        /**< The object is defocused but still selected*/
    LV_EVENT_HIT_TEST,     /**< Perform advanced hit-testing*/

    /** Drawing events*/
    LV_EVENT_COVER_CHECK, /**< Check if the object fully covers an area. The
                             event parameter is `lv_cover_check_info_t *`.*/
    LV_EVENT_REFR_EXT_DRAW_SIZE, /**< Get the required extra draw area around
                                    the object (e.g. for shadow). The event
                                    parameter is `lv_coord_t *` to store the
                                    size.*/
    LV_EVENT_DRAW_MAIN_BEGIN,    /**< Starting the main drawing phase*/
    LV_EVENT_DRAW_MAIN,          /**< Perform the main drawing*/
    LV_EVENT_DRAW_MAIN_END,      /**< Finishing the main drawing phase*/
    LV_EVENT_DRAW_POST_BEGIN,    /**< Starting the post draw phase (when all
                                    children are drawn)*/
    LV_EVENT_DRAW_POST, /**< Perform the post draw phase (when all children are
                           drawn)*/
    LV_EVENT_DRAW_POST_END,   /**< Finishing the post draw phase (when all
                                 children are drawn)*/
    LV_EVENT_DRAW_PART_BEGIN, /**< Starting to draw a part. The event parameter
                                 is `lv_obj_draw_dsc_t *`. */
    LV_EVENT_DRAW_PART_END,   /**< Finishing to draw a part. The event parameter
                                 is `lv_obj_draw_dsc_t *`. */

    /** Special events*/
    LV_EVENT_VALUE_CHANGED, /**< The object's value has changed (i.e. slider
                               moved)*/
    LV_EVENT_INSERT,  /**< A text is inserted to the object. The event data is
                         `char *` being inserted.*/
    LV_EVENT_REFRESH, /**< Notify the object to refresh something on it (for the
                         user)*/
    LV_EVENT_READY,   /**< A process has finished*/
    LV_EVENT_CANCEL,  /**< A process has been cancelled */

    /** Other events*/
    LV_EVENT_DELETE,         /**< Object is being deleted*/
    LV_EVENT_CHILD_CHANGED,  /**< Child was removed/added*/
    LV_EVENT_SIZE_CHANGED,   /**< Object coordinates/size have changed*/
    LV_EVENT_STYLE_CHANGED,  /**< Object's style has changed*/
    LV_EVENT_LAYOUT_CHANGED, /**< The children position has changed due to a
                                layout recalculation*/
    LV_EVENT_GET_SELF_SIZE,  /**< Get the internal size of a widget*/

    _LV_EVENT_LAST /** Number of default events*/
};

class object;
class event_handler {
  public:
    virtual void on_event(object &sender, event ev) = 0;
};

using color_t = lv_color_t;

using coord_t = lv_coord_t;

struct area_t {
    coord_t x1;
    coord_t y1;
    coord_t x2;
    coord_t y2;

    auto get_width() const { return (coord_t)(x2 - x1 + 1); }

    auto get_height() const { return (coord_t)(y2 - y1 + 1); }
};

static inline constexpr color_t make_color(uint8_t r, uint8_t g, uint8_t b) {
    return LV_COLOR_MAKE(r, g, b);
}

namespace drivers {
class display;
}

class theme {
    friend class drivers::display;
    lv_theme_t *_theme;

  public:
    theme(lv_color_t color_primary, lv_color_t color_secondary, bool dark,
          const lv_font_t *font)
        : _theme{lv_theme_default_init(nullptr, color_primary, color_secondary,
                                       dark, font)} {}
};

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

enum class flag : lv_obj_flag_t {

    hidden = LV_OBJ_FLAG_HIDDEN,
    clickable = LV_OBJ_FLAG_CLICKABLE,
    focusable = LV_OBJ_FLAG_CLICK_FOCUSABLE,
    checkable = LV_OBJ_FLAG_CHECKABLE,
    scrollable = LV_OBJ_FLAG_SCROLLABLE,
    scroll_elastic = LV_OBJ_FLAG_SCROLL_ELASTIC,
    scroll_momentum = LV_OBJ_FLAG_SCROLL_MOMENTUM,
    scroll_one = LV_OBJ_FLAG_SCROLL_ONE,
    scroll_chain = LV_OBJ_FLAG_SCROLL_CHAIN,
    scroll_on_focus = LV_OBJ_FLAG_SCROLL_ON_FOCUS,
    snappable = LV_OBJ_FLAG_SNAPPABLE,
    press_lock = LV_OBJ_FLAG_PRESS_LOCK,
    event_bubble = LV_OBJ_FLAG_EVENT_BUBBLE,
    gesture_bubble = LV_OBJ_FLAG_GESTURE_BUBBLE,
    adv_hittest = LV_OBJ_FLAG_ADV_HITTEST,
    ignore_layout = LV_OBJ_FLAG_IGNORE_LAYOUT,
    floating = LV_OBJ_FLAG_FLOATING,

    layout_1 = LV_OBJ_FLAG_LAYOUT_1,
    layout_2 = LV_OBJ_FLAG_LAYOUT_2,

    widget_1 = LV_OBJ_FLAG_WIDGET_1,
    widget_2 = LV_OBJ_FLAG_WIDGET_2,

    user_1 = LV_OBJ_FLAG_USER_1,
    user_2 = LV_OBJ_FLAG_USER_2,
    user_3 = LV_OBJ_FLAG_USER_3,
    user_4 = LV_OBJ_FLAG_USER_4,

}; // namespace lvgl

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
    coord_t x;
    coord_t y;
};

struct size {
    coord_t width;
    coord_t height;
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

template <typename T> class owning_wrapper : public T {
  public:
    template <typename... Args>
    owning_wrapper(Args... args) : T{std::forward<Args>(args)...} {}
};

/**
 * @brief base-class for all lvgl-gui-objects.
 *
 */
class object {
    lv_obj_t *_obj;

  protected:
    friend class non_owning_wrapper<object>;

    object(lv_obj_t *obj) : _obj{obj} {}

    auto get_object() { return _obj; }

    auto get_object() const { return _obj; }

    void release() { _obj = nullptr; }

    //~object() { lv_obj_del(_obj); }

  public:
    template <typename CTOR, typename... Args>
    object(CTOR ctor, object *parent, Args... args)
        : _obj{ctor(parent ? parent->_obj : nullptr,
                    std::forward<Args>(args)...)} {}

    object(object *parent)
        : _obj{lv_obj_create(parent ? parent->_obj : nullptr)} {}

    ~object() {
        // if constexpr (Owning)
        if (_obj)
            lv_obj_del(_obj);
    }

    bool operator==(const object &other) const { return _obj == other._obj; }

    auto get_parent() const {
        return non_owning_wrapper<object>(lv_obj_get_parent(_obj));
    }

    void add_event_handler(event_handler *handler, event filter) {
        lv_obj_add_event_cb(
            _obj,
            [](lv_event_t *ev) {
                auto h = reinterpret_cast<event_handler *>(ev->user_data);
                if (h) {
                    auto sender = non_owning_wrapper<object>(ev->target);
                    h->on_event(sender, static_cast<event>(ev->code));
                }
            },
            static_cast<lv_event_code_t>(filter), handler);
    }

    /**
     * @brief Defer deletion of the object
     *
     * After calling delete_later(), the object must not be used any longer by
     * application-code, i.e. the user is not allowed to call any method of the
     * object.
     *
     */
    void delete_later() {
        lv_obj_del_async(_obj);
        release();
    }

    void set_width(coord_t w) { lv_obj_set_width(_obj, w); }
    void set_height(coord_t h) { lv_obj_set_height(_obj, h); }
    void set_size(lvgl::size s) { lv_obj_set_size(_obj, s.width, s.height); }
    void set_size(coord_t width, lv_coord_t height) {
        lv_obj_set_size(_obj, width, height);
    }

    void set_x(coord_t x) { lv_obj_set_x(_obj, x); }
    void set_y(coord_t y) { lv_obj_set_y(_obj, y); }
    void set_pos(int x, int y) { lv_obj_set_pos(_obj, x, y); }
    void set_pos(coord pos) { lv_obj_set_pos(_obj, pos.x, pos.y); }

    coord_t get_x() const { return lv_obj_get_x(_obj); }
    coord_t get_y() const { return lv_obj_get_y(_obj); }
    coord get_pos() const { return {get_x(), get_y()}; }

    coord_t get_width() const { return lv_obj_get_width(_obj); }
    coord_t get_height() const { return lv_obj_get_height(_obj); }
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

    void move_foreground() { lv_obj_move_foreground(_obj); }

    void move_background() { lv_obj_move_background(_obj); }

    // void move_up() { lv_obj_move_up(_obj); }
    // void move_down() { lv_obj_move_down(_obj); }
};

class screen : public object {

  protected:
    screen(lv_obj_t *scr) : object{scr} {}

  public:
    enum class load_anim {
        none = LV_SCR_LOAD_ANIM_NONE,
        over_left = LV_SCR_LOAD_ANIM_OVER_LEFT,
        over_right = LV_SCR_LOAD_ANIM_OVER_RIGHT,
        over_top = LV_SCR_LOAD_ANIM_OVER_TOP,
        over_bottom = LV_SCR_LOAD_ANIM_OVER_BOTTOM,
        move_left = LV_SCR_LOAD_ANIM_MOVE_LEFT,
        move_right = LV_SCR_LOAD_ANIM_MOVE_RIGHT,
        move_top = LV_SCR_LOAD_ANIM_MOVE_TOP,
        move_bottom = LV_SCR_LOAD_ANIM_MOVE_BOTTOM,
        fade_in = LV_SCR_LOAD_ANIM_FADE_ON
    };

    screen() : object{lv_obj_create, nullptr} {}

    static auto get_current() {
        return non_owning_wrapper<screen>{lv_scr_act()};
    }

    static void load(screen &s) { lv_scr_load(s.get_object()); }

    static void load(screen &s, load_anim anim, uint32_t time,
                     uint32_t delay = 0) {
        lv_scr_load_anim(s.get_object(), static_cast<lv_scr_load_anim_t>(anim),
                         time, delay, false);
    }
};

class click_handler {
  public:
    virtual void on_object_clicked(object &sender) = 0;
};

class button : public object {

  public:
    button(object *parent) : object{lv_btn_create, parent} {}

    void add_click_handler(click_handler *handler) {
        lv_obj_add_event_cb(
            get_object(),
            [](lv_event_t *e) {
                auto s = non_owning_wrapper<object>(e->target);
                if (e->user_data) {
                    auto handler =
                        reinterpret_cast<click_handler *>(e->user_data);
                    handler->on_object_clicked(s);
                }
            },
            LV_EVENT_CLICKED, handler);
    }
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

class slider : public object {
  public:
    slider(object *parent) : object{lv_slider_create, parent} {}

    auto get_value() const { lv_slider_get_value(get_object()); }

    void set_value(int32_t value) {
        lv_slider_set_value(get_object(), value, LV_ANIM_OFF);
    }
};

class spinbox : public object {
  public:
    spinbox(object *parent) : object{lv_spinbox_create, parent} {}

    auto get_value() const { lv_spinbox_get_value(get_object()); }

    void set_value(int32_t value) { lv_spinbox_set_value(get_object(), value); }

    void increment() { lv_spinbox_increment(get_object()); }

    void decrement() { lv_spinbox_decrement(get_object()); }
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
    //    : object{lv_msgbox_create, parent, title, txt, btn_texts,
    //    add_close}
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
    static constexpr uint16_t repeat_indef = 0xffff;

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

    ~animation() { lv_anim_del(_obj.var, _obj.exec_cb); }

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