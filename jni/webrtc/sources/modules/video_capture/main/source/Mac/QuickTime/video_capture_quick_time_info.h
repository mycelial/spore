/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

/*
 *  video_capture_quick_time_info.h
 *
 */

#ifndef WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_MAC_QUICKTIME_VIDEO_CAPTURE_QUICK_TIME_INFO_H_
#define WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_MAC_QUICKTIME_VIDEO_CAPTURE_QUICK_TIME_INFO_H_

#include <QuickTime/QuickTime.h>

#include "../../video_capture_impl.h"
#include "../../device_info_impl.h"
#include "list_wrapper.h"
#include "map_wrapper.h"

class VideoRenderCallback;

namespace webrtc
{
class CriticalSectionWrapper;
class EventWrapper;
class ThreadWrapper;
class Trace;

class VideoCaptureMacQuickTimeInfo: public DeviceInfoImpl
{
public:

    static DeviceInfo* Create(const WebRtc_Word32 id);
    static void Destroy(DeviceInfo* deviceInfo);

    VideoCaptureMacQuickTimeInfo(const WebRtc_Word32 id);
    virtual ~VideoCaptureMacQuickTimeInfo();

    WebRtc_Word32 Init();

    virtual WebRtc_UWord32 NumberOfDevices();

    /*
     * Returns the available capture devices.
     * deviceNumber   -[in] index of capture device
     * deviceNameUTF8 - friendly name of the capture device
     * deviceUniqueIdUTF8 - unique name of the capture device if it exist.
     *                      Otherwise same as deviceNameUTF8
     * productUniqueIdUTF8 - unique product id if it exist. Null terminated
     *                       otherwise.
     */
    virtual WebRtc_Word32 GetDeviceName(
        WebRtc_UWord32 deviceNumber, char* deviceNameUTF8,
        WebRtc_UWord32 deviceNameLength, char* deviceUniqueIdUTF8,
        WebRtc_UWord32 deviceUniqueIdUTF8Length,
        char* productUniqueIdUTF8 = 0,
        WebRtc_UWord32 productUniqueIdUTF8Length = 0);


    // ************** The remaining public functions are not supported on Mac

    /*
     *   Returns the number of capabilities for this device
     */
    virtual WebRtc_Word32 NumberOfCapabilities(const char* deviceUniqueIdUTF8);

    /*
     *   Gets the capabilities of the named device
     */
    virtual WebRtc_Word32 GetCapability(
        const char* deviceUniqueIdUTF8,
        const WebRtc_UWord32 deviceCapabilityNumber,
        VideoCaptureCapability& capability);

    /*
     *  Gets the capability that best matches the requested width, height and frame rate.
     *  Returns the deviceCapabilityNumber on success.
     */
    virtual WebRtc_Word32 GetBestMatchedCapability(
        const char* deviceUniqueIdUTF8,
        const VideoCaptureCapability& requested,
        VideoCaptureCapability& resulting);

    /*
     * Display OS /capture device specific settings dialog
     */
    virtual WebRtc_Word32 DisplayCaptureSettingsDialogBox(
        const char* deviceUniqueIdUTF8,
        const char* dialogTitleUTF8, void* parentWindow,
        WebRtc_UWord32 positionX, WebRtc_UWord32 positionY);

protected:
    virtual WebRtc_Word32 CreateCapabilityMap(
        const char* deviceUniqueIdUTF8);

private:

    struct VideoCaptureMacName
    {
        VideoCaptureMacName();

        enum
        {
            kVideoCaptureMacNameMaxSize = 64
        };
        char _name[kVideoCaptureMacNameMaxSize];
        CFIndex _size;
    };

    enum
    {
        kVideoCaptureMacDeviceListTimeout = 5000
    }; // Timeout value [ms] if we want to create a new device list or not
    enum
    {
        kYuy2_1280_1024_length = 2621440
    }; // Temporary constant allowing this size from built-in iSight webcams.

private:
    // private methods

    int GetCaptureDevices(WebRtc_UWord32 deviceNumber,
                          char* deviceNameUTF8,
                          WebRtc_UWord32 deviceNameUTF8Length,
                          char* deviceUniqueIdUTF8,
                          WebRtc_UWord32 deviceUniqueIdUTF8Length,
                          char* productUniqueIdUTF8,
                          WebRtc_UWord32 productUniqueIdUTF8Length,
                          int& numberOfDevices);

    static CFIndex PascalStringToCString(const unsigned char* pascalString,
                                         char* cString, CFIndex bufferSize);

private:
    // member vars
    WebRtc_Word32 _id;
    bool _terminated;
    CriticalSectionWrapper* _grabberCritsect;
    webrtc::Trace* _trace;
    webrtc::ThreadWrapper* _grabberUpdateThread;
    webrtc::EventWrapper* _grabberUpdateEvent;
    SeqGrabComponent _captureGrabber;
    Component _captureDevice;
    char _captureDeviceDisplayName[64];
    bool _captureIsInitialized;
    GWorldPtr _gWorld;
    SGChannel _captureChannel;
    ImageSequence _captureSequence;
    bool _sgPrepared;
    bool _sgStarted;
    int _codecWidth;
    int _codecHeight;
    int _trueCaptureWidth;
    int _trueCaptureHeight;
    ListWrapper _captureDeviceList;
    WebRtc_Word64 _captureDeviceListTime;
    ListWrapper _captureCapabilityList;
};
}  // namespace webrtc
#endif  // WEBRTC_MODULES_VIDEO_CAPTURE_MAIN_SOURCE_MAC_QUICKTIME_VIDEO_CAPTURE_QUICK_TIME_INFO_H_
