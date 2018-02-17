#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

#if defined(__APPLE__)
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <errno.h>
#endif // defined(__APPLE__)

#include "gecko_vr.h"

#define M_PI 3.14159265358979323846264338327950288

using namespace mozilla::gfx;

int main(int argc, char **argv) {
  if (!gecko_vr_init()) {
    fprintf(stderr, "Failied to initialize Gecko VR\n");
    return EXIT_FAILURE;
  }

  VRDisplayInfo displayInfo;
  memset(&displayInfo, 0, sizeof(displayInfo));

  displayInfo.mType = VRDeviceType::External;

  strncpy(displayInfo.mDisplayName, "HelloVR HMD", kVRDisplayNameMaxLen);
  displayInfo.mIsConnected = true;
  displayInfo.mIsMounted = true;
  displayInfo.mCapabilityFlags = (VRDisplayCapabilityFlags)((int)(VRDisplayCapabilityFlags::Cap_None) |
                                  (int)VRDisplayCapabilityFlags::Cap_Orientation |
                                  (int)VRDisplayCapabilityFlags::Cap_Position |
                                  (int)VRDisplayCapabilityFlags::Cap_External |
                                  (int)VRDisplayCapabilityFlags::Cap_Present |
                                  (int)VRDisplayCapabilityFlags::Cap_StageParameters |
                                  (int)VRDisplayCapabilityFlags::Cap_MountDetection);

  displayInfo.mEyeResolution.width = 1280;
  displayInfo.mEyeResolution.height = 800;

  for (uint32_t eye = 0; eye < 2; ++eye) {
    float left, right, up, down;
    // TODO - Implement real values
    left = -0.785398f; // 45 degrees
    right = 0.785398f; // 45 degrees
    up = -0.785398f; // 45 degrees
    down = 0.785398f; // 45 degrees

    displayInfo.mEyeFOV[eye].upDegrees = atan(up) * 180.0 / M_PI;
    displayInfo.mEyeFOV[eye].rightDegrees = atan(right) * 180.0 / M_PI;
    displayInfo.mEyeFOV[eye].downDegrees = atan(down) * 180.0 / M_PI;
    displayInfo.mEyeFOV[eye].leftDegrees = atan(left) * 180.0 / M_PI;
  }

  displayInfo.mStageSize.width = 1.0f;
  displayInfo.mStageSize.height = 1.0f;

  displayInfo.mSittingToStandingTransform[0] = 1.0f;
  displayInfo.mSittingToStandingTransform[1] = 0.0f;
  displayInfo.mSittingToStandingTransform[2] = 0.0f;
  displayInfo.mSittingToStandingTransform[3] = 0.0f;

  displayInfo.mSittingToStandingTransform[4] = 0.0f;
  displayInfo.mSittingToStandingTransform[5] = 1.0f;
  displayInfo.mSittingToStandingTransform[6] = 0.0f;
  displayInfo.mSittingToStandingTransform[7] = 0.0f;

  displayInfo.mSittingToStandingTransform[8] = 0.0f;
  displayInfo.mSittingToStandingTransform[9] = 0.0f;
  displayInfo.mSittingToStandingTransform[10] = 1.0f;
  displayInfo.mSittingToStandingTransform[11] = 0.0f;

  displayInfo.mSittingToStandingTransform[12] = 0.0f;
  displayInfo.mSittingToStandingTransform[13] = 0.75f;
  displayInfo.mSittingToStandingTransform[14] = 0.0f;
  displayInfo.mSittingToStandingTransform[15] = 1.0f;

  for(int frame = 0; frame < 1000; frame++) {
    fprintf(stdout, "Frame %i\n", frame);
    displayInfo.mFrameId++;
    gecko_vr_push_state(displayInfo);
    std::this_thread::sleep_for(std::chrono::milliseconds(11));
  }

  gecko_vr_shutdown();
}
