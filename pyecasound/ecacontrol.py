# coding=utf-8
"""Native python ECI (ecasound control interface) implementation

   Can be used to replace the C implementation 'pyecasound.so'.
"""

from __future__ import print_function
import sys
import re
import subprocess
from select import select
import os
import signal
import time

authors = """Kai Vehmanen, Eric S. Tiedemann and Janne Halttunen."""

if sys.hexversion < 0x02040000:
    print(
        "ERROR: Python 2.4 or newer is required by ecacontrol.py",
        file=sys.stderr
    )
    sys.exit(-1)

_ecasound = []

type_override = {}
eci_str_sync_lost = 'Connection to the processing engine was lost.\n'


class ECA_CONTROL_INTERFACE:
    def __init__(self, verbose=1):
        """Instantiate new ECI session
        
        verbose: set this false to get rid of startup-messages
        """

        self.verbose = verbose
        self._cmd = ''
        self._type = ''
        self._timeout = 5  # in seconds
        self._resp = {}
        self.initialize()
        
    def __call__(self, cmd, f=None):
        if f:
            val = self.command_float_arg(cmd, f)
        else:
            cmds = cmd.split('\n')
            if len(cmds) > 1:
                v = []
                for c in cmds:
                    c = c.strip()
                    if c:
                        v.append(self.command(c))
                    
                        if self.error():
                            raise Exception(v[-1])

                val = str.join('\n', map(str, v))
            else:
                val = self.command(cmd)
                    
        if self.error():
            raise Exception(val)
        
        return val            

    def _readline(self):
        return self.eca.stdout.readline().strip().decode('utf-8')
        
    def _read_eca(self):
        buffer = ''
        while select([self.eca.stdout.fileno()], [], [self.eca.stdout.fileno()], 0.01)[0]:
            buffer += self.eca.stdout.read(1).decode('utf-8')
        return buffer
    
    def _parse_response(self):
        tm = ''
        r = ()
        fail_count = 0

        if self.verbose > 2:
            print('c=%s' % self._cmd)

        while 1:
            s = self._read_eca()
            # print('read s=%s' % s)
            if s:
                if self.verbose > 3:
                    print('s=<%s>' % s)
            else:
                fail_count += 1
                if fail_count < self._timeout * 10:
                    # if failcount < 0:
                    time.sleep(0.01)
                    continue
                else:
                    print('timeout: s=<%s>, cmd=%s.' % (s, self._cmd))
                    r = ('e', eci_str_sync_lost)
                    break
            tm += s
            m = expand_eiam_response(tm)
            r = parse_eiam_response(tm, m)

            if r:
                if self.verbose > 2:
                    print('r=%s' % r)
                break

        if not r:
            self._resp['e'] = '-'
            self._type = 'e'
            r = None
        else:
            self._type = r[0]
            
            if self._cmd in type_override.keys():
                self._type = type_override[self._cmd]
            
            if self._type == 'S':
                self._resp[self._type] = r[1].split(',')
            elif self._type == 'Sn':
                self._resp[self._type] = r[1].split('\n')
            elif self._type == 'f':
                self._resp[self._type] = float(r[1])
            elif self._type == 'i':
                self._resp[self._type] = int(r[1])
            elif self._type == 'li':
                # Python 2 will correctly cast to long;
                # in Python 3 there is only one integer type -- int.
                self._resp[self._type] = int(r[1])
            else:
                self._resp[self._type] = r[1]

        return self._resp[self._type]

    def initialize(self):
        """Reserve resources"""
                
        # if_ecasound is not None:
        #     self.cleanup()  # exit previous ecasound session cleanly
           
        global _ecasound

        ecasound_binary = os.environ.get('ECASOUND', 'ecasound')

        p = subprocess.Popen(ecasound_binary + ' -c -d:256 2>/dev/null',
                             shell=True, bufsize=0, stdin=subprocess.PIPE, stdout=subprocess.PIPE, close_fds=True)
        _ecasound.append(p)
        
        self.eca = _ecasound[-1]
        
        lines = ''
        
        lines = lines + self._readline() + '\n'

        version = self._readline()
            
        s = version.find('ecasound v')

        if float(version[s+10:s+13]) >= 2.2:
            lines = lines + version+'\n'
        else:
            raise RuntimeError('ecasound version 2.2 required!')
        
        lines = lines + self._readline() + '\n'
        
        if self.verbose:
            print(lines)
            print(__doc__)
            print('by %s' % authors)
            print('\n(to get rid of this message, pass zero to instance init)')
            
        self.command('int-output-mode-wellformed')
        # self._read_eca()
        # self.command('debug 256')
        
    def cleanup(self):
        """Free all reserved resources"""
        
        self.eca.stdin.write('quit\n'.encode('utf-8'))

        os.kill(self.eca.pid, signal.SIGTERM)
                
        signal.signal(signal.SIGALRM, handler)
        signal.alarm(2)
        
        try:
            return self.eca.wait()
        except:
            pass
        
        signal.alarm(0)
        os.kill(self.eca.pid, signal.SIGKILL)

    def command(self, cmd):
        """Issue an EIAM command"""
        
        cmd = cmd.strip()

        if cmd:
            self._cmd = cmd
            cmd += '\n'

            self.eca.stdin.write(cmd.encode('utf-8'))
            return self._parse_response()
        
    def command_float_arg(self, cmd, f=None):
        """Issue an EIAM command
        
        This function can be used instead of command(string), 
        if the command in question requires exactly one numerical parameter."""
        
        cmd = cmd.strip()

        if cmd:
            self._cmd = cmd
            if f:
                cmd = '%s %f\n' % (cmd, f)
            else:
                cmd += '\n'

            self.eca.stdin.write(cmd.encode('utf-8'))

            return self._parse_response()
            
    def error(self):
        """Return true if error has occured during the execution of last EIAM command"""
        return self._type == 'e'
        
    def last_error(self):
        """Return a string describing the last error"""
        
        if self.error():
            return self._resp.get('e')
        else: 
            return ''
        
    def last_float(self):
        """Return the last floating-point return value"""
        return self._resp.get('f')
    
    def last_integer(self):
        """Return the last integer return value
        
        This function is also used to return boolean values."""
        return self._resp.get('i')
    
    def last_long_integer(self):
        """Return the last long integer return value
        
        Long integers are used to pass values like 'length_in_samples' 
        and 'length_in_bytes'.  It's implementation specific whether there's 
        any real difference between integers and long integers."""
        return self._resp.get('li')
    
    def last_string(self):
        """Return the last string return value"""
        return self._resp.get('s')
    
    def last_string_list(self):
        """Return the last collection of strings (one or more strings)"""
        return self._resp.get('S')
    
    def last_type(self):
        return self._type
    
    def current_event(self):
        """** not implemented **"""

    def events_available(self):
        """** not implemented **"""

    def next_event(self):
        """** not implemented **"""


def handler(*args):
    print('AARGH!')
    raise Exception('killing me not so softly')

expand = re.compile('256 ([0-9]{1,5}) (.+)\r\n(.*)\r\n\r\n.*', re.MULTILINE | re.S)


def expand_eiam_response(st):
    """Checks wheter 'str' is a valid EIAM response.

    @return Regex match object.
    """

    m = expand.search(st)
    return m

parse = re.compile('256 ([0-9]{1,5}) (.+)\r\n(.*)', re.MULTILINE | re.S)


def parse_eiam_response(st, m=None):
    """Parses a valid EIAM response.

    @param m Valid regex match object.
    @param str The whole EIAM response.

    @return tuple of return value type and value
    """

    if not m:
        m = parse.search(st)
        if not m:
            return ()

    if m and len(m.groups()) == 0:
        # print("(pyeca) Matching groups failed: %s" % str(m.groups()))
        return 'e', 'Matching groups failed'

    if m and len(m.groups()) == 3:
        # print('received=%s, expected=%s' % (len(m.group(3)), m.group(1)))
        if int(m.group(1)) != len(m.group(3)):
            print('(pyeca) Response length error. Received %s, expected for %s.' % (len(m.group(3)), m.group(1)))
            # print('g=%s' % m.group(3))
            return 'e', 'Response length error.'

    if m:
        return (m.group(2), m.group(3))

    return 'e',''


class base:
    def __init__(self, eci, cmd):
        self.eci = eci
        self.cmd = cmd.replace('_', '-')

    def __call__(self, *args):
        return self.eci(self.cmd)


class string_argument(base):
    def __call__(self, s):
        return self.eci('%s %s' % (self.cmd, s))


class EIAM:
    def __init__(self, verbose=0):
        self._eci = ECA_CONTROL_INTERFACE(verbose)
        self._cmds = self._eci('int-cmd-list')

        for c in self._cmds:
            c = c.replace('-', '_')
            if c.count('add') or c.count('select'):
                self.__dict__[c] = string_argument(self._eci, c)
            else:
                self.__dict__[c] = base(self._eci, c)


def main():
    e = ECA_CONTROL_INTERFACE()
    print(e.command('c-add huppaa'))
    print(e.command('c-list'))

    print(
        e("""

        c-list
        c-status
        """)
    )

    print(e.cleanup())

if __name__ == '__main__':
    main()
