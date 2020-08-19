#include "common/Resample_api.h"
#include "asm/cpu.h"
#include "audio_config.h"
#include "audio_digital_vol.h"

#include "my_digital_deal.h"
#include "math.h"



static struct digital_volume d_volume;



/***********
 * 改变左右声道的音量,只降低音量，不升
 * data:输入处理的数据
 * len：传入数据的长度
 * right_left：0，对右声道都处理
 *             1，对左右声道进行处理
 *             2，对左声道进行处理
 * l_vol: 0~128
 *********/
int my_audio_digital_vol_run(void *data, u32 len, u8 right_left, u8 l_vol, u8 r_vol)
{
    s32 valuetemp;
    s16 *buf;

    if(l_vol>128)
        l_vol = 128;
    if(r_vol>128)
        r_vol = 128;

    buf = data;
    len >>= 1; //byte to point

    for (u32 i = 0; i < len; i += 2) {
        
        ///left channel
        if(right_left>0){
            valuetemp = buf[i];
            valuetemp = (valuetemp * l_vol) >> 7 ; //valuetemp * (l_vol/128)
            buf[i] = (s16)valuetemp;
        }

        ///right channel
        if(right_left<2){
            valuetemp = buf[i+1];
            valuetemp = (valuetemp *r_vol) >> 7 ; //valuetemp * (l_vol/128)
            buf[i+1] = (s16)valuetemp;
        }
    }
    return 0;
}




/***********
 * 左右声道合并
 * data:出入的待处理的数据
 * len：传入数据的长度
 * right_left：0，合并到左声道
 *             1，合并到右声道
 *********/
int my_audio_digital_Channel_merging_add(void *data, u32 len, u8 right_left)
{
    if(data == NULL){
        return -1;
    }

    s32 valuetemp;
    s16 *buf;

    buf = data;
    len >>= 1; //byte to point

    for (u32 i = 0; i < len; i += 2) {

        valuetemp = (buf[i] + buf[i + 1]);

        if (valuetemp < -32768) {
            valuetemp = -32768;
        } else if (valuetemp > 32767) {
            valuetemp = 32767;
        }
        buf[i + right_left] = (s16)valuetemp;
        buf[i + 1 - right_left] = (s16)0;
    }


    return 0;

}


/***********
 * 左右声道合并
 * data:出入的待处理的数据
 * len：传入数据的长度
 * right_left：0，合并到左声道
 *             1，合并到右声道
 *********/
int my_audio_digital_Channel_merging_swap(void *data, u32 len, u8 right_left)
{
    if(data == NULL){
        return -1;
    }


    s16 *buf;

    buf = data;
    len >>= 1; //byte to point

    for (u32 i = 0; i < len; i += 4) {
        if(!right_left){
            buf[i+1] = 0;
            buf[i+2] = buf[i+3];
            buf[i+3] = 0;
        }
        else{
            buf[i] = 0;
            buf[i+3] = buf[i+2]; 
            buf[i+2] = 0;           
        }
    }

    return 0;

}



/***********
 * 删除一个声道的声音
 * data:出入的待处理的数据
 * len：传入数据的长度
 * right_left：0，删除左声道
 *             1，删除右声道
 *********/
int my_audio_digital_del_one_channel(void *data, u32 len, u8 right_left)
{
    if(data == NULL){
        return -1;
    }

    s16 *buf;
    buf = data;
    len >>= 1; //byte to point

    for (u32 i = 0; i < len; i += 2) {
        if(right_left){
            buf[i+1] = 0;
        }
        else{
            buf[i] = 0;
        }
    }


    return 0;

}

/***********
 * 当声道变双声道
 * data:出入的待处理的数据
 * len：传入数据的长度，字节数
 *********/
int my_audio_digital_one_to_two_channel(void *data, u32 len , s16 *output_buf)
{
    if(data == NULL){
        return -1;
    }

    s16 *buf;
    buf = data;
    len >>= 1;

    for (int i = 0; i < len; i++) {//在一个新数组放数据
        output_buf[2*i] = buf[i];
        output_buf[2*i+1] = buf[i];
    } 
    return 0;

}



static RS_STUCT_API *my_src_api = NULL;
static u8 *my_src_buf = NULL;
static s16 my_src_output[2048];
static int last_points_nums = 1024;
int current_points_nums = 0;
u16 newsrI = 44100;
u32 my_src_need_buf;

/***********
 * 改变输出采样率初始化
 * input_rate：输入采样率
 * output_rate：输出采样率
 *********/
int my_audio_change_output_sample_rate_init(u16 input_rate, u16 output_rate)
{
    my_src_api = get_rs16_context();
    g_printf("sw_src_api:0x%x\n", my_src_api);
    ASSERT(my_src_api);
    my_src_need_buf = my_src_api->need_buf();
    g_printf("sw_src_buf:%d\n", my_src_need_buf);
    my_src_buf = malloc(my_src_need_buf);
    ASSERT(my_src_buf);
    RS_PARA_STRUCT rs_para_obj;
    rs_para_obj.nch = 2;

    newsrI = input_rate;
    rs_para_obj.new_insample = input_rate;
    rs_para_obj.new_outsample = output_rate;
    printf("my src,in = %d,out = %d\n", rs_para_obj.new_insample, rs_para_obj.new_outsample);
    my_src_api->open(my_src_buf, &rs_para_obj);
    return 0;

}

/***********
 * 改变输出采样率
 * data：输入数据
 * len：数据长度，单位，字节
 * output_buf:输出的数据
 * 返回值：输出数据的长度
 *********/
int my_audio_change_sample_rate_output(s16 *data, u16 len, s16 *output_buf)
{
    u16 outlen = len;



    if (my_src_api && my_src_buf ) {
        outlen = my_src_api->run(my_src_buf, data, len >> 1, output_buf);
        ASSERT(outlen <= (sizeof(my_src_output) >> 1));
        /* printf("16->48k:%d,%d,%d\n",len >> 1,outlen,sizeof(sw_src_output)); */
    } 

    current_points_nums = app_audio_output_get_cur_buf_points();

    newsrI =  newsrI - (last_points_nums - current_points_nums);

    my_src_api->set_sr(my_src_buf, newsrI);

    last_points_nums = current_points_nums;

    printf("%d-%d",current_points_nums, newsrI);

    return outlen;   
}

/************
 * 计算均方根（RMS） 即能量值
 * data：输入的数据
 * len：数据长度，单位，字节
 * 返回值：均方根
 * ***********/
s16 my_audio_data_energy(s16 *data, u16 len)
{
    len = len >> 2;
    s16 sum_square = 0, rms = 0;
    for(u16 i=0;i<len;i++){
        sum_square += data[i] * data[i];
    }
    rms = sqrt(sum_square / len);
    return rms;
}