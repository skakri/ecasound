Buildroot: /tmp/ecasound-build
Prefix: /usr
Packager: Kai Vehmanen <kaiv@wakkanet.fi>
Distribution: Red Hat Contrib
Name: ecasound
Version: 1.8.5d15
Release: 1
Copyright: GPL
Source: http://ecasound.seul.org/download/ecasound-1.8.5d15.tar.gz

Summary: ecasound - multitrack audio processing tool
Group: Applications/Sound

%description
Ecasound is a software package designed for multitrack audio
processing. It can be used for simple tasks like audio playback, 
recording and format conversions, as well as for multitrack effect 
processing, mixing, recording and signal recycling. Ecasound supports 
a wide range of audio inputs, outputs and effect algorithms. 
Effects and audio objects can be combined in various ways, and their
parameters can be controlled by operator objects like oscillators 
and MIDI-CCs. As most functionality is located in shared libraries,
creating alternative user-interfaces is easy. A versatile console mode
interface is included in the package.

%package devel
Summary: Ecasound - Library header files
Group: Applications/Sound
Requires: ecasound

%description devel
Headers files needed for compiling other programs against
ecasound libraries. This is package is not required for installing 
other ecasound RPMs.

%package plugins
Summary: Additional ecasound plugins (ALSA, Audiofile, aRts).
Group: Applications/Sound
AutoReqProv: no
Requires: ecasound

%description plugins
Additional ecasound plugins (ALSA, Audiofile, aRts).

%prep
%setup -n ecasound-1.8.5d15
%build
./configure --prefix=%prefix --disable-static
make

%install
make DESTDIR=$RPM_BUILD_ROOT%prefix install-strip
make DESTDIR=$RPM_BUILD_ROOT%prefix delete-static-libs
make DESTDIR=$RPM_BUILD_ROOT%prefix strip-shared-libs

%files
%defattr(-, root, root)
%doc NEWS README INSTALL BUGS Documentation
%doc /usr/man/man1/ecasound.1
%doc /usr/man/man1/ecatools.1
%doc /usr/man/man1/ecasound-iam.1
%doc /usr/man/man5/ecasoundrc.5
/usr/bin/ecasound
/usr/bin/ecaconvert
/usr/bin/ecafixdc
/usr/bin/ecanormalize
/usr/bin/ecaplay
/usr/bin/ecasignalview
/usr/bin/ecasound-config
/usr/lib/libecasound.*
/usr/lib/libkvutils.*
%config /usr/share/ecasound

%files devel
%defattr(-, root, root)
/usr/include/ecasound
/usr/include/kvutils

%files plugins
%defattr(-, root, root)
/usr/lib/ecasound-plugins

%changelog
* Sat Nov 25 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- ecasignalview added to the package.

* Thu Aug 31 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- 'ecasound-config' script added.
- All Qt-related stuff removed.
- Added DESTDIR to %install.

* Wed Jul 06 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- Added the -plugins package.

* Wed Jun 07 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- ecaconvert added to the package.

* Mon Jun 05 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- Renamed ecatools programs.

* Thu May 02 2000 Marc Lavall�e <odradek@videotron.ca>
- Adapted the spec for Mandrake.

* Mon Apr 15 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- Removed dynamic linking to ALSA libraries. You 
  can get ALSA support by recompiling the source-RPM
  package.

* Mon Feb 10 2000 Kai Vehmanen <kaiv@wakkanet.fi>
- Added libqtecasound to ecasound-qt.

* Mon Nov 09 1999 Kai Vehmanen <kaiv@wakkanet.fi>
- A complete reorganization. Ecasound distribution is now 
  divided to three RPMs: ecasound, ecasound-qt and ecasound-devel.

* Mon Nov 08 1999 Kai Vehmanen <kaiv@wakkanet.fi>
- As Redhat stopped the RHCN project, so these rpms 
  are again distributed via Redhat's contrib service
- You can also get these from http://ecasound.seul.org/download

* Sun Aug 15 1999 Kai Vehmanen <kaiv@wakkanet.fi>
- Initial rhcn release.