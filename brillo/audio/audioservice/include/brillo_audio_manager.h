// Copyright 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// Class to manage audio devices in Brillo.

#ifndef BRILLO_AUDIO_AUDIOSERVICE_BRILLO_AUDIO_MANAGER_H_
#define BRILLO_AUDIO_AUDIOSERVICE_BRILLO_AUDIO_MANAGER_H_

#include <sys/cdefs.h>

#include "brillo_audio_device_info.h"

__BEGIN_DECLS

struct BAudioManager;

typedef struct BAudioManager BAudioManager;

// Get a pointer to a BAudioManager. This object will refer to the same
// underlying client object no matter how many times it is called.
//
// Returns a pointer to a BAudioManager. Returns NULL on failure.
BAudioManager* BAudioManager_new();

// Flag to get input devices.
static const int GET_DEVICES_INPUTS = 1;
// Flag to get output devices.
static const int GET_DEVICES_OUTPUTS = 2;

// Returns the list of input/output devices connected to the system.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager.
//   flag: Either GET_DEVICES_INPUTS or GET_DEVICES_OUTPUTS.
//   device_array: An array of BAudioDeviceInfo pointers. The caller has to
//                 allocate this array.
//   size: The size of device_array.
//   num_devices: A pointer to an unsigned int which will represent the number
//                of audio devices connected to the device.
//
// Returns 0 on success and errno on failure.
int BAudioManager_getDevices(
    const BAudioManager* brillo_audio_manager, int flag,
    BAudioDeviceInfo* device_array[], unsigned int size,
    unsigned int* num_devices);

// Select the input device to be used for recording.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager.
//   device: Device to set as the input device. Note that the device has to be
//           an input device.
//
// Returns 0 on success and errno on failure.
int BAudioManager_setInputDevice(const BAudioManager* brillo_audio_manager,
                                 const BAudioDeviceInfo* device);

// Usage types.
enum BAudioUsage {
  kUsageAlarm,
  kUsageMedia,
  kUsageNotifications,
  kUsageSystem
};

// Select the output device to be used for playback.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager.
//   device: Device to set as the output device. Note that the device has to
//           be an output device.
//   usage: A BAudioUsage type representing a usage to route to |device|.
//
// Returns 0 on success and errno on failure.
int BAudioManager_setOutputDevice(
    const BAudioManager* brillo_audio_manager, const BAudioDeviceInfo* device,
    BAudioUsage usage);

// Object used for callbacks.
struct BAudioCallback {
  // Function to be called when an audio device is added. If multiple audio
  // devices are added, then this function will be called multiple times. The
  // user is not responsible for freeing added_device.
  void (*OnAudioDeviceAdded)(const BAudioDeviceInfo* added_device,
                             void* user_data);

  // Function to be called when an audio device is removed. If multiple audio
  // devices are removed, then this function will be called multiple times. The
  // user is not responsible for freeing removed_device.
  void (*OnAudioDeviceRemoved)(const BAudioDeviceInfo* removed_device,
                               void* user_data);
};

typedef struct BAudioCallback BAudioCallback;

// Registers a callback object that lets clients know when audio devices have
// been added/removed from the system.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager.
//   callback: An object of type BAudioCallback. The BAudioManager
//             maintains ownership of this object.
//   user_data : A pointer to user data. This is not used by BAudioManager and
//               is passed as an arg to callbacks.
//   callback_id: A pointer to an int. The int represents a token that can be
//                used to de-register this callback. Contains 0 on failure.
//
// Returns 0 on success and errno on failure.
int BAudioManager_registerAudioCallback(
    const BAudioManager* brillo_audio_manager, const BAudioCallback* callback,
    void* user_data, int* callback_id);

// Unregisters a callback object.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager.
//   callback_id: A token correspoding to the callback object.
//
// Returns 0 on success and errno on failure.
int BAudioManager_unregisterAudioCallback(
    const BAudioManager* brillo_audio_manager, int callback_id);

// Free a Brillo audio manager object.
//
// Arg:
//   brillo_audio_manager: A pointer to a BAudioManager to be freed.
//
// Returns 0 on success and errno on failure.
int BAudioManager_delete(BAudioManager* brillo_audio_manager);

__END_DECLS

#endif  // BRILLO_AUDIO_AUDIOSERVICE_BRILLO_AUDIO_MANAGER_H_
