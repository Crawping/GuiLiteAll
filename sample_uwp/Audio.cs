using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Media.MediaProperties;
using Windows.Storage;
using Windows.Storage.Streams;

namespace sample_uwp
{
    class Audio
    {
        public Audio(MainPage main_page)
        {
            m_main_page = main_page;
            init_audio_capture();
        }

        public async void start_record()
        {
            if (m_is_recording == true)
            {
                return;
            }

            try
            {
                var storageFile = await KnownFolders.MusicLibrary.CreateFileAsync("Voice.wav",
                                           Windows.Storage.CreationCollisionOption.ReplaceExisting);
                var profile = new MediaEncodingProfile();
                profile = MediaEncodingProfile.CreateWav(AudioEncodingQuality.Auto);
                profile.Audio = AudioEncodingProperties.CreatePcm(SAMPLE_RATE, 1, 16);
                await m_audio_capture.StartRecordToStorageFileAsync(profile, storageFile);
            }
            catch (Exception e)
            {
                return;
            }
            m_is_recording = true;
        }

        public async void stop_record()
        {
            if (m_is_recording == false)
            {
                return;
            }

            try
            {
                await m_audio_capture.StopRecordAsync();
                var wav_file = await KnownFolders.MusicLibrary.CreateFileAsync("Voice.wav",
                                                        CreationCollisionOption.OpenIfExists);
                IBuffer buffer = await FileIO.ReadBufferAsync(wav_file);
                byte[] raw_data = new byte[buffer.Length];
                DataReader reader = DataReader.FromBuffer(buffer);
                reader.ReadBytes(raw_data);
                m_main_page.m_sample_native.inputWavBuffer(raw_data, SAMPLE_RATE);
            }
            catch (Exception e)
            {
                return;
            }
            m_is_recording = false;
        }

        private async void init_audio_capture()
        {
            var captureInitSettings = new Windows.Media.Capture.MediaCaptureInitializationSettings();
            captureInitSettings.StreamingCaptureMode = Windows.Media.Capture.StreamingCaptureMode.Audio;
            m_audio_capture = new Windows.Media.Capture.MediaCapture();
            await m_audio_capture.InitializeAsync(captureInitSettings);
        }

        private Windows.Media.Capture.MediaCapture m_audio_capture;
        private MainPage m_main_page;
        private const int SAMPLE_RATE = 16000;
        public Boolean m_is_recording = false;
    }
}
