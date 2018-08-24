
#ifndef __QKC_INTERNAL_SOCKOPT_H
#define __QKC_INTERNAL_SOCKOPT_H 1

#include <quark_compile.h>
#include <stdint.h>
#include <windows.h>
#include <winsock2.h>

__BEGIN_DECLS


bool sockopt_get_keepalive(SOCKET& s , bool &optval) ;
bool sockopt_set_keepalive(SOCKET& s , bool optval) ;

bool sockopt_get_recv_buffer_size(SOCKET& s , int& optval) ;
bool sockopt_set_recv_buffer_size(SOCKET& s , int optval) ;

bool sockopt_get_send_buffer_size(SOCKET& s , int& optval) ;
bool sockopt_set_send_buffer_size(SOCKET& s , int optval) ;

bool sockopt_get_recv_timeout(SOCKET& s , DWORD& optval) ;
bool sockopt_set_recv_timeout(SOCKET& s , DWORD optval) ;

bool sockopt_get_send_timeout(SOCKET& s , DWORD& optval) ;
bool sockopt_set_send_timeout(SOCKET& s , DWORD optval) ;

bool sockopt_get_nodelay(SOCKET&s , bool& optval) ;
bool sockopt_set_nodelay(SOCKET&s , bool optval) ;

bool socket_get_readable_size(SOCKET& s , DWORD& size) ;
bool socket_set_nonblock(SOCKET& s , bool enable) ;


__END_DECLS

#endif /** __QKC_INTERNAL_SOCKOPT_H */
