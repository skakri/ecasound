#ifndef INCLUDED_AUDIOIO_JACK_MANAGER_H
#define INCLUDED_AUDIOIO_JACK_MANAGER_H

#include <map>
#include <list>
#include <string>
#include <vector>

#include <pthread.h>
#include <jack/jack.h>

#include "sample-specs.h"
#include "dynamic-object.h"
#include "audioio-manager.h"
#include "eca-engine-driver.h"
#include "audioio_jack.h"

class AUDIO_IO;

using std::map;
using std::list;
using std::string;
using std::vector;

/**
 * Manager class for JACK client objects.
 *
 * Related design patterns:
 *     - Mediator (GoF273)
 *
 * @author Kai Vehmanen
 */
class AUDIO_IO_JACK_MANAGER : public AUDIO_IO_MANAGER, 
			      public ECA_ENGINE_DRIVER {

 public:

  friend int eca_jack_process(jack_nframes_t nframes, void *arg);
  friend int eca_jack_bufsize (jack_nframes_t nframes, void *arg);
  friend int eca_jack_srate (jack_nframes_t nframes, void *arg);
  friend void eca_jack_shutdown (void *arg);

  static const int instance_limit;

public:

  typedef struct jack_node {
    AUDIO_IO_JACK* aobj;
    AUDIO_IO* origptr;
    int in_ports;
    int first_in_port;
    int out_ports;
    int first_out_port;
  } jack_node_t;

  typedef struct jack_port_data {
    jack_port_t* jackport;
    string autoconnect;
    bool autoconnect_addprefix;
    jack_nframes_t total_latency;
    jack_default_audio_sample_t* cb_buffer;
  } jack_port_data_t;

 private:

  typedef enum Operation_mode {
    Streaming,
    Master,
    Slave
  } Operation_mode_t;

 public:

  /** @name Constructors */
  /*@{*/

  AUDIO_IO_JACK_MANAGER(void);
  virtual ~AUDIO_IO_JACK_MANAGER(void);

  /*@}*/

  /** @name Functions reimplemented from AUDIO_IO_MANAGER */
  /*@{*/

  virtual bool is_managed_type(const AUDIO_IO* aobj) const;
  virtual void register_object(AUDIO_IO* aobj);
  virtual int get_object_id(const AUDIO_IO* aobj) const;
  virtual const list<int>& get_object_list(void) const;
  virtual void unregister_object(int id);

  /*@}*/

  /** @name Functions reimplemented from ECA_OBJECT */
  /*@{*/

  virtual string name(void) const { return("jack"); }
  virtual string description(void) const { return("JACK object manager"); }

  /*@}*/

  /** @name Function reimplemented from DYNAMIC_PARAMETERS */
  /*@{*/

  virtual std::string parameter_names(void) const { return("mode"); }
  virtual void set_parameter(int param, std::string value);
  virtual std::string get_parameter(int param) const;

  /*@}*/

  /** @name Function reimplemented from DYNAMIC_OBJECT */
  /*@{*/

  AUDIO_IO_JACK_MANAGER* clone(void) const { return new_expr(); }
  AUDIO_IO_JACK_MANAGER* new_expr(void) const { return new AUDIO_IO_JACK_MANAGER(); }  

  /*@}*/


  /** @name Functions reimplemented from ECA_ENGINE_DRIVER */
  /*@{*/

  virtual void exec(ECA_ENGINE* engine, ECA_CHAINSETUP* csetup);
  virtual void start(void);
  virtual void stop(void);
  virtual void exit(void);

  /*@}*/

  /** @name Public API for JACK clients */
  /*@{*/

  void register_jack_ports(int client_id, int ports, const string& portprefix);
  void unregister_jack_ports(int client_id);
  void auto_connect_jack_port(int client_id, int portnum, const string& portname);

  long int client_latency(int client_id);

  void open(int client_id);
  void close(int client_id);
  
  long int read_samples(int client_id, void* target_buffer, long int samples);
  void write_samples(int client_id, void* target_buffer, long int samples);

  bool is_open(void) const { return(open_rep); }
  bool is_connection_active(void) const { return(connection_active_rep); }

  long int buffersize(void) const;
  SAMPLE_SPECS::sample_rate_t samples_per_second(void) const;

  /*@}*/

private:

  static void get_total_port_latency(jack_client_t* client, jack_port_data_t* ports);

  void open_connection(void);
  void close_connection(void);
  void stop_connection(void);

  void set_node_connection(jack_node_t* node, bool connect);
  void connect_node(jack_node_t* node);
  void disconnect_node(jack_node_t* node);

  void wait_for_exit(void);
  void signal_exit(void);
  void wait_for_stop(void);
  void signal_stop(void);

  pthread_cond_t exit_cond_rep;
  pthread_mutex_t exit_mutex_rep;
  pthread_cond_t stop_cond_rep;
  pthread_mutex_t stop_mutex_rep;
  pthread_mutex_t lock_rep;

  Operation_mode_t mode_rep;

  int total_nodes_rep;
  int active_nodes_rep;

  int last_in_port_rep;
  int last_out_port_rep;

  bool open_rep;
  bool connection_active_rep;

  bool shutdown_request_rep;
  bool exit_request_rep;

  int last_id_rep;
  list<int> objlist_rep;
  map<int,jack_node_t*> jacknodemap_rep;

  ECA_ENGINE* engine_repp;

  jack_client_t *client_repp;
  string jackname_rep;

  vector<jack_port_data_t> inports_rep;
  vector<jack_port_data_t> outports_rep;

  SAMPLE_SPECS::sample_rate_t srate_rep;
  long int buffersize_rep;
  long int cb_allocated_frames_rep;
};

#endif
