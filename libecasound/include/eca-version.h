#ifndef _ECA_VERSION_H
#define _ECA_VERSION_H

#include <string>

/**
 * Ecasound library version as a formatted string.
 *
 * "vx.y.zRt" :
 *
 * x = major version  - the overall development status
 *
 * y = devel-series   - represents a set of planned features
 *
 * z = minor version  - version number of the current development series
 *
 * Rt = 'rx'          - stable release number x
 *
 * Rt = 'dx'  	     - development version
 */
extern const string ecasound_library_version;

/**
 * Ecasound library libtool version number (current:revision:age)
 */
extern const long int ecasound_library_version_current;
extern const long int ecasound_library_version_revision;
extern const long int ecasound_library_version_age;

#endif
