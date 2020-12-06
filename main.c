#include "header.h"
#include "method.h"


int main(int argc, char *argv[]){
	if(argc < 4 || argc > 7){
		printf("\n--------주의사항-----------\n");
		printf("매개변수는 3개부터 6개까지 사용 가능합니다.\n");
		printf("매개변수는 다음과 같은 순서로 정의되어 있습니다.\n");\
		printf("모든 매개변수는 입력 시 ' '을 기준으로 구분됩니다. 띄어쓰기를 모두 제거해주세요!\n");
		printf("※ 가 있는  매개변수는 필수 입력 사항입니다. \n");
		printf("※ [1] Root URL (without \"http://\")\n");
		printf("※ [2] path (ex /bWAPP/login.php)\n" );
		printf("※ [3] Cookie\n");
		printf("  [4] Column option \n");
		printf("  [5] Table option\n");
		printf("  [6] Table name\n\n");


		printf("--------사용방법----------\n");
		printf("1. Root URL과 페이지 경로, Cookie를 이용해 SQL Injection이 가능할지 가능성을 확인한다.\n");
		printf("2. 가능성이 있는 것을 확인했다면 4번째 인자를 집어넣어 Column 개수를 확인한다. (any char)\n");
		printf("3. Column 개수를 확인했다면 5번째 인자를 통해 DB안에 있는 모든 Table을 확인한다. (any char)\n");
		printf("4. 6번째 인자로 Table name을 넣어 DB의 구조를 파악한다.\n");
		printf("5. DB 구조를 파악했다면 직접 페이지에서 SQL Injection을 해봄으로써 결과를 확인할 수 있다.\n\n");
		
		exit(0);
	}

	setSocket(argv[1]);
	connectServer();

	switch (argc){
		case 4:
			getIsInjection(argv[2],argv[3]);
			break;
		case 5:
			getColumnNumber(argv[2],argv[3]);
			break;
		case 6:
			getTableList(argv[2],argv[3]);
			break;
		case 7:
			getTableColumn(argv[2],argv[3],argv[6]);
			break;
	}

	close(sock);

	return 0;
}
