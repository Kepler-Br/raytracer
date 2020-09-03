TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        check_timer.c \
        framebuffer.c \
        linear_alg.c \
        main.c \
        mainloop.c \
        sdl_instance.c

HEADERS += \
    check_timer.h \
    framebuffer.h \
    linear_alg.h \
    mainloop.h \
    sdl_instance.h

LIBS += -lSDL2
