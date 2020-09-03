TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        camera.c \
        check_timer.c \
        framebuffer.c \
        linear_alg.c \
        main.c \
        mainloop.c \
        ray.c \
        sdl_instance.c \
        shape.c

HEADERS += \
    camera.h \
    check_timer.h \
    framebuffer.h \
    linear_alg.h \
    mainloop.h \
    ray.h \
    sdl_instance.h \
    shape.h

LIBS += -lSDL2
