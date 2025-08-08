#include "sysclock.h"
#include "drv_common.h"
#include "xprintf.h"
#include "xshell.h"

#ifdef USE_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif // #ifdef USE_OS_FREERTOS

/* lwIP core includes */
#include "lwip/opt.h"

#include "lwip/err.h"
#include "lwip/netif.h"

#include "lwip/init.h"
#include "lwip/ip4_addr.h"

#include "lwip/sys.h"
#include "lwip/api.h"

#include "lwip/sockets.h"

// Note: The following functions have been bound to specific functions to demonstrate the corresponding functions
//       Through these examples, we cover the usage of the base send/recv api
//       1. iperf_tcp_server
//          only used for iperf tcp test, you need start a iperf tcp client on PC
//          cover tcp recv api
//       2. iperf_tcp_client
//          you can start it by shell command 'tcp', it will recv/send data from/to PC
//          cover tcp send/recv api
//       3. iperf_udp_server
//          you can start a udp client on PC and send data, it will recv data from PC
//          cover udp recv api
//       4. iperf_udp_client
//          you can start it by shell command 'udp', only used for udp speed test
//          cover udp send api

#define CONFIG_LOCAL_IP_AND_PORT                          \
    uint32_t ui32IpAndPort[2];                            \
                                                          \
    ip4_addr_t _ipaddr;                                   \
    IP4_ADDR(&_ipaddr,                                    \
             IP_ADDR0,                                    \
             IP_ADDR1,                                    \
             IP_ADDR2,                                    \
             IP_ADDR3);                                   \
                                                          \
    ui32IpAndPort[0] = inet_addr(ip4addr_ntoa(&_ipaddr)); \
    ui32IpAndPort[1] = 5001;                              \
                                                          \
    xprintf("local: %d.%d.%d.%d (port %d)\n",             \
            (PP_HTONL(ui32IpAndPort[0]) >> 24) & 0xff,    \
            (PP_HTONL(ui32IpAndPort[0]) >> 16) & 0xff,    \
            (PP_HTONL(ui32IpAndPort[0]) >> 8) & 0xff,     \
            (PP_HTONL(ui32IpAndPort[0]) >> 0) & 0xff,     \
            ui32IpAndPort[1]);

#define CONFIG_REMOTE_IP_AND_PORT                      \
    uint32_t ui32IpAndPort[2];                         \
                                                       \
    ui32IpAndPort[0] = inet_addr("192.168.0.111");     \
    ui32IpAndPort[1] = 5001;                           \
                                                       \
    xprintf("remote: %d.%d.%d.%d (port %d)\n",         \
            (PP_HTONL(ui32IpAndPort[0]) >> 24) & 0xff, \
            (PP_HTONL(ui32IpAndPort[0]) >> 16) & 0xff, \
            (PP_HTONL(ui32IpAndPort[0]) >> 8) & 0xff,  \
            (PP_HTONL(ui32IpAndPort[0]) >> 0) & 0xff,  \
            ui32IpAndPort[1]);

void iperf_tcp_server(void *pvParameters)
{
#define IPERF_BUFSZ (4 * 1024)

    CONFIG_LOCAL_IP_AND_PORT

    struct netconn *conn, *newconn;
    void *recv_data;

    recv_data = (void *)pvPortMalloc(IPERF_BUFSZ);
    if (recv_data == NULL) {
        xprintf("No memory\n");
    }

    conn = netconn_new(NETCONN_TCP);
    if (conn == NULL) {
        xprintf("Failed to create netconn\n");
    } else {
        xprintf("Create conn successful\n");
    }

    if (netconn_bind(conn, IP_ADDR_ANY, ui32IpAndPort[1]) != ERR_OK) {
        xprintf("Failed to bind port\n");
    } else {
        xprintf("Bind port successful\n");
    }

    if (netconn_listen(conn) != ERR_OK) {
        xprintf("Failed to listen on port\n");
    } else {
        xprintf("Listen port successful\n");
    }

    while (1) {
        err_t err = netconn_accept(conn, &newconn);

        xprintf("Connect accepted\n");

        if (err == ERR_OK) {
            struct netbuf *buf;
            u16_t len;

            while ((err = netconn_recv(newconn, &buf)) == ERR_OK) {
                do {
                    netbuf_data(buf, &recv_data, &len);
#if LWIP_DEBUG_BY_USER_IPERF_SERVER
                    xprintf("tcp recv, len = %d\n", len);
                    for (int i = 0; i < len; i++) {
                        xprintf("%c", ((char *)recv_data)[i]);
                    }
                    xprintf("\n");
#endif
                } while (netbuf_next(buf) >= 0);
                netbuf_delete(buf);
            }

            xprintf("Connect closed\n");

            netconn_close(newconn);
            netconn_delete(newconn);
        }
    }
}

void iperf_tcp_client(void *pvParameters)
{
#define IPERF_BUFSZ (4 * 1024)

    CONFIG_REMOTE_IP_AND_PORT

    struct netconn *conn;
    uint8_t *recv_send_data;

    ip4_addr_t ipaddr;
    ipaddr.addr = ui32IpAndPort[0];

    recv_send_data = (uint8_t *)pvPortMalloc(IPERF_BUFSZ);
    if (!recv_send_data) {
        return;
    }

    for (int i = 0; i < IPERF_BUFSZ; i++) {
        recv_send_data[i] = i & 0xff;
    }

    while (1) {
        conn = netconn_new(NETCONN_TCP);
        if (conn == NULL) {
            xprintf("Create conn failed\n");
            vTaskDelay(10);
            continue;
        } else {
            xprintf("Create conn successful\n");
        }

        if (netconn_connect(conn, &ipaddr, ui32IpAndPort[1]) != ERR_OK) {
            xprintf("Connect failed\n");
            netconn_close(conn);
            vTaskDelay(10);
            continue;
        }

        xprintf("Connect iperf server successful\n");

#ifdef LWIP_IPERF_CLIENT_RECV
        struct netbuf *buf;
        u16_t len;

        while (netconn_recv(conn, &buf) == ERR_OK) {
            do {
                netbuf_data(buf, (void **)&recv_send_data, &len);
#if LWIP_DEBUG_BY_USER_IPERF_CLIENT
                xprintf("tcp recv, len = %d\n", len);
                for (int i = 0; i < len; i++) {
                    xprintf("%c", ((char *)recv_send_data)[i]);
                }
                xprintf("\n");
#endif
                netconn_write(conn, recv_send_data, len, NETCONN_COPY);
            } while (netbuf_next(buf) >= 0);
            netbuf_delete(buf);
        }
#endif // #ifdef LWIP_IPERF_CLIENT_RECV

#ifdef LWIP_IPERF_CLIENT_SEND
        while (1) {
            err_t err = netconn_write(conn, recv_send_data, IPERF_BUFSZ, NETCONN_NOCOPY);
            if (err != ERR_OK) {
                xprintf("Write failed: %d\n", err);
            }

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
#endif // #ifdef LWIP_IPERF_CLIENT_SEND

        netconn_close(conn);
        netconn_delete(conn);

        break;
    }

    vPortFree(recv_send_data);
}

void iperf_udp_server(void *pvParameters)
{
#define IPERF_BUFSZ (4 * 1024)

    CONFIG_LOCAL_IP_AND_PORT

    struct netconn *conn;
    struct netbuf *buf;
    char *recv_data;
    err_t err;

    recv_data = (char *)pvPortMalloc(IPERF_BUFSZ);
    if (recv_data == NULL) {
        xprintf("No memory\n");
    }

    conn = netconn_new(NETCONN_UDP);
    netconn_bind(conn, IP_ADDR_ANY, ui32IpAndPort[1]);

    while (1) {
        err = netconn_recv(conn, &buf);
        if (err == ERR_OK) {
            if (netbuf_copy(buf, recv_data, IPERF_BUFSZ) != buf->p->tot_len) {
                xprintf("netbuf_copy failed\n");
            } else {
                recv_data[buf->p->tot_len] = '\0';
                xprintf("udp recv: %s\n", recv_data);
            }
            netbuf_delete(buf);
        }
    }

    vPortFree(recv_data);
}

void iperf_udp_client(void *pvParameters)
{
#define IPERF_BUFSZ (4 * 1024)

    CONFIG_REMOTE_IP_AND_PORT

    struct netconn *conn;
    uint8_t *recv_send_data;

    ip4_addr_t ipaddr;
    ipaddr.addr = ui32IpAndPort[0];

    recv_send_data = (uint8_t *)pvPortMalloc(IPERF_BUFSZ);
    if (!recv_send_data) {
        return;
    }

    for (int i = 0; i < IPERF_BUFSZ; i++) {
        recv_send_data[i] = '0' + (i % 10);
    }
    recv_send_data[TCP_MSS] = '\0';

    struct netbuf *buf = netbuf_new();
    if (buf == NULL) {
        xprintf("Create netbuf failed\n");
        return;
    }

    netbuf_ref(buf, recv_send_data, strlen((const char *)recv_send_data));

    while (1) {
        conn = netconn_new(NETCONN_UDP);
        if (conn == NULL) {
            xprintf("Create conn failed\n");
            vTaskDelay(10);
            continue;
        } else {
            xprintf("Create conn successful\n");
        }

        for (int i = 0; i < 1000; i++) {
            err_t err;
            do {
                err = netconn_sendto(conn, buf, &ipaddr, ui32IpAndPort[1]);
                if (err != ERR_OK) {
                    if (err == ERR_USE) {
                        continue;
                    } else {
                        xprintf("Write failed: %d\n", err);
                    }
                } else {
                    break;
                }
            } while (1);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));

        netconn_close(conn);
        netconn_delete(conn);

        break;
    }

    netbuf_delete(buf);

    vPortFree(recv_send_data);
}
