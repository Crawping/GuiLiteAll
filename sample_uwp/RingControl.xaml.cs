using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace sample_uwp
{
    public sealed partial class RingControl : UserControl
    {
        public RingControl()
        {
            this.InitializeComponent();
            this.InitStoryBoard();
        }

        private void InitStoryBoard()
        {
            control = (Control)RingControll;
            StartIdleAnimation();
        }

        public void StartIdleAnimation()
        {
            VisualStateManager.GoToState(control, "VsIdle", true);
        }

        public void StartThinkingAnimation()
        {
            VisualStateManager.GoToState(control, "VsThinking", true);
        }

        public void StartListeningAnimation()
        {
            VisualStateManager.GoToState(control, "VsListening", true);
        }

        private Control control;
    }
}
