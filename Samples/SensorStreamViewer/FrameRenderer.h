//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the Microsoft Public License.
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "LookupTable.h"

namespace SensorStreaming
{
    // Function type used to map a scanline of pixels to an alternate pixel format.
    typedef std::function<void(int, byte*, byte*)> TransformScanline;

    public ref class FrameRenderer sealed
    {
    public:
        FrameRenderer(Windows::UI::Xaml::Controls::Image^ image);

    public: // Public methods.
        void SetSensorName(Platform::String^ sensorName);

        /// <summary>
        /// Buffer and render frames.
        /// </summary>
        void ProcessFrame(Windows::Media::Capture::Frames::MediaFrameReference^ frame);

        /// <summary>
        /// Determines the subtype to request from the MediaFrameReader that will result in
        /// a frame that can be rendered by ConvertToDisplayableImage.
        /// </summary>
        /// <returns>Subtype string to request, or null if subtype is not renderable.</returns>
        static Platform::String^ GetSubtypeForFrameReader(
            Windows::Media::Capture::Frames::MediaFrameSourceKind kind,
            Windows::Media::Capture::Frames::MediaFrameFormat^ format);

    private: // Private static methods.
        /// <summary>
        /// Transforms pixels of inputBitmap to an output bitmap using the supplied pixel transformation method.
        /// Returns nullptr if translation fails.
        /// </summary>
        static Windows::Graphics::Imaging::SoftwareBitmap^ TransformBitmap(
            Windows::Graphics::Imaging::SoftwareBitmap^ inputBitmap,
            TransformScanline pixelTransformation);

        static Windows::Graphics::Imaging::SoftwareBitmap^ TransformVlcBitmap(
            Windows::Graphics::Imaging::SoftwareBitmap^ inputBitmap);

        static Windows::Graphics::Imaging::SoftwareBitmap^ DeepCopyBitmap(
            Windows::Graphics::Imaging::SoftwareBitmap^ inputBitmap);

    private: // Private instance methods.
        /// <summary>
        /// Converts the input frame to BGRA8 premultiplied alpha format and returns the result.
        /// Returns nullptr if the input frame cannot be converted BGRA8 premultiplied alpha.
        /// </summary>
        Windows::Graphics::Imaging::SoftwareBitmap^ ConvertToDisplayableImage(
            Windows::Media::Capture::Frames::VideoMediaFrame^ inputFrame);

    private: // Private data.
        Windows::UI::Xaml::Controls::Image^ m_imageElement;
        Platform::String^ m_sensorName;

        static const int32_t c_maxNumberOfTasksScheduled{ 1 };
        static const int32_t c_maxNumberOfTasksRunning{ 1 };

        volatile long m_numberOfTasksScheduled{ 0 };
        volatile long m_numberOfTasksRunning{ 0 };
    };
}