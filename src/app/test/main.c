#include <stdio.h>
#include "sys_plat.h"
#include "tcp_echo_client.h"
#include "tcp_echo_server.h"
#include "net.h"
#include "netif_pcap.h"
#include "dbg.h"
#include "nlist.h"
#include "mblock.h"


net_err_t netdev_init(){
	netif_pcap_open();
	return NET_ERR_OK;
}

typedef struct _tnode_t
{
	nlist_node_t node;
	int id;
}tnode_t;

void nlist_test(void){

	#define NODE_CNT 	4	
	tnode_t node[NODE_CNT];
	nlist_t list;

	nlist_init(&list);
	for(int i = 0; i < NODE_CNT; i++){
		node[i].id = i;
		nlist_insert_first(&list, &node[i].node);
	}


	plat_printf("insert first\n");
	nlist_node_t *p;
	nlist_for_each(p, &list){
		tnode_t * tnode = nlist_entry(p, tnode_t, node);
		plat_printf("id: %d\n", tnode->id);
	}
	plat_printf("remove first\n");
	for(int i = 0; i < NODE_CNT; i++){
		p = nlist_remove_first(&list);
		plat_printf("id:%d\n", nlist_entry(p, tnode_t, node)->id);
	}


	plat_printf("insert last\n");
	for(int i = 0; i < NODE_CNT; i++){
		nlist_insert_last(&list, &node[i].node);
	}
	nlist_for_each(p, &list){
		tnode_t * tnode = nlist_entry(p, tnode_t, node);
		plat_printf("id: %d\n", tnode->id);
	}

	plat_printf("remove last\n");
	for(int i = 0; i < NODE_CNT; i++){
		p = nlist_remove_last(&list);
		plat_printf("id:%d\n", nlist_entry(p, tnode_t, node)->id);
	}

	plat_printf("insert after\n");
	for(int i = 0; i < NODE_CNT; i++){
		nlist_insert_after(&list, nlist_first(&list), &node[i].node);
	}

	nlist_for_each(p, &list){
		tnode_t * tnode = nlist_entry(p, tnode_t, node);
		plat_printf("id: %d\n", tnode->id);
	}
}

void mblock_test(){
	mblock_t blist;
	static uint8_t buffer[100][10];

	mblock_init(&blist, buffer, 100, 10, NLCOKER_THREAD);

	void *temp[10];
	for(int i = 0; i < 10; i++){
		temp[i] = mblock_alloc(&blist, 0);
		plat_printf("block: %p, free_count :%d\n", temp[i], mblock_free_cnt(&blist));
	}


	for(int i = 0; i < 10; i++){
		mblock_free(&blist, temp[i]);
		plat_printf("free_count :%d\n", mblock_free_cnt(&blist));
	}

	mblock_destroy(&blist);
}

void basic_test(void){
	//nlist_test();
	mblock_test();
}


#define DBG_TEST DBG_LEVEL_INFO
int main(void){
	dbg_info(DBG_TEST, "info");
	dbg_warning(DBG_TEST, "warning");
	dbg_error(DBG_TEST, "error");
	dbg_assert(1 == 1, "failed");


	net_init();

	basic_test();
	net_start();

	netdev_init();

	while(1){
		sys_sleep(10);
	}
	return 0;
}