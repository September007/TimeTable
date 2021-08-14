using System;
using System.Diagnostics;
using System.IO;
using Windows.Data.Xml.Dom;
using Windows.UI.Notifications;
using Windows.UI.Xaml;

namespace DesktopToastsSample
{
    public partial class MAINCLASS
    {
        static void Main(string[] args)
        {
            RunCmd("echo create toast later");
            ShowToastButton_Click();
        }
        static private void RunCmd(string command)
        {
            //例Process
            Process p = new Process();
            p.StartInfo.FileName = "cmd.exe";         //确定程序名
            p.StartInfo.Arguments = "/c " + command;   //确定程式命令行
            p.StartInfo.UseShellExecute = false;      //Shell的使用
            p.StartInfo.RedirectStandardInput = true;  //重定向输入
            p.StartInfo.RedirectStandardOutput = true; //重定向输出
            p.StartInfo.RedirectStandardError = true;  //重定向输出错误
            p.StartInfo.CreateNoWindow = true;        //设置置不显示示窗口
            p.Start();
            Console.Write( p.StandardOutput.ReadToEnd());      //输出出流取得命令行结果果
        }
        private const String APP_ID = "Microsoft.Samples.DesktopToastsSample";


        // Create and show the toast.
        // See the "Toasts" sample for more detail on what can be done with toasts
        static private void ShowToastButton_Click()
        {

            // Get a toast XML template
            XmlDocument toastXml = ToastNotificationManager.GetTemplateContent(ToastTemplateType.ToastImageAndText04);

            // Fill in the text elements
            XmlNodeList stringElements = toastXml.GetElementsByTagName("text");
            for (int i = 0; i < stringElements.Length; i++)
            {
                stringElements[i].AppendChild(toastXml.CreateTextNode("Line " + i));
            }

            // Specify the absolute path to an image
            String imagePath = "file:///" + Path.GetFullPath("toastImageAndText.png");
            XmlNodeList imageElements = toastXml.GetElementsByTagName("image");
            imageElements[0].Attributes.GetNamedItem("src").NodeValue = imagePath;

            // Create the toast and attach event listeners
            ToastNotification toast = new ToastNotification(toastXml);
            toast.Activated += ToastActivated;
            toast.Dismissed += ToastDismissed;
            toast.Failed += ToastFailed;

            // Show the toast. Be sure to specify the AppUserModelId on your application's shortcut!
            ToastNotificationManager.CreateToastNotifier(APP_ID).Show(toast);
            Console.ReadKey(); 
        }

        static private void ToastActivated(ToastNotification sender, object e)
        {
            RunCmd("echo The user activated the toast.");
        }

        static private void ToastDismissed(ToastNotification sender, ToastDismissedEventArgs e)
        {
            String outputText = "";
            switch (e.Reason)
            {
                case ToastDismissalReason.ApplicationHidden:
                    outputText = "The app hid the toast using ToastNotifier.Hide";
                    break;
                case ToastDismissalReason.UserCanceled:
                    outputText = "The user dismissed the toast";
                    break;
                case ToastDismissalReason.TimedOut:
                    outputText = "The toast has timed out";
                    break;
            }

            RunCmd(outputText);
        }

        static private void ToastFailed(ToastNotification sender, ToastFailedEventArgs e)
        {
        RunCmd("echo The toast encountered an error.");
    
        }
    }
}