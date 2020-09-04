TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        camera.c \
        check_timer.c \
        color.c \
        framebuffer.c \
        linear_alg.c \
        main.c \
        mainloop.c \
        ray.c \
        render_mask.c \
        scene.c \
        sdl_instance.c \
        shape.c

HEADERS += \
    camera.h \
    check_timer.h \
    color.h \
    framebuffer.h \
    linear_alg.h \
    mainloop.h \
    ray.h \
    render_mask.h \
    scene.h \
    sdl_instance.h \
    shape.h

LIBS += -lSDL2
