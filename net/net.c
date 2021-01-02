#include "net.h"

bool net_cons (net *_n) {
    dlog_debug (_, "net_cons");

    _n->srv_on_ = false;
    _n->srv_sock = -1;
    _n->cl_connected_ = false;
    _n->cl_sock = -1;
    return true;
}

bool net_decons (net *_n) {
    dlog_debug (_, "net_decons");

    net_close_cl (_n);
    return true;
}

bool net_connect_to (net *_n, char const *_name_s, int32_t port) {
    dlog_debug (_, "net_connect_to");

    if (_n->cl_connected_) {
        dlog_error (_, "net_connect_to: already connected");
        return false;
    }
 
    dlog_debug_s (_, "net_connect_to: gethostbyname", _name_s);
    struct hostent *host_info = gethostbyname(_name_s);
    if ((! host_info) || (host_info->h_addrtype != PF_INET)) {
        dlog_error_s (_, "net_connect_to: gethostbyname", _name_s);
        return false;
    }
 
    dlog_debug (_, "net_connect_to: creating socket");
    _n->cl_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (_n->cl_sock == -1) {
        dlog_error (_, "net_connect_to: socket");
        return false;
    }

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_addr = *(struct in_addr *)*host_info->h_addr_list;
    addr.sin_port = (short)htons(port);
    
    dlog_debug (_, "net_connect_to: connecting");
    int res_i = connect(_n->cl_sock, (struct sockaddr *)&addr, sizeof(addr));
    if (res_i != 0) {
        dlog_error (_, "net_connect_to: connect");
        net_close_cl (_n);
        return false;
    }

    _n->cl_connected_ = true;
    dlog_debug_s (_, "net_connect_to: connected to", _name_s);
    return true;
}

bool net_server (net *_n, int32_t port, void (*srv_fn)(void *)) {
    dlog_debug (_, "net_server");

    struct sockaddr_in addr_srv, addr_cli;
    socklen_t addr_cli_len = sizeof(struct sockaddr_in);
    
    if (_n->srv_on_) {
        dlog_error (_, "net_server: already running");
        return false;
    }
 
    dlog_debug (_, "net_server: creating socket");
    _n->srv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (_n->srv_sock == -1) {
        dlog_error (_, "net_server: socket");
        return false;
    }

    _n->srv_port = port;

    addr_srv.sin_family = PF_INET;
    addr_srv.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_srv.sin_port = (short)htons((short)_n->srv_port);
    
    dlog_debug (_, "net_server: binding socket");
    if (bind(_n->srv_sock, (struct sockaddr *)&addr_srv, sizeof(addr_srv))) {
        close(_n->srv_sock);
        _n->srv_sock = -1;
        dlog_error (_, "net_server: bind");
        return false;
    }
    
    dlog_debug (_, "net_server: listening");
    if (listen(_n->srv_sock, 10) == -1) {
        dlog_error (_, "net_server: listen");
        close(_n->srv_sock);
        _n->srv_sock = -1;
        return false;
    }

    _n->srv_on_ = true;

    while (_n->srv_on_) {
        dlog_debug (_, "net_server: accepting");
        _n->cl_sock = accept(_n->srv_sock, (struct sockaddr *)&addr_cli,
                &addr_cli_len);
        if (_n->cl_sock == -1) {
            dlog_error (_, "net_server: accept");
            break;
        }

        dlog_debug (_, "net_server: connected");
        _n->cl_connected_ = true;

        dlog_debug (_, "net_server: calling srv_fn");
        srv_fn (_n);

        net_close_cl (_n);
    }

    if (_n->srv_sock != -1) {
        close(_n->srv_sock);
        _n->srv_sock = -1;
    }
    _n->srv_on_ = false;
    
    dlog_debug (_, "net_server: exiting");
    return true;
}

bool net_close_cl (net *_n) {
    dlog_debug (_, "net_close_cl");

    if (_n->cl_sock != -1) {    
        close(_n->cl_sock);
        _n->cl_sock = -1;
    }
    _n->cl_connected_ = false;
    return true;
}

bool net_get_line (net const *_n, char *buf, int32_t len) {
    char c;

    int32_t res_i = -1;
    for (uint32_t i = 0; i < len; i ++) {
        res_i = recv(_n->cl_sock, &c, 1, 0);
        // ends on failure, newline or '\0'
        if ((res_i <= 0) || (c == '\n') || (c == 0)) {
            buf[i] = 0;
            break;
        }
        buf[i] = c;
    }

    return (res_i > 0);
}

bool net_send_str (net const *_n, char const *s) {
    // sends '\0' terminator
    int32_t r = send(_n->cl_sock, s, (uint32_t)strlen (s) + 1, 0);
    return (r >= 0);
}

