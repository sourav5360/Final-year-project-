#include "ssd1306_i2c.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include <pthread.h>
struct ps
{
	int st;
	pthread_t *thr;
};
//
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//

int status = 0;
void *recvsocket(void *arg)//
{
	struct ps *p = (struct ps *)arg;
	int st = p->st;
	char s[1024];
 
	while(1)
	{
		memset(s, 0, sizeof(s));
		//bytes_read = read(client, buf, sizeof(buf));
		int rc = read(st, s, sizeof(s));
		if (rc <= 0)//
			break;
		printf("phone：%s", s);
		printf("Display OLED\n");
		ssd1306_Display();
		ssd1306_drawString(s);
		ssd1306_display();
		//delay(100);
               }
	
	status = 0;
	pthread_mutex_unlock(&mutex);
	pthread_cancel(*(p->thr));
	return NULL;
}
	
