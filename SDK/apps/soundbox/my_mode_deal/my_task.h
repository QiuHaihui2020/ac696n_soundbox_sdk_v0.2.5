#ifndef _MY_TASK__H
#define _MY_TASK__H

typedef void * EventGroupHandle_t;


typedef void (*TaskFunction_t)(void *);
typedef long BaseType_t;
typedef unsigned long UBaseType_t;
typedef void * TaskHandle_t;


/* Task states returned by eTaskGetState. */
typedef enum
{
	eRunning = 0,	/* A task is querying the state of itself, so must be running. */
	eReady,			/* The task being queried is in a read or pending ready list. */
	eBlocked,		/* The task being queried is in the Blocked state. */
	eSuspended,		/* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
	eDeleted,		/* The task being queried has been deleted, but its TCB has not yet been freed. */
	eInvalid			/* Used as an 'invalid state' value. */
} eTaskState;

/* Used with the uxTaskGetSystemState() function to return the state of each task
in the system. */
typedef struct xTASK_STATUS
{
	TaskHandle_t xHandle;			/* The handle of the task to which the rest of the information in the structure relates. */
	const char *pcTaskName;			/* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	UBaseType_t xTaskNumber;		/* A number unique to the task. */
	eTaskState eCurrentState;		/* The state in which the task existed when the structure was populated. */
	UBaseType_t uxCurrentPriority;	/* The priority at which the task was running (may be inherited) when the structure was populated. */
	UBaseType_t uxBasePriority;		/* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
	uint32_t ulRunTimeCounter;		/* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
	StackType_t *pxStackBase;		/* Points to the lowest address of the task's stack area. */
	uint16_t usStackHighWaterMark;	/* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack. */
} TaskStatus_t;







extern int TaskNums;
extern const struct task_info task_info_table[];

extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
// #define portENTER_CRITICAL()		vPortEnterCritical();
// #define portEXIT_CRITICAL()			vPortExitCritical();
#define taskENTER_CRITICAL()		portENTER_CRITICAL()
#define taskEXIT_CRITICAL()			portEXIT_CRITICAL()


//extern void vTaskList( char * pcWriteBuffer );//获取任务列表，？调用系统会重启？
//extern void vTaskGetRunTimeStats(char * pcWriteBuffer );//获取所有任务运行时间，？调用系统会重启？
extern void vTaskStartScheduler();//开启任务调度器
extern void vTaskEndScheduler();//关闭任务调度器
extern void vTaskSuspendAll();//挂起所有任务
extern void xTaskResumeAll();//释放所有任务
extern UBaseType_t uxTaskGetNumberOfTasks();//获取系统任务数量
extern TaskHandle_t xTaskGetIdleTaskHandle(void);//获取空闲任务的句柄
extern eTaskState eTaskGetState(TaskHandle_t xTask);//获取某个任务的状态
extern char *pcTaskGetName(TaskHandle_t xTaskToQuery);//句柄找任务名
extern TaskHandle_t xTaskGetHandle(const char * pcNameToQuery);//任务名找句柄
extern TaskHandle_t xTaskGetCurrentTaskHandle();//获取当前任务的句柄
//查看堆栈历史剩余最小值
//extern UBaseType_t uxTaskGetStackHighWaterMark(TaskHandle_t xTask);
//获取单个任务的状态
extern void vTaskGetInfo(TaskHandle_t xTask,
                         TaskStatus_t* pxTasStatus,
                         BaseType_t xGetFreeStackSpace,
                         eTaskState eState);
//获取系统所有任务的状态，？调用系统会重启？
// extern UBaseType_t uxTaskGetSystemState(TaskStatus_t * const pxTaskStatusArray,
//                                         const UBaseType_t uxArraySize,
//                                          uint32_t * const pulTotalRunTime );
//创建任务
extern BaseType_t xTaskCreate(TaskFunction_t pxTaskCode,
                              const char * const pcName,
                              const uint16_t usStaskDepth,
                              void *const pvParameters,
                              UBaseType_t uxPriority,
                              TaskHandle_t *const pxCreateTask);

extern TaskHandle_t xTaskCreateStatic(  TaskFunction_t pxTaskCode,
                                        const char * const pcName,
                                        const uint32_t ulStackDepth,
                                        void * const pvParameters,
                                        UBaseType_t uxPriority,
                                        StackType_t * const puxStackBuffer,
                                        StaticTask_t * const pxTaskBuffer );  
                                                                    
extern void vTaskDelete(TaskHandle_t xTaskToDelete);//删除任务
extern void vTaskSuspend(TaskHandle_t xTaskToSuspend);
extern void vTaskResume(TaskHandle_t xTaskToResume);
extern void vTaskDelay(portTickType xTicksToDelay);
extern void vTaskDelayUntil(portTickType * pxPreviousWakeTime,
                            portTickType xTimeIncrement);


extern QueueHandle_t Message_Queue; //信息队列句柄
extern OS_QUEUE  my_os_queue;  //队列句柄
extern OS_SEM my_mic_os_sem;//信号量句柄
extern OS_SEM my_pc_os_sem;//信号量句柄
extern OS_SEM my_linein_os_sem;//信号量句柄
extern struct my_usr_cbuffer *my_pc_cbuffer;


void showTaskState();
void mytask_create();
void mytask_del();


#endif
