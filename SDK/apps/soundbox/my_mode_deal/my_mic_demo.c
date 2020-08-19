
#include "asm/includes.h"
#include "media/includes.h"
#include "audio_config.h"




#define MIC_OUTPUT  1       //测试使用，0：不输出mic声音，1：输出mic声音

#define LADC_MIC_BUF_NUM        2       //mic数据buf数
#define LADC_MIC_CH_NUM         1       //mic通道数
#define LADC_MIC_IRQ_POINTS     256     //每次中断的采样点数
#define LADC_MIC_BUFS_SIZE      (LADC_MIC_CH_NUM * LADC_MIC_BUF_NUM * LADC_MIC_IRQ_POINTS)


struct ladc_mic_demo {
    struct audio_adc_output_hdl adc_output;
    struct adc_mic_ch mic_ch;
    s16 adc_buf[LADC_MIC_BUFS_SIZE];    //align 4Bytes
    int cut_timer;
};
static struct ladc_mic_demo *ladc_mic = NULL;

#if MIC_OUTPUT
static s16 dual_ch_buf[LADC_MIC_IRQ_POINTS * 2];
#endif

extern struct audio_adc_hdl adc_hdl;

/**********
 * mic中断函数，每次采集到的数据到传到这里
 * priv：mic参数
 * data：mic采集到的数据
 * len：数据长度
 **********/
static void my_adc_mic_demo_output(void *priv, s16 *data, int len)
{
    struct audio_adc_hdl *hdl = priv;

#if MIC_OUTPUT
    printf("%d %d",data[0], data[1]);   //打印每次mic中断数据的前两个点

    u16 points = len >> 1;      //数据点数
    //将mic数据变成双声道数据
    for (int i = 0; i < points; i++) {
        dual_ch_buf[2 * i] = data[i];
        dual_ch_buf[2 * i + 1] = data[i];
    }

    int wlen = app_audio_output_write(dual_ch_buf,len*2);   //把mic数据输出到dac播放
    if (wlen != len*2) {
        printf("wlen:%d-%d",wlen,len*2);
    }
#endif

}

/**********
 * 开启mic
 * sr：采样率
 **********/
void my_audio_adc_mic_demo(u16 sr)
{
    r_printf("audio_adc_mic_open:%d\n", sr);
    ladc_mic = zalloc(sizeof(struct ladc_mic_demo));
    if (ladc_mic) {
        audio_adc_mic_open(&ladc_mic->mic_ch, AUDIO_ADC_MIC_CH, &adc_hdl);  //设置mic通道
        audio_adc_mic_set_sample_rate(&ladc_mic->mic_ch, sr);               //设置mic采样率
        audio_adc_mic_set_gain(&ladc_mic->mic_ch, 5);                       //设置mic增益
        audio_adc_mic_set_buffs(&ladc_mic->mic_ch, ladc_mic->adc_buf, LADC_MIC_IRQ_POINTS * 2, LADC_MIC_BUF_NUM);   //设置mic数据buf
        ladc_mic->adc_output.handler = my_adc_mic_demo_output;              //mic中断
        ladc_mic->adc_output.priv = &adc_hdl;                               //mic参数
        audio_adc_add_output_handler(&adc_hdl, &ladc_mic->adc_output);      //添加mic中断
        audio_adc_mic_start(&ladc_mic->mic_ch);                             //启动mic采集
        JL_AUDIO->ADC_CON &= ~(0XF<<24);

#if MIC_OUTPUT
        app_audio_output_samplerate_set(sr);
        app_audio_output_start();
#endif
    }
}

/**********
 * 关闭mic
 **********/
void my_audio_adc_mic_exit(void)
{
    //printf("ladc_mic_close\n");
    if (ladc_mic) {
        audio_adc_del_output_handler(&adc_hdl, &ladc_mic->adc_output);
        audio_adc_mic_close(&ladc_mic->mic_ch);
        free(ladc_mic);
        ladc_mic = NULL;
    }
}

extern int audio_dec_init();
extern int audio_enc_init();
/**********
 * 编解码初始化
 **********/
int my_audio_dev_init()
{
    audio_dec_init();
    audio_enc_init();
    return 0;
}



