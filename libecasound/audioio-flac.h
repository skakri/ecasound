#ifndef INCLUDED_AUDIOIO_FLAC_H
#define INCLUDED_AUDIOIO_FLAC_H

#include <string>
#include <cstdio>
#include "audioio-buffered.h"
#include "audioio-forked-stream.h"

/**
 * Interface to FLAC decoders and encoders using UNIX pipe i/o.
 *
 * @author Kai Vehmanen
 */
class FLAC_FORKED_INTERFACE : public AUDIO_IO_BUFFERED,
			      protected AUDIO_IO_FORKED_STREAM {

 private:
  
  static std::string default_input_cmd;
  static std::string default_output_cmd;

 public:

  static void set_input_cmd(const std::string& value);
  static void set_output_cmd(const std::string& value);

 public:

  FLAC_FORKED_INTERFACE (const std::string& name = "");
  virtual ~FLAC_FORKED_INTERFACE(void);
    
  virtual FLAC_FORKED_INTERFACE* clone(void) const { return new FLAC_FORKED_INTERFACE(*this); }
  virtual FLAC_FORKED_INTERFACE* new_expr(void) const { return new FLAC_FORKED_INTERFACE(*this); }

  virtual std::string name(void) const { return("FLAC stream"); }
  virtual std::string description(void) const { return("Interface to FLAC decoders and encoders using UNIX pipe i/o."); }
  virtual std::string parameter_names(void) const { return("label"); }
  virtual bool locked_audio_format(void) const { return(true); }

  virtual int supported_io_modes(void) const { return(io_read | io_write); }
  virtual bool supports_seeking(void) const { return(false); }

  virtual void open(void) throw(AUDIO_IO::SETUP_ERROR &);
  virtual void close(void);
  
  virtual long int read_samples(void* target_buffer, long int samples);
  virtual void write_samples(void* target_buffer, long int samples);

  virtual bool finished(void) const { return(finished_rep); }
  virtual void seek_position(void);

  virtual void set_parameter(int param, std::string value);
  virtual std::string get_parameter(int param) const;

  // --
  // Realtime related functions
  // --
  
 private:

  bool triggered_rep;
  bool finished_rep;
  long int bytes_rep;
  int fd_rep;
  FILE* f1_rep;
  
  void fork_input_process(void);
  void fork_output_process(void);
};

#endif
