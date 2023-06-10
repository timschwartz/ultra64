#!/bin/bash

aclocal -I./m4 \
&& libtoolize -c \
&& autoheader \
&& automake --add-missing -c \
&& autoconf
