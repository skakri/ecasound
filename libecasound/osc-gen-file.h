#ifndef INCLUDED_GENERIC_OSCILLATOR_FILE_H
#define INCLUDED_GENERIC_OSCILLATOR_FILE_H

#include "osc-gen.h"
#include "eca-error.h"

/**
 * Generic oscillator using preset envelopes. 
 * Presets are read from an ascii configuration file.
 */
class GENERIC_OSCILLATOR_FILE : public GENERIC_OSCILLATOR {

 public:

  static void set_preset_file(const string& fname);
  static string filename;

 private:

  int preset_rep;
  void get_oscillator_preset(int preset);

 protected:

  void parse_envelope(const string& str);
  
 public:

  virtual string parameter_names(void) const { return("freq,mode,preset-number"); }
  virtual void set_parameter(int param, parameter_type value);
  virtual parameter_type get_parameter(int param) const;
  virtual string name(void) const { return("Generic oscillator (preset)"); }

  GENERIC_OSCILLATOR_FILE* clone(void)  { return new GENERIC_OSCILLATOR_FILE(*this); }
  GENERIC_OSCILLATOR_FILE* new_expr(void)  { return new GENERIC_OSCILLATOR_FILE(*this); }
  GENERIC_OSCILLATOR_FILE (double freq = 0.0, int preset_number = 0);
  virtual ~GENERIC_OSCILLATOR_FILE (void);
};

#endif