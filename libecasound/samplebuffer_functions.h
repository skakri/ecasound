#ifndef INCLUDED_SAMPLEBUFFER_FUNCTIONS_H
#define INCLUDED_SAMPLEBUFFER_FUNCTIONS_H

#include <cmath>
#include "samplebuffer.h"

/**
 * Various simple functions that operate on sample buffer data. This 
 * class really is just an extension of class SAMPLE_BUFFER.
 */
class SAMPLE_BUFFER_FUNCTIONS {

 public:

  typedef SAMPLE_BUFFER::sample_t sample_t;

  static sample_t max_value(const SAMPLE_BUFFER& buf, 
			       SAMPLE_BUFFER::channel_size_t channel) {
    sample_t t = SAMPLE_SPECS::impl_min_value;
    for(SAMPLE_BUFFER::buf_size_t m = 0; m < buf.buffersize_rep; m++) {
      if (buf.buffer[channel][m] > t) t = buf.buffer[channel][m];
    }
    return(t);
  }

  static sample_t min_value(const SAMPLE_BUFFER& buf, 
			SAMPLE_BUFFER::channel_size_t channel) {
    sample_t t = SAMPLE_SPECS::impl_max_value;
    for(SAMPLE_BUFFER::buf_size_t m = 0; m < buf.buffersize_rep; m++) {
      if (buf.buffer[channel][m] < t) t = buf.buffer[channel][m];
    }
    return(t);
  }

  static sample_t average_amplitude(const SAMPLE_BUFFER& buf) {
    sample_t temp_avg = 0.0;
    for(int n = 0; n < buf.channel_count_rep; n++) {
      for(SAMPLE_BUFFER::buf_size_t m = 0; m < buf.buffersize_rep; m++) {
	temp_avg += fabs(buf.buffer[n][m] - SAMPLE_SPECS::silent_value);
      }
    }
    return(temp_avg / buf.channel_count_rep / buf.buffersize_rep);
  }

  static sample_t RMS_volume(const SAMPLE_BUFFER& buf) {
    sample_t temp_avg = 0.0;
    for(int n = 0; n < buf.channel_count_rep; n++) {
      for(SAMPLE_BUFFER::buf_size_t m = 0; m < buf.buffersize_rep; m++) {
	temp_avg += buf.buffer[n][m] * buf.buffer[n][m];
      }
    }
    return(sqrt(temp_avg / buf.channel_count_rep / buf.buffersize_rep));
  }

  static sample_t average_amplitude(const SAMPLE_BUFFER& buf,
				       SAMPLE_BUFFER::channel_size_t channel,
				       SAMPLE_BUFFER::buf_size_t count_samples) {
      sample_t temp_avg = 0.0;
      if (count_samples == 0) count_samples = static_cast<int>(buf.channel_count_rep);
      
      for(SAMPLE_BUFFER::buf_size_t n = 0; n < buf.buffersize_rep; n++) {
	temp_avg += fabs(buf.buffer[channel][n] - SAMPLE_SPECS::silent_value);
      }
      
      return(temp_avg / count_samples);
  }
  
  static sample_t RMS_volume(const SAMPLE_BUFFER& buf,
				SAMPLE_BUFFER::channel_size_t channel,
				SAMPLE_BUFFER::buf_size_t count_samples) {
    sample_t temp_avg = 0.0;
    if (count_samples == 0) count_samples = static_cast<int>(buf.channel_count_rep);
    for(SAMPLE_BUFFER::buf_size_t n = 0; n < buf.buffersize_rep; n++) {
      temp_avg += buf.buffer[channel][n] * buf.buffer[channel][n];
    }
    return(sqrt(temp_avg / count_samples));
  }
};

#endif
