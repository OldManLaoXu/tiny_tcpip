#include <stdio.h>
#include "sys_plat.h"
#include "tcp_echo_client.h"
#include "tcp_echo_server.h"
#include "net.h"
#include "netif_pcap.h"
#include "dbg.h"

net_err_t netdev_init(){
	netif_pcap_open();
	return NET_ERR_OK;
}


#define DBG_TEST DBG_LEVEL_INFO
int main(void){
	dbg_info(DBG_TEST, "info");
	dbg_warning(DBG_TEST, "warning");
	dbg_error(DBG_TEST, "error");
	dbg_assert(1 == 1, "failed");
	dbg_assert(1 == 0, "failed");

	net_init();

	net_start();

	netdev_init();

	while(1){
		sys_sleep(10);
	}
	return 0;
}