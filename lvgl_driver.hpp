#pragma once

#include "lvgl.h"

namespace lvgl::drivers {

class input_driver_base {
  protected:
    friend class group;
    lv_indev_drv_t _driver;
    lv_indev_t *_dev;
};

template <typename T> class input_pointer_driver : public input_driver_base {

    auto &get() { return *static_cast<T *>(this); }

    static void read_callback(lv_indev_drv_t *indev_drv,
                              lv_indev_data_t *data) {

#if LV_USE_USER_DATA
        auto drv =
            reinterpret_cast<input_pointer_driver<T> *>(indev_drv->user_data);
#else
        static_assert(std::is_standard_layout_v<input_pointer_driver<T>>, "");
        auto drv = reinterpret_cast<input_pointer_driver<T> *>(indev_drv);
#endif
        drv->get().get_pointer_state(data->point.x, data->point.y, data->state);
    }

  public:
    input_pointer_driver() {
        lv_indev_drv_init(&_driver);
        _driver.type = LV_INDEV_TYPE_POINTER;
        _driver.read_cb = input_pointer_driver::read_callback;
#if LV_USE_USER_DATA
        _driver.user_data = this;
#endif

        _dev = lv_indev_drv_register(&_driver);
    }
};

template <typename T> class input_keyboard_driver : public input_driver_base {

    auto &get() { return *static_cast<T *>(this); }

    static void read_callback(lv_indev_drv_t *indev_drv,
                              lv_indev_data_t *data) {

#if LV_USE_USER_DATA
        auto drv =
            reinterpret_cast<input_keyboard_driver<T> *>(indev_drv->user_data);
#else
        static_assert(std::is_standard_layout_v<input_keyboard_driver<T>>, "");
        auto drv = reinterpret_cast<input_keyboard_driver<T> *>(indev_drv);
#endif
        data->continue_reading =
            drv->get().get_keyboard_state(data->key, data->state);
    }

  public:
    input_keyboard_driver() {
        lv_indev_drv_init(&_driver);
        _driver.type = LV_INDEV_TYPE_KEYPAD;
        _driver.read_cb = input_keyboard_driver::read_callback;

#if LV_USE_USER_DATA
        _driver.user_data = this;
#endif

        _dev = lv_indev_drv_register(&_driver);
    }
};

class group {
    lv_group_t *_group;

  public:
    group(const group &) = delete;
    group(group &&) = delete;
    auto operator=(const group &) = delete;
    auto operator=(group &&) = delete;

    group(bool default_group = true) : _group{lv_group_create()} {
        if (default_group) {
            lv_group_set_default(_group);
        }
    }

    ~group() { lv_group_del(_group); }

    void add_driver(input_driver_base &drv) {
        lv_indev_set_group(drv._dev, _group);
    }
};
} // namespace lvgl::drivers