/**
 * Copyright 2020, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_MEDIA_TUNERFRONTEND_H
#define ANDROID_MEDIA_TUNERFRONTEND_H

#include <aidl/android/media/tv/tuner/BnTunerFrontend.h>
#include <android/hardware/tv/tuner/1.0/ITuner.h>
#include <media/stagefright/foundation/ADebug.h>
#include <utils/Log.h>

using Status = ::ndk::ScopedAStatus;
using ::aidl::android::media::tv::tuner::BnTunerFrontend;
using ::aidl::android::media::tv::tuner::ITunerFrontendCallback;
using ::aidl::android::media::tv::tuner::TunerFrontendAtsc3Settings;
using ::aidl::android::media::tv::tuner::TunerFrontendDvbsCodeRate;
using ::aidl::android::media::tv::tuner::TunerFrontendSettings;
using ::aidl::android::media::tv::tuner::TunerFrontendStatus;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::hardware::hidl_vec;
using ::android::hardware::tv::tuner::V1_0::FrontendAtsc3PlpSettings;
using ::android::hardware::tv::tuner::V1_0::FrontendDvbsCodeRate;
using ::android::hardware::tv::tuner::V1_0::FrontendEventType;
using ::android::hardware::tv::tuner::V1_0::FrontendId;
using ::android::hardware::tv::tuner::V1_0::FrontendScanMessage;
using ::android::hardware::tv::tuner::V1_0::FrontendScanMessageType;
using ::android::hardware::tv::tuner::V1_0::IFrontend;
using ::android::hardware::tv::tuner::V1_0::IFrontendCallback;
using ::android::hardware::tv::tuner::V1_0::ITuner;


namespace android {

class TunerFrontend : public BnTunerFrontend {

public:
    TunerFrontend(sp<ITuner> tuner, int frontendHandle);
    virtual ~TunerFrontend();
    Status setCallback(
            const std::shared_ptr<ITunerFrontendCallback>& tunerFrontendCallback) override;
    Status tune(const TunerFrontendSettings& settings) override;
    Status stopTune() override;
    Status scan(const TunerFrontendSettings& settings, int frontendScanType) override;
    Status stopScan() override;
    Status setLnb(int lnbHandle) override;
    Status setLna(bool bEnable) override;
    Status close() override;
    Status getStatus(const std::vector<int32_t>& statusTypes,
            std::vector<TunerFrontendStatus>* _aidl_return) override;

    struct FrontendCallback : public IFrontendCallback {
        FrontendCallback(const std::shared_ptr<ITunerFrontendCallback> tunerFrontendCallback)
                : mTunerFrontendCallback(tunerFrontendCallback) {};

        virtual Return<void> onEvent(FrontendEventType frontendEventType);
        virtual Return<void> onScanMessage(
                FrontendScanMessageType type, const FrontendScanMessage& message);

        std::shared_ptr<ITunerFrontendCallback> mTunerFrontendCallback;
    };

private:
    hidl_vec<FrontendAtsc3PlpSettings> getAtsc3PlpSettings(
            const TunerFrontendAtsc3Settings& settings);
    FrontendDvbsCodeRate getDvbsCodeRate(const TunerFrontendDvbsCodeRate& codeRate);
    int mId;
    sp<ITuner> mTuner;
    sp<IFrontend> mFrontend;
};

} // namespace android

#endif // ANDROID_MEDIA_TUNERFRONTEND_H
