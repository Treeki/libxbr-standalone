Standalone XBR/hqx Library
==========================

This library implements the xBR pixel art scaling filter developed by Hyllian,
and now also the hqx filter developed by Maxim Stepin.

I needed a standalone implementation, but could not find any ready-to-use ones.
I found versions of the algorithm used in various open-source projects and I
eventually settled on a modified version of the one used in FFmpeg's
libavfilter.

I also wanted to use hqx and needed to make some changes to the code, so I
wound up merging this into the same codebase/library. This project is the
result.

Original source for the xBR implementation: http://git.videolan.org/gitweb.cgi/ffmpeg.git/?p=ffmpeg.git;a=blob;f=libavfilter/vf_xbr.c;h=5c14565b3a03f66f1e0296623dc91373aeac1ed0;hb=HEAD

Original source for the hqx implementation: https://code.google.com/p/hqx/

This version is a pure C library that operates on 32-bit ARGB data. It scales
images up to 2x, 3x and 4x sizes, and preserves and interpolates the alpha
channel as necessary.

Project files included to compile as a DLL with Visual Studio 2012. Code
should also compile successfully under GCC on other platforms, and probably
with other Visual Studio versions.

A crude example application is included that uses libpng to apply the
available filters to 32-bit images.

Licensed under the GNU Lesser General Public License, version 2.1. (See the
header in `xbr.h` and `xbr.c`.)

