#pragma once
#ifndef _COMMON_H_
#define _COMMON_H_

#include<Windows.h>
#include<stdio.h>
#include<strsafe.h>

#define STR_LEN		32

typedef bool (*FuncRetCallback)(void* pvParam);

// 公共数据定义
typedef enum
{
	TYPE_TASK_1 = 0,
	TYPE_TASK_2,
	TYPE_TASK_3,
	TYPE_MAX
}TaskType;

typedef struct _st_DATA_HEADER
{
	int nType;
}DATA_HEADER, *PDATA_HEADER;

typedef struct _st_CALL_BACK
{
	FuncRetCallback pCallback;
}CALL_BACK, *PCALL_BACK;

// 任务类型1数据定义

typedef bool (*FuncPrintCallback)(const char*);

typedef struct _st_TASK_1_DATA
{
	char szMsg[STR_LEN];
	FuncPrintCallback pCallback;
}TASK_1_DATA, *PTASK_1_DATA;

typedef struct _st_TASK_1
{
	DATA_HEADER header;
	CALL_BACK callback;
	TASK_1_DATA data;
}TASK_1, *PTASK_1;

typedef struct _st_TASK_1_RET_DATA
{
	int nRet;
}TASK_1_RET_DATA, *PTASK_1_RET_DATA;

typedef struct _st_TASK_1_RET
{
	DATA_HEADER header;
	TASK_1_RET_DATA ret;
}TASK_1_RET, *PTASK_1_RET;

// 任务类型2数据定义
typedef struct _st_TASK_2_DATA
{
	char szMsg[STR_LEN];
}TASK_2_DATA, * PTASK_2_DATA;

typedef struct _st_TASK_2
{
	DATA_HEADER header;
	CALL_BACK callback;
	TASK_2_DATA data;
}TASK_2, * PTASK_2;

typedef struct _st_TASK_2_RET_DATA
{
	int nRet;
}TASK_2_RET_DATA, * PTASK_2_RET_DATA;

typedef struct _st_TASK_2_RET
{
	DATA_HEADER header;
	TASK_2_RET_DATA ret;
}TASK_2_RET, * PTASK_2_RET;

// 任务类型3数据定义
typedef struct _st_TASK_3_DATA
{
	char szMsg[STR_LEN];
}TASK_3_DATA, * PTASK_3_DATA;

typedef struct _st_TASK_3
{
	DATA_HEADER header;
	CALL_BACK callback;
	TASK_3_DATA data;
}TASK_3, * PTASK_3;

typedef struct _st_TASK_3_RET_DATA
{
	int nRet;
}TASK_3_RET_DATA, * PTASK_3_RET_DATA;

typedef struct _st_TASK_3_RET
{
	DATA_HEADER header;
	TASK_3_RET_DATA ret;
}TASK_3_RET, * PTASK_3_RET;

#endif // !_COMMON_H_
