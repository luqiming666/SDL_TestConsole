#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int testffmpeg_main(int argc, char* argv[]); // has dependency on vulkan

int testrumble_main(int argc, char** argv);
int testhaptic_main(int argc, char** argv);

// Audio
int testresample_main(int argc, char** argv);

#ifdef __cplusplus
}
#endif

