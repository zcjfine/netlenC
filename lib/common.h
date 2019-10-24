//
// Created by zhoucj on 2019/10/22.
//

#ifndef NETLENC_COMMON_H
#define NETLENC_COMMON_H

#include <sys/socket.h>

int socket_perr(int, int, int);

ssize_t send_perr(int, const void *, size_t, int);

int bind_perr(int, const sockaddr *, socklen_t);

#endif //NETLENC_COMMON_H
