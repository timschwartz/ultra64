#!/bin/bash

aclocal -I./m4 \
&& autoheader \
&& automake --add-missing -c \
&& autoconf
