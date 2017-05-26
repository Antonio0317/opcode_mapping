/*
* ȭ�ϸ� : my_assembler.c
* ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
* �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
*
*/

/*
*
* ���α׷��� ����� �����Ѵ�.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
* my_assembler ���α׷��� �������� �����͵��� ���ǵ� ���̴�.
*/

#include "my_assembler.h"

/* -----------------------------------------------------------------------------------
* ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
* �Ű� : ���� ����, ����� ����
* ��ȯ : ���� = 0, ���� = < 0
* ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
*		   ���� �߰������� �������� �ʴ´�.
* -----------------------------------------------------------------------------------
*/


int main(int args, char *arg[])
{

	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if (assem_pass1() < 0){
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	if (assem_pass2() < 0){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	make_objectcode("output");
	return 0;
}
/* -----------------------------------------------------------------------------------
* ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
* �Ű� : ����
* ��ȯ : �������� = 0 , ���� �߻� = -1
* ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
*		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
*		   �����Ͽ���.
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
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
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
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
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
* ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)��
*        �����ϴ� �Լ��̴�.
* �Ű� : ���� ��� ����
* ��ȯ : �������� = 0 , ���� < 0
* ���� : ���� ������� ������ ������ ����.
*
*	===============================================================================
*		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
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
* ���� : ����� �� �ҽ��ڵ带 �о���� �Լ��̴�.
* �Ű� : ������� �ҽ����ϸ�
* ��ȯ : �������� = 0 , ���� < 0
* ���� :
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
* ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
*        �н� 1�� ���� ȣ��ȴ�.
* �Ű� : �ҽ��ڵ��� ���ι�ȣ
* ��ȯ : �������� = 0 , ���� < 0
* ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
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
				tmp = strtok(NULL, "\t\n ");	//���۷��尡 2���̻��̸� tab���� ��ū�� �ڸ�.
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
				tmp = strtok(NULL, "\t\n ");	//���۷��尡 2���̻��̸� tab���� ��ū�� �ڸ�.
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
* ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
* �Ű� : ��ū ������ ���е� ���ڿ�
* ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0
* ���� :
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
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
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


