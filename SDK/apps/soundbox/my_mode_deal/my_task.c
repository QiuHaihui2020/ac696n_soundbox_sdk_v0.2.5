#include "system/includes.h"
#include "media/includes.h"
#include "tone_player.h"

#include "app_config.h"
#include "app_action.h"

#include "btstack/avctp_user.h"
#include "btstack/ble_api.h"
#include "btstack/btstack_task.h"
#include "btctrler/btctrler_task.h"
#include "user_cfg.h"
#include "aec_user.h"
#include "lmp_api.h"
#include "audio_reverb.h"
#include "bt_common.h"

#include "ui/ui_api.h"
#include "fm_emitter/fm_emitter_manage.h"

#include "bt_tws.h"

#include "bt_ble.h"

#include "app_chargestore.h"

#include "asm/charge.h"
#include "app_charge.h"
#include "ui_manage.h"

#include "app_chargestore.h"
#include "app_online_cfg.h"
#include "app_main.h"
#include "app_power_manage.h"
#include "gSensor/gSensor_manage.h"
#include "key_event_deal.h"
#include "classic/tws_api.h"
#include "asm/pwm_led.h"

#include "vol_sync.h"

#include "math.h"

#include "dma_deal.h"
#include "rcsp_bluetooth.h"
#include "clock_cfg.h"
#include "rcsp_adv_bluetooth.h"

#include "debug.h"

#include "task.h"
#include "os/os_api.h"

#include "my_task.h"
#include "my_audio_dev.h"



#if TCFG_APP_MYTEST_EN



char *taskState[]={"eRunning","eReady","eBlocked","eSuspended","eDeleted","eInvalid"};
TaskHandle_t Task_Mytest_Handle1;
TaskHandle_t Task_Mytest_Handle2;
StackType_t MyTask2Stack[2048]; //任务堆栈
StaticTask_t MyTask2TaskTCB; //任务控制块
QueueHandle_t Message_Queue; //信息队列句柄

OS_SEM my_mic_os_sem;//信号量句柄
OS_SEM my_pc_os_sem;//信号量句柄
OS_SEM my_linein_os_sem;//信号量句柄
struct my_usr_cbuffer *my_pc_cbuffer = NULL;


void showTaskState()
{
    extern const struct task_info task_info_table[];
    extern int TaskNums;
    TaskHandle_t tempTaskHandle;
    TaskHandle_t currentTaskHandle;
    TaskStatus_t pxTasStatus;
    BaseType_t xGetFreeStackSpace = pdFALSE;
    eTaskState eState = eInvalid;
    
    currentTaskHandle = xTaskGetCurrentTaskHandle();
    
    vTaskGetInfo(currentTaskHandle,
                &pxTasStatus,
                xGetFreeStackSpace,
                eState);
    printf("\nTotal task numner = %d \n",uxTaskGetNumberOfTasks()) ;
    printf("RunningTask:\n");
    printf("TaskName = %s\t\ttaskState = %s\t\tTaskPrio = %d\t\tTaskNumber = %d\n",
                pxTasStatus.pcTaskName,
                taskState[pxTasStatus.eCurrentState],
                pxTasStatus.uxCurrentPriority,
                pxTasStatus.xTaskNumber);   
    printf("TaskList:\n"); 
    for(int i=0;i<TaskNums-1;i++){
        tempTaskHandle = xTaskGetHandle(task_info_table[i].name);
        if(tempTaskHandle){
            vTaskGetInfo(tempTaskHandle,
                        &pxTasStatus,
                        xGetFreeStackSpace,
                        eState);
            printf("TaskName = %s\t\ttaskState = %s\t\tTaskPrio = %d\t\tTaskNumber = %d",
                        pxTasStatus.pcTaskName,
                        taskState[pxTasStatus.eCurrentState],
                        pxTasStatus.uxCurrentPriority,
                        pxTasStatus.xTaskNumber);
      }
    }
    
    tempTaskHandle = xTaskGetIdleTaskHandle();

    vTaskGetInfo(tempTaskHandle,
                &pxTasStatus,
                xGetFreeStackSpace,
                eState);
    printf("TaskName = %s\t\ttaskState = %s\t\tTaskPrio = %ld\t\tTaskNumber = %ld\n\n",
                pxTasStatus.pcTaskName,
                taskState[pxTasStatus.eCurrentState],
                pxTasStatus.uxCurrentPriority,
                pxTasStatus.xTaskNumber); 
}

static void my_task_fun1(void *_scan_para)
{
    printf("my_task_fun1\n");
    // os_sem_create(&my_linein_os_sem, 0);//创建信号量
    os_sem_create(&my_pc_os_sem, 0);//创建信号量
    my_pc_cbuffer = zalloc(sizeof(struct my_usr_cbuffer));
    cbuf_init(&my_pc_cbuffer->my_adc_to_dac_cbuf,my_pc_cbuffer->my_tmp_buffer,2048);


    // s16 tmp_mic_buf[512];
    // s16 tmp_linein_buf[512];
    s16 tmp_pc_buf[512];
    int len=0;
    while (1)
    {
        os_time_dly(1000);
        printf("my_task_fun1\n");
    }
    
    
}
int BaseType;
void mytask_create()
{

    /*注册按键扫描定时器*/
//     sys_s_hi_timer_add(NULL, mytest_print_fun, 5000);  
 
    taskENTER_CRITICAL(); 

    BaseType = os_task_create(my_task_fun1, NULL, 1, 2048, 64, "mytask1");

    taskEXIT_CRITICAL();

    if(BaseType != 0)
        printf("my_task_fun1 create fail %d\n",BaseType);
    printf("my_task_fun1 create success %d\n", BaseType);


}

void mytask_del()
{

    if(Message_Queue){

    }

    if(BaseType==0){
        os_task_del("mytask1");//删除任务 
        //os_task_del("mytask2");
    }
    
}



#endif