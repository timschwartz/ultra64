#!/bin/bash

aclocal \
&& autoheader \
&& automake --add-missing -c \
&& autoconf
