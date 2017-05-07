# Compile and run guide

### GCC compilation flags

* `-m32`: Compile as 32-bit executable
* `-fno-stack-protector`: No stack canaries

### Enabling/Disabling ASLR

Non-persistent:
```
# echo 2 > /proc/sys/kernel/randomize_va_space # Enabling
# echo 0 > /proc/sys/kernel/randomize_va_space # Disabling
```

Persistent:
```
# echo 'kernel.randomize_va_space = 2' > /etc/sysctl.d/01-disable-aslr.conf # Enabling
# echo 'kernel.randomize_va_space = 0' > /etc/sysctl.d/01-disable-aslr.conf # Disabling
```

### Socat

Run a local executable as a network service:
```
socat -v tcp-l:1337,fork exec:'./program'
```

### Protecting against changes with chattr

```
chattr +i /etc/passwd
```

etc...

### Xinetd

`man xinetd.conf` is your friend. :)

For example: (remove the comments)
```
service ctf_challenge
{
        disable         = no # Enable the service, nice to control in-CTF new challenges
        type            = UNLISTED # It's not a standard service from /etc/services or RPC
        id              = ctf_challenge_uniq_id # Our ID...
        socket_type     = stream # TCP...
        protocol        = tcp # TCP...
        user            = ctf_chall_user # Our user which will run the executable
        wait            = no # If yes, then only allow 1 connection and let everyone else wait.
}
```
Source: https://en.wikipedia.org/wiki/Xinetd

If you want to chroot:
```
	server      = /usr/sbin/chroot
	server_args = /home/ctf_chall_user /home/ctf_chall_user/ctf_challenge
```
Source: https://github.com/fuzyll/libctf

Using a small bash wrapper around your executable:
```
#!/bin/bash

dir=$(dirname $0)
cd $dir; ulimit -u 200
exec timeout -s9 5m /full/path/to/executable
```
Source: Kudos to picoCTF people from which I got this little bash script. :)
