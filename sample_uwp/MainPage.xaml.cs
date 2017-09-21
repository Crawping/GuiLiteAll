using System.IO;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;
using Windows.UI.ViewManagement;

using SampleUwpDll;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace sample_uwp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            InitializeComponent();
            initScreen();
            m_audio = new Audio(this);
    }

        private async void update_screen(object sender, object e)
        {
            m_sample_native.getFrameBuffer(m_frame_buffer);
            await m_fb_bitmap.PixelBuffer.AsStream().WriteAsync(m_frame_buffer, 0, m_frame_buffer.Length);
            m_fb_bitmap.Invalidate();
        }

        private void initScreen()
        {
            m_screen.Source = m_fb_bitmap;
            m_screen.PointerPressed += OnPointerPressed;
            m_screen.PointerReleased += OnPointerRelease;
            m_screen.PointerMoved += OnPointerMoved;
            m_sample_native.start();

            // Update frame buff per 30 ms.
            m_timer.Interval = new System.TimeSpan(0, 0, 0, 0, 30);
            m_timer.Tick += update_screen;
            m_timer.Start();
        }

        private void OnPointerMoved(object sender, PointerRoutedEventArgs e)
        {
            if(m_dragging)
            {
                OnPointerPressed(sender, e);
            }
        }

        private void OnPointerPressed(System.Object sender, PointerRoutedEventArgs e)
        {
            var pointer = e.GetCurrentPoint(null);
            var x = pointer.Position.X;
            var y = pointer.Position.Y;
            var rect = ApplicationView.GetForCurrentView().VisibleBounds;

            m_sample_native.onClickDown((int)(x * 1024 / rect.Width), (int)(y * 768 / rect.Height));
            m_dragging = true;

            //m_audio.start_record();
        }

        private void OnPointerRelease(System.Object sender, PointerRoutedEventArgs e)
        {
            var pointer = e.GetCurrentPoint(null);
            var x = pointer.Position.X;
            var y = pointer.Position.Y;
            var rect = ApplicationView.GetForCurrentView().VisibleBounds;

            m_sample_native.onClickUp((int)(x * 1024 / rect.Width), (int)(y * 768 / rect.Height));
            m_dragging = false;

            //m_audio.stop_record();
        }

        DispatcherTimer m_timer = new DispatcherTimer();
        private byte[] m_frame_buffer = new byte[1024 * 768 * 4];// Must be the same size of native frame buffer.
        private Audio m_audio = null;
        public SampleUwpNative m_sample_native = new SampleUwpNative();
        private Windows.UI.Xaml.Media.Imaging.WriteableBitmap m_fb_bitmap = new Windows.UI.Xaml.Media.Imaging.WriteableBitmap(1024, 768);
        private bool m_dragging = false;
    }
}
