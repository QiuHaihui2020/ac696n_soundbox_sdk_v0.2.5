#ifndef _MY_AUDIO_DEV_H__
#define _MY_AUDIO_DEV_H__

#define AUDIO_DEV_LOG_ENABLE
#ifdef AUDIO_DEV_LOG_ENABLE
#define AUDIO_DEV_LOG	y_printf
#else
#define AUDIO_DEV_LOG(...)
#endif/*AUDIO_DEV_LOG_ENABLE*/


#define MY_CBUFFER_SIZE 1024
struct my_usr_cbuffer
{
    cbuffer_t my_adc_to_dac_cbuf;
    s16 my_tmp_buffer[MY_CBUFFER_SIZE];
};

extern struct my_usr_cbuffer *my_mic_cbuffer;
extern struct my_usr_cbuffer *my_linein_cbuffer;

extern int g726_decoder_init();
extern int g726_encoder_init();
extern int sbc_encoder_init();
extern int sbc_decoder_init();
extern int pcm_decoder_enable();
extern int audio_sbc_enc_open();
extern int audio_sbc_dec_open(void);
extern int audio_enc_open(u32 code_type);
extern int audio_dec_open(u32 code_type);
extern int platform_device_sbc_init();
extern void audio_fade_in_fade_out(u8 left_gain, u8 right_gain);
extern struct dac_platform_data dac_data;
extern struct adc_platform_data adc_data;

extern struct audio_dac_hdl dac_hdl;
extern struct audio_adc_hdl adc_hdl;

extern u8 left_vol, right_vol;


int audio_demo_init();
int my_set_pitch(unsigned int shiftv, unsigned int formant_shift);
int my_audio_adc_all_demo_close();


#endif