Hi ! Welcome to this two student project !

The goal of this project is to infect an ELF binary and make the infected binary able to infect other one.
To make the life of virus analysts a little bite harder, the virus is encrypted and change itself to prevent the use of fingerprint to find it and the infection is not run if a debugger is attached.
The virus can also execute a string in a TXT DNS record as an easy way to create a back door. The remote connection is crypt using symmetric hard coded key (I know, this is not secure, but better than noting).

## Run our virus

> make && ./Death

All ELF in /tmp/test/ and /tmp/test2 should be infected !
