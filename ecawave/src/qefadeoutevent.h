#ifndef QEFADEOUTEVENT_H
#define QEFADEOUTEVENT_H

#include <string>
#include "qeblockingevent.h"

/**
 * Fade-out effect
 */
class QEFadeOutEvent : public QEBlockingEvent {

 public:

  QEFadeOutEvent(ECA_CONTROLLER* ctrl,
		 const string& input,
		 const string& output,
		 long int insert_pos,
		 long int insert_length); 

 private:

  ECA_CONTROLLER* ectrl;
};

#endif
