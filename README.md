base64
======

Implementation of the Base64 encoding and decoding in C.


Installation
------------

    git clone https://github.com/ivartj/base64
    cd base64
    autoreconf --install
    ./configure
    make all install clean


Usage
-----

    base64 [ -o <output-file> ] [ -r | --reverse ] [ <input-file> ]

Be aware that the last newline in the input is also encoded, so if you may want
to use the Unix `echo` command with the `-n` option to suppress the usual last
newline.
