/*
* 화일명 : my_assembler.c
* 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
* 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
*
*/

/*
*
* 프로그램의 헤더를 정의한다.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* my_assembler 프로그램의 의존적인 데이터들이 정의된 곳이다.
*/

#include "my_assembler.h"

/* -----------------------------------------------------------------------------------
* 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
* 매계 : 실행 파일, 어셈블리 파일
* 반환 : 성공 = 0, 실패 = < 0
* 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다.
*		   또한 중간파일을 생성하지 않는다.
* -----------------------------------------------------------------------------------
*/


int main(int args, char *arg[])
{

	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}

	if (assem_pass1() < 0){
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}

	if (assem_pass2() < 0){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}

	make_objectcode("output");
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다.
* 매계 : 없음
* 반환 : 정상종료 = 0 , 에러 발생 = -1
* 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기
*		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
*		   구현하였다.
* -----------------------------------------------------------------------------------
*/

int init_my_assembler(void)
{
	int result;

	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input")) < 0)
		return -1;
	return result;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/

static int assem_pass1(void)
{
	/* add your code here */
	for (int i = 0; i < line_num; i++) {
		if (token_parsing(i) == -1)
			return -1;
	}
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/

static int assem_pass2(void)
{
	/* add your code here */
	int tmp;
	for (int i = 0; i < line_num; i++) {
		if (token_table[i]->operator != NULL) {
			tmp = search_opcode(token_table[i]->operator);
		}
		if (tmp != -1) {
			strcpy(inst_to_op[i], inst[tmp][2]);
		}
		else if (tmp == -1){
			strcpy(inst_to_op[i], "");
		}
	}
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을
*        생성하는 함수이다.
* 매계 : 기계어 목록 파일
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : 기계어 목록파일 형식은 다음과 같다.
*
*	===============================================================================
*		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
*	===============================================================================
*
* -----------------------------------------------------------------------------------
*/

int init_inst_file(char *inst_file)
{
	/* add your code here */
	FILE* fin = fopen(inst_file, "r");
	char buffer[25];
	inst_index = 0;

	if (fin == NULL) {
		return -1;
	}

	while (1) {
		if (fgets(buffer, 25, fin) == NULL) {
			break;
		}

		char* tmp;
		int column_num = 0;

		tmp = strtok(buffer, "|");
		inst[inst_index][column_num] = (char*)malloc(strlen(tmp) + 1);
		strcpy(inst[inst_index][column_num++], tmp);
		tmp = strtok(NULL, "|");
		inst[inst_index][column_num] = (char*)malloc(strlen(tmp) + 1);
		strcpy(inst[inst_index][column_num++], tmp);
		tmp = strtok(NULL, "|");
		inst[inst_index][column_num] = (char*)malloc(strlen(tmp) + 1);
		strcpy(inst[inst_index][column_num++], tmp);
		tmp = strtok(NULL, "|");
		inst[inst_index][column_num] = (char*)malloc(strlen(tmp) + 1);
		strcpy(inst[inst_index][column_num++], tmp);
		inst_index++;
	}

	fclose(fin);
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 어셈블리 할 소스코드를 읽어오는 함수이다.
* 매계 : 어셈블리할 소스파일명
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/


int init_input_file(char *input_file)
{
	/* add your code here */
	FILE* fin = fopen(input_file, "r");
	char buffer[100];
	line_num = 0;

	if (fin == NULL) {
		return -1;
	}

	while (1) {
		if (fgets(buffer, 100, fin) == NULL)
			break;
		input_data[line_num] = (char*)malloc(strlen(buffer) + 1);
		strcpy(input_data[line_num++], buffer);
	}

	fclose(fin);
	return 0;
}

/* -----------------------------------------------------------------------------------
* 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다.
*        패스 1로 부터 호출된다.
* 매계 : 소스코드의 라인번호
* 반환 : 정상종료 = 0 , 에러 < 0
* 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다.
* -----------------------------------------------------------------------------------
*/

int token_parsing(int index)
{
	/* add your code here */
	if (index > inst_index || index < 0) {
		return -1;
	}
	int i = 0;
	int len = strlen(input_data[index]);
	token_table[index] = (token*)malloc(sizeof(token));


	if (input_data[index][0] == '.') {
		char *tmp;
		tmp = strtok(input_data[index], "\t\n ");
		token_table[index]->label = (char*)malloc(strlen(tmp) + 1);
		strcpy(token_table[index]->label, tmp);

		tmp = strtok(NULL, "\t\n");
		if (tmp != NULL) {
			token_table[index]->comment = (char*)malloc(strlen(tmp) + 1);
			token_table[index]->operator = NULL;
			token_table[index]->operand[0] = NULL;
			token_table[index]->operand[1] = NULL;
			token_table[index]->operand[2] = NULL;
			strcpy(token_table[index]->comment, tmp);
		}
		else {
			token_table[index]->operator = NULL;
			token_table[index]->operand[0] = NULL;
			token_table[index]->operand[1] = NULL;
			token_table[index]->operand[2] = NULL;
			token_table[index]->comment = NULL;
		}
	}
	else if (input_data[index][0] == '\t') {
		char *tmp;
		token_table[index]->label = NULL;
		tmp = strtok(input_data[index], "\t\n ");
		token_table[index]->operator = (char*)malloc(strlen(tmp) + 1);
		strcpy(token_table[index]->operator, tmp);

		int num_inst = search_opcode(token_table[index]->operator);
		if (num_inst == -1) {
			tmp = strtok(NULL, "\t\n ");
			if (tmp != NULL) {
				token_table[index]->operand[0] = (char*)malloc(strlen(tmp) + 1);
				strcpy(token_table[index]->operand[0], tmp);
			}
			else {
				token_table[index]->operand[0] = NULL;
			}
		}
		else {
			if (strcmp(inst[num_inst][3], "0") != 0) {
				tmp = strtok(NULL, "\t\n ");	//오퍼랜드가 2개이상이면 tab으로 토큰을 자름.
				token_table[index]->operand[0] = (char*)malloc(strlen(tmp) + 1);
				strcpy(token_table[index]->operand[0], tmp);
			}
			else {
				token_table[index]->operand[0] = NULL;
				token_table[index]->operand[1] = NULL;
				token_table[index]->operand[2] = NULL;
			}
		}
		tmp = strtok(NULL, "\t\n ");
		if (tmp != NULL) {
			token_table[index]->comment = (char*)malloc(strlen(tmp) + 1);
			strcpy(token_table[index]->comment, tmp);
		}
	}
	else {
		char *tmp;
		tmp = strtok(input_data[index], "\t\n ");
		token_table[index]->label = (char*)malloc(strlen(tmp) + 1);
		strcpy(token_table[index]->label, tmp);

		tmp = strtok(NULL, "\t\n ");
		token_table[index]->operator = (char*)malloc(strlen(tmp) + 1);
		strcpy(token_table[index]->operator, tmp);

		int num_inst = search_opcode(token_table[index]->operator);
		if (num_inst == -1) {
			tmp = strtok(NULL, "\t\n ");
			if (tmp != NULL) {
				token_table[index]->operand[0] = (char*)malloc(strlen(tmp) + 1);
				strcpy(token_table[index]->operand[0], tmp);
			}
			else {
				token_table[index]->operand[0] = NULL;
			}
		}
		else {
			if (strcmp(inst[num_inst][3], "0") != 0) {
				tmp = strtok(NULL, "\t\n ");	//오퍼랜드가 2개이상이면 tab으로 토큰을 자름.
				token_table[index]->operand[0] = (char*)malloc(strlen(tmp) + 1);
				strcpy(token_table[index]->operand[0], tmp);
			}
			else {
				token_table[index]->operand[0] = NULL;
				token_table[index]->operand[1] = NULL;
				token_table[index]->operand[2] = NULL;
			}
		}
		tmp = strtok(NULL, "\t\n ");
		if (tmp != NULL) {
			token_table[index]->comment = (char*)malloc(strlen(tmp) + 1);
			strcpy(token_table[index]->comment, tmp);
		}
	}
	return 0;
}
/* -----------------------------------------------------------------------------------
* 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다.
* 매계 : 토큰 단위로 구분된 문자열
* 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0
* 주의 :
*
* -----------------------------------------------------------------------------------
*/

int search_opcode(char *str)
{
	/* add your code here */
	for (int i = 0; i < inst_index; i++) {
		if (!strcmp(inst[i][0], str)) {
			return i;
		}
	}
	return -1;
}
/* -----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/

void make_objectcode(char *file_name)
{
	/* add your code here */
	FILE* fout = fopen(file_name, "w");
	for (int i = 0; i < line_num; i++){
		if (token_table[i]->label != NULL && !(strcmp(token_table[i]->label, ".")))
		{
			fprintf(fout, "%s\t", token_table[i]->label);
			if (token_table[i]->comment != NULL) {
				fprintf(fout, "%s\n", token_table[i]->comment);
			}
			else {
				fprintf(fout, "\n");
			}
		}
		else if (token_table[i]->label != NULL)
		{
			fprintf(fout, "%s\t", token_table[i]->label);
			fprintf(fout, "%s\t", token_table[i]->operator);
			fprintf(fout, "%s\t\t", token_table[i]->operand[0]);
			fprintf(fout, "%s\n", inst_to_op[i]);
		}
		else if (token_table[i]->label == NULL)
		{
			fprintf(fout, "\t");
			fprintf(fout, "%s\t", token_table[i]->operator);
			if (token_table[i]->operand[0] != NULL) {
				if (strlen(token_table[i]->operand[0]) >= 8) {
					fprintf(fout, "%s\t", token_table[i]->operand[0]);
				}
				else {
					fprintf(fout, "%s\t\t", token_table[i]->operand[0]);
				}
			}
			else {
				fprintf(fout, "\t\t");

			}
			fprintf(fout, "%s\n", inst_to_op[i]);
		}
	}
	fclose(fout);
}


