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
	
void *sendsocket(void *arg)
{
	int st = *(int *)arg;
	char s[1024];
	while(1)
	{
		memset(s, 0, sizeof(s));
		read(STDIN_FILENO, s, sizeof(s));
		write(st, s, strlen(s));
	}
	return NULL;
}

int main(int arg, char *args[]) {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);
    //ssd1306 begin
	ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening 
listen(s,1);
	//accepting one connection
	client = accept(s,(struct sockaddr *)&rem_addr, &opt;
	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "accepted connection from %s\n", buf);
	memset(buf, 0, sizeof(buf));
	//read data from the client
	bytes_read= read(client, buf, sizeof(buf));
	if(bytes_read > 0)
			{
printf("received [%s]\n", buf);
ssd1306_clearDisplay();
ssd1306_drawString(buf);
ssd1306_display();
delay(200);
			}
			else{
				print("no receieve");
			}
			}
			
			
		ba2str( &rem_addr.rc_bdaddr, buf );
		fprintf(stderr, "accepted connection from %s\n", buf);
		//memset(buf, 0, sizeof(buf));
		struct ps ps1;
		ps1.st=client;
		ps1.thr=&thrd2;
		pthread_create(&thrd1,NULL,recvsocket,&ps1);
		pthread_detach(thrd1);
		pthread_create(&thrd2,NULL,sendsocket,&client);
		pthread_detach(thrd2);
	}
	close(client);
    close(s);
{

  char buffer[64];
	char * p = buffer;
	int n;
  va_list args;
  va_start (args, format);
  vsnprintf (buffer, sizeof(buffer)-1);
	n = strlen(buffer);
		
	while (*p != 0 && n-->0)
	{
		write ( (uint8_t) *p++);
	}

  va_end (args);
}
	while(1){
		const char * p = string;
	int n = str(string);
	
	while (*p != 0 && n-->0)
	{
		write ( (uint8_t) *p++);
	}
		
	}
    // close connection
    close(client);
    close(s);


	ssd1306_display();
	ssd1306_clearDisplay();
	delay(4000);

	char* text = "Raspberry Pi";
	ssd1306_drawString(text);
	ssd1306_display();
	delay(4000);

	ssd1306_dim(1);
	ssd1306_startscrollright(00,0xFF);
	delay(4000);

	ssd1306_clearDisplay();
	ssd1306_fillRect(10,10, 50, 20, WHITE);
	ssd1306_fillRect(80, 10, 130, 50, WHITE);
	ssd1306_display();
}
