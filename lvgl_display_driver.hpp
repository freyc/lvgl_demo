#pragma once

#include <array>

namespace lvgl::drivers {

template <typename Buffer> class draw_buffer {
  protected:
    friend class display_driver_base;
    lv_disp_draw_buf_t _draw_buffer;

    auto &get() { return *static_cast<Buffer *>(this); }

  public:
    draw_buffer() {
        lv_disp_draw_buf_init(&_draw_buffer, get().get_buffer(0),
                              get().get_buffer(1), get().get_buffer_size());
    }

    auto width() const { return static_cast<const Buffer *>(this)->width(); }

    auto height() const { return static_cast<const Buffer *>(this)->height(); }
};

template <lv_coord_t Width, lv_coord_t Height>
class static_buffer
    : public lvgl::drivers::draw_buffer<static_buffer<Width, Height>> {

  private:
    std::array<lv_color_t, Width * Height> _buffer;

  public:
    auto width() const { return Width; }

    auto height() const { return Height; }

    lv_color_t *get_buffer(int idx) {
        if (idx == 0)
            return &_buffer[0];

        return nullptr;
    }

    auto get_buffer_size() const { return Width * Height; }
};

class display {
    lv_disp_t *_disp;

    display(lv_disp_t *disp) : _disp{disp} {}
};

template <typename T>
concept DisplayDriver = requires(T &o, const lv_area_t *area,
                                 lv_color_t *color_p) {
    {o.flush_display(area, color_p)};
};

class display_driver_base {
  protected:
    lv_disp_drv_t _driver;
    lv_disp_t *_disp;

    template <typename Buffer>
    display_driver_base(draw_buffer<Buffer> &buffer) {
        lv_disp_drv_init(&_driver);
        _driver.draw_buf = &buffer._draw_buffer;
    }
};

template <typename T> class display_driver : protected display_driver_base {

    auto &get() { return *static_cast<T *>(this); }

    static void flush(lv_disp_drv_t *disp_drv, const lv_area_t *area,
                      lv_color_t *color_p) {
#if LV_USE_USER_DATA
        auto self = reinterpret_cast<display_driver<T> *>(disp_drv->user_data);
#else
        static_assert(std::is_standard_layout_v<display_driver<T>>,
                      "display_driver<T> must be a standard-layout type");
        auto self = reinterpret_cast<display_driver<T> *>(disp_drv);
#endif
        self->get().flush_display(area, color_p);
    }

  protected:
    void flush_ready() { lv_disp_flush_ready(&_driver); }

    bool flush_is_last() { return lv_disp_flush_is_last(&_driver); }

  public:
    template <typename Buffer>
    display_driver(draw_buffer<Buffer> &buffer) : display_driver_base{buffer} {

        _driver.flush_cb = display_driver<T>::flush; // monitor_flush;
        _driver.hor_res = get().get_x_res();         // SDL_HOR_RES;
        _driver.ver_res = get().get_y_res();         // SDL_VER_RES;
        _driver.antialiasing = 1;
        _driver.full_refresh = 0;

#if LV_USE_USER_DATA
        _driver.user_data = this;
#endif

        _disp = lv_disp_drv_register(&_driver);
    }
};

} // namespace lvgl::drivers