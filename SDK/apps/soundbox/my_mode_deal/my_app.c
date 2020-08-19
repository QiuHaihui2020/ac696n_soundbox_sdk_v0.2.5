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

#include "my_audio_dev.h"
#include "my_digital_deal.h"
#include "my_task.h"

#if TCFG_APP_MYTEST_EN



#if TCFG_ADKEY_ENABLE
static const u8 app_mytest_key_ad_table[KEY_AD_NUM_MAX][KEY_EVENT_MAX] = {
    [0] =
    {
        /*SHORT*/ KEY_MUSIC_PP,
        /*LONG*/  KEY_POWEROFF,
        /*HOLD*/  KEY_POWEROFF_HOLD,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },
    [1] =
    {
        /*SHORT*/ KEY_MUSIC_PREV,
        /*LONG*/  KEY_VOL_UP,
        /*HOLD*/  KEY_VOL_UP,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_VOL_UP,
        /*TRIBLE*/KEY_NULL,
    },
    [2] =
    {
        /*SHORT*/ KEY_MUSIC_NEXT,
        /*LONG*/  KEY_VOL_DOWN,
        /*HOLD*/  KEY_VOL_DOWN,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_VOL_DOWN,
        /*TRIBLE*/KEY_NULL,
    },
    [3] =
    {
        /*SHORT*/ KEY_REVERB_OPEN,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },
    [4] =
    {
        /*SHORT*/ KEY_SWITCH_PITCH_MODE,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_REVERB_OPEN,
        /*TRIBLE*/KEY_NULL,

    },
    [5] =
    {

        /*SHORT*/ KEY_CHANGE_MODE,
        /*LONG*/  KEY_FM_EMITTER_NEXT_FREQ,
        /*HOLD*/  KEY_FM_EMITTER_NEXT_FREQ,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },

    [6] =
    {
        /*SHORT*/ KEY_FM_EMITTER_NEXT_FREQ,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },
    [7] =
    {
        /*SHORT*/ KEY_FM_EMITTER_PERV_FREQ,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },
    [8] =
    {
        /*SHORT*/ KEY_FM_SCAN_UP,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },

    [9] =
    {
        /*SHORT*/ KEY_FM_SCAN_DOWN,
        /*LONG*/  KEY_NULL,
        /*HOLD*/  KEY_NULL,
        /*UP*/	  KEY_NULL,
        /*DOUBLE*/KEY_NULL,
        /*TRIBLE*/KEY_NULL,
    },

};
#endif



u8 mytest_event_get(struct key_event *key)
{
    printf("mytest_event_get\n");
    if(key == NULL)
    {
        return -1;
    }
    u8 key_event = 0;

/*     key_event = 66;

    return key_event; */

    switch (key->type) {
    case KEY_DRIVER_TYPE_IO:
#if TCFG_IOKEY_ENABLE
        key_event = app_common_key_io_table[key->value][key->event];
#endif
        break;
    case KEY_DRIVER_TYPE_AD:
    case KEY_DRIVER_TYPE_RTCVDD_AD:
#if TCFG_ADKEY_ENABLE
        key_event = app_mytest_key_ad_table[key->value][key->event];
#endif
        break;
    }
    return key_event;
}

unsigned int shiftv = 56, formant_shift = 150;
extern void uart_dev_test_main();
extern void audio_adc_linein_demo(void);
static int _mytest_event_opr(struct sys_event *event)
{
    printf("_mytest_event_opr\n");
    int ret = false;
    struct key_event *key = &event->u.key;
    u8 key_event = mytest_event_get(key);

    switch (key_event)
    {
    case KEY_MUSIC_PP:
        printf("KEY_MUSIC_PP\n");
        r_printf("    KEY_MUSIC_PP \n");


        return true;
    case KEY_MUSIC_PREV:
        r_printf("    KEY_MUSIC_PREV \n");
        printf("KEY_MUSIC_PREV\n");

        return true;
    case KEY_MUSIC_NEXT:
        printf("KEY_MUSIC_NEXT\n");
        r_printf("    KEY_MUSIC_NEXT \n");


        return true;

    case KEY_VOL_UP:
        printf("KEY_VOL_UP\n");
        r_printf("    KEY_VOL_UP \n");

        return true;

    case KEY_VOL_DOWN:
        printf("KEY_VOL_DOWN\n");
        r_printf("    KEY_VOL_DOWN \n");

        return true;

    case KEY_REVERB_OPEN:
        printf("KEY_REVERB_OPEN\n");
        r_printf("    KEY_REVERB_OPEN \n");


        return true;

    case KEY_SWITCH_PITCH_MODE:
        printf("KEY_SWITCH_PITCH_MODE\n");
        r_printf("    KEY_SWITCH_PITCH_MODE \n");

        return true;



    case 66:
        printf("mytest msg_event = %d\n",key_event);
        app_task_msg_post(MYTEST_MSG_TEST, 4 ,9,7,8,5); //发送 5 个数据
        return true;
    default:
        printf("mytest msg_event = %d\n",key_event);
        app_task_msg_post(MYTEST_MSG_TEST, 4 ,9,7,8,5); //发送 5 个数据
        return false;
    }
    return ret;
}





static void mytest_app_init()
{
    printf("mytest_app_init\n");


}

static void mytest_app_uninit()
{
    mytask_del();
    printf("mytest_app_uninit\n");
}


/*表示模式的状态， 模式状态的回调函数， 通常进入模式和退出模式时才会用到*/
static int mytest_state_machine(struct application *app, enum app_state state, struct intent *it)
{
    printf("mytest_state_machine\n");
    int ret;
    switch (state)
    {
    case APP_STA_CREATE:
        break;
    case APP_STA_START:
        if (!it)
        {
            break;
        }
        switch (it->action)
        {
        case ACTION_APP_MAIN:
            printf("ACTION_APP_MAIN\n");
            mytest_app_init();
            break;
        }
        break;
    case APP_STA_PAUSE:
        printf("APP_STA_PAUSE\n");
        break;
    case APP_STA_RESUME:
        printf("APP_STA_RESUME\n");
        break;
    case APP_STA_STOP:

        printf("APP_STA_STOP\n");
        break;
    case APP_STA_DESTROY:
        printf("APP_STA_DESTROY\n");
        mytest_app_uninit();
        break;
    }
    return 0;
}

/*表示模式的事件回调函数， 通常用于处理按键事件和设备事件*/
static int mytest_event_handler(struct application *app, struct sys_event *event)
{
    printf("mytest_event_handler\n");
    int err = 0;
    switch (event->type)
    {
    case SYS_KEY_EVENT:
        return _mytest_event_opr(event);
    case SYS_MYTEST_EVENT:
        break;
    default:
        return false;
    }
    return false;

}

/*
发送用户自定义消息
函数： bool app_task_msg_post(int msg, int argc, ...)
说明:
参数一： 要发送的消息名字参数
参数二： 要发送的数据参数总数
参数三至参数七： 要发送的数据(参数个数为 0-5 个)

app_task_msg_post(MYTEST_MSG_TEST, 5, 1,2,3,4,5); //发送 5 个数据
*/

static int mytest_user_msg_deal(int msg, int argc, int *argv)
{
    printf("mytest_user_msg_deal\n");
    switch (msg)
    {
    case MYTEST_MSG_TEST:
        printf("get user msg %d, msg_val_count = %d, msg_val:\n", msg, argc);
        for(int i=0; i<argc;i++)
        {
            printf("%d ", argv[i]);
        }
        break;

    default:
        /*common 去处理用户自定义消息*/
        return 0;
    }
    /*返回 1 则结束此次消息， 不再执行 common 的函数*/
    return 1;
}

/*模式私有数据*/
static const struct application_reg my_test_reg = {
    .tone_name = TONE_NUM_8,
    .enter_check = NULL,
    .exit_check = NULL,
    .user_msg = mytest_user_msg_deal,
};

/*模式运行的主线--结构体函数组*/
static const struct application_operation my_test_ops = {
    .state_machine  = mytest_state_machine,
    .event_handler 	= mytest_event_handler,
};

/*模式注册*/

#if 1
REGISTER_APPLICATION(app_app_test) = {
    .name 	= APP_NAME_MYAPP,
    .action	= ACTION_APP_MAIN,
    .ops 	= &my_test_ops,
    .state  = APP_STA_DESTROY,
    .private_data = (void *) &my_test_reg,
};
#else
REGISTER_APPLICATION(app_app_test) = {
    .name 	= APP_NAME_LINEIN,
    .action	= ACTION_APP_MAIN,
    .ops 	= &my_test_ops,
    .state  = APP_STA_DESTROY,
    .private_data = (void *) &my_test_reg,
};
#endif



#endif






