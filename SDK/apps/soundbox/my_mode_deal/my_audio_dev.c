#include "system/includes.h"
#include "media/includes.h"
#include "app_config.h"
#include "app_action.h"
#include "app_main.h"
#include "audio_config.h"
#include "audio_digital_vol.h"
#include "audio_reverb.h"
#include "reverb/reverb_api.h"
#include "audio_pitch.h"
#include "pitchshifter/pitchshifter_api.h"


#include "my_audio_dev.h"
#include "my_digital_deal.h"
#include "my_task.h"




static s16 * output_buf  =NULL;








/*总共使能多少个通道*/
#define LADC_CH_NUM         2
#define LADC_BUF_NUM        2
#define LADC_IRQ_POINTS     128	/*中断点数*/
#define LADC_BUFS_SIZE  (LADC_CH_NUM * LADC_BUF_NUM * LADC_IRQ_POINTS)

typedef struct {
    struct audio_adc_output_hdl output;
    struct audio_adc_ch linein_ch;
    struct adc_mic_ch mic_ch;
    s16 adc_buf[LADC_BUFS_SIZE];    //align 4Bytes
    s16 temp_buf[LADC_IRQ_POINTS];
} audio_adc_t;
static audio_adc_t *ladc_linein_mic = NULL;




/*
 * 使能2个通道,1个linein 和 1个mic：
 * 数据结构：LINL0 LINR0 MIC0 LINL1 LINR1 MIC1 LINL2 LINR2 MIC2...
 */
static void my_audio_adc3_output_demo(void *priv, s16 *data, int len)
{
    struct audio_adc_hdl *hdl = priv;
    int wlen = 0;

    putchar('3');


    //int temp = my_p_reverb_obj->func_api->run(my_p_reverb_obj->ptr, data, output_buf, len/2);

    wlen = app_audio_output_write(data, len);

    if (wlen != len ) {
        //printf("wlen:%d-%d",wlen,len * 2);
    }    

}

int my_audio_adc_open_demo(void)
{
    u16 ladc_sr = 44100;
    u8 mic_gain = 5;
    u8 linein_gain = 3;
    r_printf("audio_adc_open_demo,sr:%d,mic_gain:%d,linein_gain:%d\n", ladc_sr, mic_gain, linein_gain);
    if (ladc_linein_mic) {
        r_printf("ladc already open \n");
        return 0;
    }
    output_buf  = zalloc(LADC_BUFS_SIZE*4);
    ladc_linein_mic = zalloc(sizeof(audio_adc_t));
    // my_p_reverb_obj = zalloc(sizeof(REVERBN_API_STRUCT));
    // my_p_reverb_obj = open_reverb(NULL, ladc_sr);
    if (ladc_linein_mic) {
        audio_adc_mic_open(&ladc_linein_mic->mic_ch, AUDIO_ADC_MIC_CH, &adc_hdl);
        audio_adc_mic_set_sample_rate(&ladc_linein_mic->mic_ch, ladc_sr);
        audio_adc_mic_set_gain(&ladc_linein_mic->mic_ch, mic_gain);


        audio_adc_linein_open(&ladc_linein_mic->linein_ch, AUDIO_ADC_LINE0_L, &adc_hdl);
        audio_adc_linein_set_sample_rate(&ladc_linein_mic->linein_ch, ladc_sr);
        audio_adc_linein_set_gain(&ladc_linein_mic->linein_ch, linein_gain);

        printf("adc_buf_size:%d", sizeof(ladc_linein_mic->adc_buf));
        audio_adc_set_buffs(&ladc_linein_mic->linein_ch, ladc_linein_mic->adc_buf, LADC_CH_NUM * LADC_IRQ_POINTS * 2, LADC_BUF_NUM);

        ladc_linein_mic->output.handler = my_audio_adc3_output_demo;
        ladc_linein_mic->output.priv = &adc_hdl;
        audio_adc_add_output_handler(&adc_hdl, &ladc_linein_mic->output);
        // audio_adc_start(&ladc_linein_mic->linein_ch, &ladc_linein_mic->mic_ch);
        


        app_audio_output_samplerate_set(ladc_sr);
        app_audio_output_start();


        return 0;
    } else {
        return -1;
    }
}






