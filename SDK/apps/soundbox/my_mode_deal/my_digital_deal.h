#ifndef _MY_DIGITAL_DEAL_H__
#define _MY_DIGITAL_DEAL_H__

extern int __my_audio_digital_vol_run(void *data, u32 len, u8 right_left);
//extern s16 *output_buf;


int my_audio_digital_Channel_merging_add(void *data, u32 len, u8 right_left);
int my_audio_digital_vol_run(void *data, u32 len, u8 right_left, u8 l_vol, u8 r_vol);
int my_audio_digital_Channel_merging_swap(void *data, u32 len, u8 right_left);
int my_audio_digital_del_one_channel(void *data, u32 len, u8 right_left);
int my_audio_digital_one_to_two_channel(void *data, u32 len , s16 *output_buf);

int my_audio_change_output_sample_rate_init(u16 input_rate, u16 output_rate);
int my_audio_change_sample_rate_output(s16 *data, u16 len, s16 *output_buf);

//same 是 data 跟 outdata 是不是同一片地址，是为 1，不是为 0
extern void REAL_FFT_FUN(int *data,int blockbit,int *outdata,int same); //实数 fft
extern void REAL_IFFT_FUN(int *data,int blockbit,int *outdata,int same); //实数 ifft

#endif