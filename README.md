# SDL_TestConsole
A windows console to test SDL apis.  
SDL3-3.1.3 included.  
To test ffmpeg, you need to install ffmpeg-7.1-full_build-shared.  

**How to run?**
1. download SDL source codes (https://github.com/libsdl-org/SDL) if you like
2. download one of SDL releases like SDL3-devel-3.1.3-VC.zip (from https://github.com/libsdl-org/SDL/releases)
3. configure include and lib paths
- .\SDL3-3.1.3\includ
- .\SDL3-3.1.3\lib\
- D:\SDKs\ffmpeg-7.1-full_build-shared\include
- D:\SDKs\ffmpeg-7.1-full_build-shared\lib
4. linked to SDL libs: SDL3.lib
5. linked to SDL test framework libs: SDL3_test.lib
6. [TBD] linked to FFmpeg libs: avcodec.lib;avdevice.lib;avformat.lib;avutil.lib;swscale.lib;swresample.lib;
