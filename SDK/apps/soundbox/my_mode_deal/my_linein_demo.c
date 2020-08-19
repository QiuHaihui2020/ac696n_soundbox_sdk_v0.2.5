#include "asm/includes.h"
#include "media/includes.h"
#include "audio_config.h"

#define LADC_LINEIN_BUF_NUM        2
#define LADC_LINEIN_CH_NUM         2
#define LADC_LINEIN_IRQ_POINTS     64
#define LADC_LINEIN_BUFS_SIZE      (LADC_LINEIN_CH_NUM * LADC_LINEIN_BUF_NUM * LADC_LINEIN_IRQ_POINTS)
struct audio_adc_var {
    struct audio_adc_output_hdl adc_output;
    struct audio_adc_ch ch;
    s16 adc_buf[LADC_LINEIN_BUFS_SIZE];    //align 4Bytes
};
extern struct audio_adc_hdl adc_hdl;
static struct audio_adc_var *ladc_linein = NULL;
static s16 dual_ch_buf[LADC_LINEIN_BUFS_SIZE * 2];


static void my_adc_linein_demo_output(void *priv, s16 *data, int len)
{
    struct audio_adc_hdl *hdl = priv;

    //？？？不知道为什么，linein可以设置左声道的声音或者右声道的声音
    //？？？但是设置双声道时，只有左声道的声音

    // printf("%d %d",data[0], data[1]);   //打印每次linein中断数据的前两个点
    u16 points = len >> 1;      //数据点数
    //将数据变成双声道数据
    for (int i = 0; i < points; i++) {
        dual_ch_buf[2 * i] = data[i];
        dual_ch_buf[2 * i + 1] = data[i];
    }

    int wlen = app_audio_output_write(dual_ch_buf,len*2);   //把linein数据输出到dac播放

    if (wlen != len*2) {
        printf("wlen:%d-%d",wlen,len*2);
    }


}


void my_audio_adc_linein_demo(void)
{
    u16 ladc_linein_sr = 44100;
    r_printf("audio_adc_linein_demo...");
    ladc_linein = zalloc(sizeof(*ladc_linein));
    if (ladc_linein) {
        audio_adc_linein_open(&ladc_linein->ch, AUDIO_ADC_LINE0_LR, &adc_hdl);
        audio_adc_linein_set_sample_rate(&ladc_linein->ch, ladc_linein_sr);
        audio_adc_linein_set_gain(&ladc_linein->ch, 5);
        printf("adc_buf_size:%d", sizeof(ladc_linein->adc_buf));
        audio_adc_set_buffs(&ladc_linein->ch, ladc_linein->adc_buf, LADC_LINEIN_CH_NUM * LADC_LINEIN_IRQ_POINTS * 2, LADC_LINEIN_BUF_NUM);
        ladc_linein->adc_output.handler = my_adc_linein_demo_output;
        ladc_linein->adc_output.priv = &adc_hdl;
        audio_adc_add_output_handler(&adc_hdl, &ladc_linein->adc_output);
        audio_adc_linein_start(&ladc_linein->ch);

        app_audio_output_samplerate_set(ladc_linein_sr);
        app_audio_output_start();
    }
}


void my_audio_adc_linein_exit(void)
{
    //printf("ladc_mic_close\n");
    if (ladc_linein) {
        audio_adc_del_output_handler(&adc_hdl, &ladc_linein->adc_output);
        audio_adc_mic_close(&ladc_linein->ch);
        free(ladc_linein);
        ladc_linein = NULL;
    }
}