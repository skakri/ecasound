#ifndef INCLUDED_ECA_VERSION_H
#define INCLUDED_ECA_VERSION_H

/**
 * Ecasound library version as a formatted std::string.
 *
 * "vX.Y[.Z][devT]" :
 *
 * x = major version  - the overall development status
 *
 * y = minor version  - represents a set of planned features (see TODO)
 *
 * z = revision       - version number of the current development series
 *
 * devT = dev-release - development releases leading to the stable
 * 		        release X.Y[.Z]
 */
extern const char* ecasound_library_version;

/**
 * Ecasound library libtool version number (current:revision:age)
 */
extern const long int ecasound_library_version_current;
extern const long int ecasound_library_version_revision;
extern const long int ecasound_library_version_age;

#endif
