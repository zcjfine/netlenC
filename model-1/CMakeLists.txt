add_executable(tcp_server tcp_server.c tcp_client.c)

add_executable(main.c main.c tcp_client.c)