#ifndef _AUDIOIO_RAW_H
#define _AUDIOIO_RAW_H

#include <string>

#include "eca-fileio.h"
#include "eca-fileio-mmap.h"
#include "eca-fileio-stream.h"
#include "samplebuffer.h"

/**
 * Class for handling raw/headerless audio files
 *
 * - multiple channels are interleaved, left channel first
 *
 * @author Kai Vehmanen
 */
class RAWFILE : public AUDIO_IO_BUFFERED {

  ECA_FILE_IO* fio_repp;
  string mmaptoggle_rep;

  RAWFILE(const RAWFILE& x) { }
  RAWFILE& operator=(const RAWFILE& x) { return *this; }

  void set_length_in_bytes(void);

  /**
   * Do a info query prior to actually opening the device.
   *
   * require:
   *  !is_open()
   *
   * ensure:
   *  !is_open()
   */
  void format_query(void);

 public:

  virtual string name(void) const { return("Raw audio file"); }
  virtual string parameter_names(void) const { return("label,toggle_mmap"); }
  
  virtual void open(void);
  virtual void close(void);

  virtual long int read_samples(void* target_buffer, long int samples);
  virtual void write_samples(void* target_buffer, long int samples);

  virtual bool finished(void) const;
  virtual void seek_position(void);

  virtual void set_parameter(int param, string value);
  virtual string get_parameter(int param) const;

  RAWFILE (const string& name = "");
  ~RAWFILE(void);
    
  RAWFILE* clone(void) { cerr << "Not implemented!" << endl;  return this; }    
  RAWFILE* new_expr(void) { return new RAWFILE(); }    
};

#endif
