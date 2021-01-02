#include "../win/win.h" 
#include "../colors/colors.h"

uint32_t const win_w = 500,
               win_h = 500;
char const *win_s = "plot";
float const mouse_sens = 0.1;

#define STYLE_METHOD_FLAG_POINTS 1
#define STYLE_METHOD_FLAG_LINES 2

typedef struct {
    uint32_t bg_color;

    uint8_t method_flag;

    uint32_t pt_w, pt_h;
    uint32_t pt_hw, pt_hh;
    uint32_t pt_color;

    uint32_t line_w;
    uint32_t line_color;

    bool grid_;
    float grid_step;
    uint32_t grid_line_w, grid_line_hw;
    uint32_t grid_color;
    uint32_t grid_origin_line_w, grid_origin_line_hw;
    uint32_t grid_origin_color;
} plot_style;

typedef struct {
    uint32_t n;
    float *_p;

    float x_min, x_max;
    float y_min, y_max;
    float w, h;
} plot_data;

typedef struct {
    uint32_t n;
    float *_p;

    float x_min, x_max;
    float y_min, y_max;
    float w, h;

    plot_style style;
    plot_data data;
} plot_view;

bool plot_data_cons (plot_data *_data);
bool plot_data_decons (plot_data *_data);
bool plot_data_read (plot_data *_data, char const *file_s, uint32_t n, 
        uint8_t dim, bool binary_);
bool plot_style_cons (plot_style *_style);
bool plot_style_decons (plot_style *_style);
bool plot_view_cons (plot_view *_view);
bool plot_view_decons (plot_view *_view);
bool plot_view_alloc_projected (plot_view *_view);
void plot_view_to_data (plot_view *_view);
void plot_view_project_data (plot_view *_view);
void plot_view_pan (plot_view *_view, float pan_x, float pan_y);
void plot_view_scale (plot_view *_view, float scale_x, float scale_y);
void paint (win *_win, plot_view *_view);
void paint_grid (win *_win, plot_view *_view);
void paint_lines (win *_win, plot_view *_view);
void paint_points (win *_win, plot_view *_view);

int main (int argc, char **__argv) {
    if ((argc < 2) || (argc > 5)) {
        fprintf(stderr, "usage: %s infile nitems [dim [binary?]] \n", __argv[0]);
        return 1;
    }
 
    plot_view *_view = malloc(sizeof(plot_view));
    if (! plot_view_cons (_view))
        return 2;

    if (! plot_data_read (&_view->data,
                __argv[1], atoi(__argv[2]),
                (argc > 3) ? atoi(__argv[3]) : 1,
                (argc > 4) ? atoi(__argv[4]) : 0)) {
        dlog_error (_, "main: plot_data_read");
        return 3;
    }
    plot_view_alloc_projected (_view);
    plot_view_to_data (_view);
 
    win_run_fps = 0.0;
    if (! win_init ()) {
        dlog_error (_, "main: win_init");
        return 4;
    }

    win_conf *_win_conf = malloc(sizeof(win_conf));
    if (! win_conf_cons (_win_conf))
        return 5;

    _win_conf->w = win_w;
    _win_conf->h = win_h;
    _win_conf->_title_s = win_s;

    win *_win = malloc(sizeof(win));
    if (! win_cons (_win, _win_conf))
        return 6;

    if (! win_open (_win)) {
        dlog_error (_, "main: win_open");
        return 7;
    }
 
    paint (_win, _view);

    bool b0_ = 0, b1_ = 0, b2_ = 0;
    float xf_0, yf_0, xf_1, yf_1;
    while (win_tick ()) {
        if (_win->m_b0_) {
            if (! b0_) {
                b0_ = 1;
                xf_0 = _win->m_xf;
                yf_0 = _win->m_yf;
            }
            plot_view_pan (_view,
                -(_win->m_xf - xf_0) * _view->w * mouse_sens,
                (_win->m_yf - yf_0) * _view->h * mouse_sens);
            paint (_win, _view);
        }
        else {
            if (b0_)
                b0_ = 0;
        }
            
        if (_win->m_b1_) {
            if (! b1_) {
                b1_ = 1;
                xf_1 = _win->m_xf;
                (void)xf_1; // shut up the compiler
                yf_1 = _win->m_yf;
            }
            float s_y = 1.0 + ((_win->m_yf - yf_1) * mouse_sens);
            plot_view_scale (_view, s_y, s_y);
            paint (_win, _view);
        }
        else {
            if (b1_)
                b1_ = 0;
        }

        if (_win->m_b2_) {
            if (! b2_) {
                b2_ = 1;
                plot_view_to_data (_view);
                paint (_win, _view);
            }
        }
        else {
            if (b2_)
                b2_ = 0;
        }

        win_render (_win);
        SDL_Delay (33);
    }

    if (! plot_view_decons (_view))
        return 8;
    free(_view);

    win_deinit ();
    free(_win);

    if (! win_conf_decons (_win_conf))
        return 9;
    free(_win_conf);

    return 0;
}

bool plot_data_cons (plot_data *_data) {
    _data->n = 0;
    _data->_p = _;
    return true;
}

bool plot_data_decons (plot_data *_data) {
    if (_data->_p)
        free(_data->_p);
    return true;
}

bool plot_data_read (plot_data *_data, char const *file_s, uint32_t n, 
        uint8_t dim, bool binary_) {
    FILE *f_in = fopen(file_s, "r");
    if (! f_in) {
        dlog_error_s (_, "plot_data_read: fopen", file_s);
        return false;
    }

    _data->n = n;
    _data->_p = (float *)malloc(n * 2 * 4);

    uint8_t read_n;
    float *_f = _data->_p;
    for (uint32_t c = 0; c < n; c ++) {
        if (dim == 1) {
            *_f = (float)c;
            _f ++;
        }
        for (uint8_t dim_c = 0; dim_c < dim; dim_c ++) {
            if (binary_)
                read_n = fread(_f, 4, 1, f_in);
            else 
                read_n = fscanf(f_in, "%f", _f);
            if (read_n != 1) {
                dlog_error_u (_, "plot_data_read: out of data after", c);
                return false;
            }
            _f ++;
        }
    }
    fclose(f_in);

    _f = _data->_p;
    _data->x_min = _data->x_max = *_f;
    _data->y_min = _data->y_max = *(_f + 1);
    for (uint32_t c = 0; c < _data->n; c ++) {
        if (*_f < _data->x_min)
            _data->x_min = *_f;
        if (*_f > _data->x_max)
            _data->x_max = *_f;
        _f ++;

        if (*_f < _data->y_min)
            _data->y_min = *_f;
        if (*_f > _data->y_max)
            _data->y_max = *_f;
        _f ++;
    }

    _data->w = _data->x_max - _data->x_min;
    _data->h = _data->y_max - _data->y_min;
    return true;
}

bool plot_style_cons (plot_style *_style) {
    _style->bg_color = COLOR_gray20;

    _style->method_flag = STYLE_METHOD_FLAG_POINTS | STYLE_METHOD_FLAG_LINES;
    _style->pt_w = 3;
    _style->pt_h = 3;
    _style->pt_hw = _style->pt_w / 2;
    _style->pt_hh = _style->pt_h / 2;
    _style->pt_color = COLOR_yellow;
    _style->line_w = 1;
    _style->line_color = COLOR_cyan;

    _style->grid_ = true;
    _style->grid_step = 1.0;

    _style->grid_line_w = 1;
    _style->grid_line_hw = _style->grid_line_w / 2;
    _style->grid_color = COLOR_gray40;

    _style->grid_origin_line_w = 2;
    _style->grid_origin_line_hw = _style->grid_origin_line_w / 2;
    _style->grid_origin_color = COLOR_gray60;
    return true;
}

bool plot_style_decons (plot_style *_style) {
    return true;
}

bool plot_view_cons (plot_view *_view) {
    _view->n = 0;
    _view->_p = _;

    if (! plot_data_cons (&_view->data))
        return false;

    if (! plot_style_cons (&_view->style))
        return false;

    return true;
}

bool plot_view_decons (plot_view *_view) {
    if (_view->_p)
        free(_view->_p);

    if (! plot_data_decons (&_view->data))
        return false;

    if (! plot_style_decons (&_view->style))
        return false;

    return true;
}

bool plot_view_alloc_projected (plot_view *_view) {
    _view->n = _view->data.n;
    _view->_p = (float *)malloc(_view->data.n * 2 * 4);
    return (_view->_p != _);
}

void plot_view_to_data (plot_view *_view) {
    _view->x_min = _view->data.x_min - (_view->data.w / 10.0);
    _view->x_max = _view->data.x_max + (_view->data.w / 10.0);

    _view->y_min = _view->data.y_min - (_view->data.h / 10.0);
    _view->y_max = _view->data.y_max + (_view->data.h / 10.0);

    _view->w = _view->x_max - _view->x_min;
    _view->h = _view->y_max - _view->y_min;
    
    if (_view->w == 0.0) {
        _view->x_min -= 0.1;
        _view->x_max += 0.1;
        _view->w = 0.2;
    }

    if (_view->h == 0.0) {
        _view->y_min -= 0.1;
        _view->y_max += 0.1;
        _view->h = 0.2;
    }

    plot_view_project_data (_view);
}

void plot_view_project_data (plot_view *_view) {
    float *_p = _view->_p;
    float *_q = _view->data._p;
    for (uint32_t c = 0; c < _view->n; c ++) {
        *_p = (*_q - _view->x_min) / _view->w;
        _p ++;
        _q ++;

        *_p = (*_q - _view->y_min) / _view->h;
        _p ++;
        _q ++;
    }
}

void plot_view_pan (plot_view *_view, float pan_x, float pan_y) {
    _view->x_min += pan_x;
    _view->x_max += pan_x;

    _view->y_min += pan_y;
    _view->y_max += pan_y;

    plot_view_project_data (_view);
}

void plot_view_scale (plot_view *_view, float scale_x, float scale_y) {
    _view->x_min *= scale_x;
    _view->x_max *= scale_x;

    _view->y_min *= scale_y;
    _view->y_max *= scale_y;

    _view->w = _view->x_max - _view->x_min;
    _view->h = _view->y_max - _view->y_min;

    plot_view_project_data (_view);
}

void paint (win *_win, plot_view *_view) {
    draw_clear (_win->_draw, _view->style.bg_color);

    if (_view->style.grid_) 
        paint_grid (_win, _view);

    if (_view->style.method_flag & STYLE_METHOD_FLAG_LINES)
        paint_lines (_win, _view);

    if (_view->style.method_flag & STYLE_METHOD_FLAG_POINTS)
        paint_points (_win, _view);
}

void paint_grid (win *_win, plot_view *_view) {
    if (_view->y_min < 0.0) {
        int32_t y = _win->_conf->h +
                (int32_t)(_view->y_min / _view->h * (float)_win->_conf->h);
        draw_rect (_win->_draw,
                0, y - _view->style.grid_origin_line_hw,
                _win->_conf->w, _view->style.grid_origin_line_w,
                _view->style.grid_origin_color);
    }

    if (_view->x_min < 0.0) {
        int32_t x =
                -(int32_t)(_view->x_min / _view->w * (float)_win->_conf->w);

        draw_rect (_win->_draw,
                x - _view->style.grid_origin_line_hw, 0,
                _view->style.grid_origin_line_w, _win->_conf->h,
                _view->style.grid_origin_color);
    }
}

void paint_lines (win *_win, plot_view *_view) {
    if (_view->n < 2)
        return;

    float *_p = _view->_p;
    for (uint32_t c = 0; c < (_view->n - 1); c ++) {
        for (uint32_t line_c = 0; line_c < _view->style.line_w; line_c ++) {
            draw_line (_win->_draw,
                    (int32_t)(_p[0] * _win->_conf->w) + line_c,
                    _win->_conf->h - (int32_t)(_p[1] * _win->_conf->h),
                    (int32_t)(_p[2] * _win->_conf->w) + line_c,
                    _win->_conf->h - (int32_t)(_p[3] * _win->_conf->h),
                    _view->style.line_color);
            _p += 2;
        }
    }
}

void paint_points (win *_win, plot_view *_view) {
    if (_view->n < 1)
        return;

    float *_p = _view->_p;
    for (uint32_t c = 0; c < _view->n; c ++) {
        draw_rect (_win->_draw,
                (int32_t)(_p[0] * _win->_conf->w) - _view->style.pt_hw,
                _win->_conf->h - (int32_t)(_p[1] * _win->_conf->h) -
                    _view->style.pt_hh,
                _view->style.pt_w, _view->style.pt_h,
                _view->style.pt_color);
        _p += 2;
    }
}

