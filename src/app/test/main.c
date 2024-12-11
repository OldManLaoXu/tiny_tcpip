/**
 * @file main.c
 * @author lishutong (527676163@qq.com)
 * @brief 测试主程序，完成一些简单的测试主程序
 * @version 0.1
 * @date 2022-10-23
 *
 * @copyright Copyright (c) 2022
 * @note 该源码配套相应的视频课程，请见源码仓库下面的README.md
 */
#include <stdio.h>
#include "sys_plat.h"
#include "tcp_echo_client.h"
#include "tcp_echo_server.h"
#include "net.h"

static sys_sem_t sem;
static sys_mutex_t mutex;
static int count = 0;

static char buf[100];
static int write_index, read_index;
static sys_sem_t read_sem, write_sem;
void thread1_entry(void *arg){
	for(int i = 0; i < 2 * sizeof(buf); i++){
		sys_sem_wait(read_sem, 0);
		
		char data = buf[read_index++];
		
		if(read_index >= sizeof(buf)){
			read_index = 0;
		}

		sys_sem_notify(write_sem);
		plat_printf("thread1: read data = %d\n", data);
		
		sys_sleep(200);
	}
	// mutex 互斥锁测试
	// for(int i = 1000000; i > 0; i--){
	// 	sys_mutex_lock(mutex);
	// 	count++;
	// 	sys_mutex_unlock(mutex);
	// }
	// plat_printf("count: %d\n", count);

	// 信号量测试
	// while(1){
	// 	plat_printf("thread1 %s\n",(char *)arg);
	// 	sys_sem_notify(sem);
	// 	sys_sleep(1000);
	// }
}

void thread2_entry(void *arg){
	sys_sleep(100);
	for(int i = 0; i < 2 * sizeof(buf); i++){
		sys_sem_wait(write_sem, 0);
		buf[write_index++] = i;

		if(write_index >= sizeof(buf)){
			write_index = 0;
		}	
		plat_printf("thread2: write data = %d\n", i);
		sys_sem_notify(read_sem);
		//sys_sleep(100);
	}

	// for(int i = 1000000; i > 0; i--){
	// 	sys_mutex_lock(mutex);
	// 	count--;
	// 	sys_mutex_unlock(mutex);
	// }
	// plat_printf("count: %d\n", count);
	// while(1){
	// 	sys_sem_wait(sem, 0);
	// 	plat_printf("thread2 %s\n",(char *)arg);
	// }	
}

int main(void){
	net_init();
	net_start();


	sem = sys_sem_create(0);
	mutex = sys_mutex_create();
	read_sem = sys_sem_create(0);
	write_sem = sys_sem_create(sizeof(buf));

	//sys_thread_create(thread1_entry, "aaaa");
	//sys_thread_create(thread2_entry, "bbbb");
	
	//tcp_echo_client_start(friend0_ip, 5000);
	tcp_echo_server_start(5000);
	pcap_t *pcap = pcap_device_open(netdev0_phy_ip, netdev0_hwaddr);
	while(pcap){
		static uint8_t buffer[1024];
		static int counter = 0;
		struct pcap_pkthdr *pkthdr;
		const uint8_t *pkt_data;
		plat_printf("begin test: %d\n", counter++);

		for(int i = 0; i < sizeof(buffer); i++){
			buffer[i] = i;
		}

		if(pcap_next_ex(pcap, &pkthdr, &pkt_data) != 1){
			continue;
		}

		int len = pkthdr->len > sizeof(buffer) ? sizeof(buffer) : pkthdr->len;
	
		plat_memcpy(buffer, pkt_data, len);

		buffer[0] = 1;
		buffer[1] = 2;
		if(pcap_inject(pcap, buffer, len) == -1){
			plat_printf("pcap send packet failed %s\n", pcap_geterr(pcap));
			break;
		}

	}
	printf("中文测试");
	printf("ENGlish test");
	return 0;
}