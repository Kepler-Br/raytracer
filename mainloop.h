#ifndef MAINLOOP_H
#define MAINLOOP_H

typedef struct s_mainloop
{
    void (*run)(struct s_mainloop *this);
    void (*prepare)(struct s_mainloop *this);
} t_mainloop;

t_mainloop construct_mainloop();

#endif // MAINLOOP_H
