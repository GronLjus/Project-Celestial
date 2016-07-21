using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using CelestialFileConverter;
using CelestialFileConverter.FileHandler;
using CelestialFileConverter.FileObj;
using CelestialFileConverter.ClMesh;

namespace CelestialFileConverter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        private string filePath;

        public MainWindow()
        {
            InitializeComponent();
        }


        private void SelectButton_Click(object sender, RoutedEventArgs e)
        {

            OpenFileDialog fileOpen = new OpenFileDialog();
            fileOpen.Filter = "Object Files (*.obj)|*.obj";
            fileOpen.FilterIndex = 0;
            fileOpen.Multiselect = false;

            bool? userClicked = fileOpen.ShowDialog();

            if(userClicked == true)
            {

                filePath = fileOpen.FileName;
                ConvertButton.IsEnabled = true;
                StatusBlock.Text += "\r\nFile Chosen:";
                StatusBlock.Text += "\r\n" + filePath;

            }

        }

        private void ConvertButton_Click(object sender, RoutedEventArgs e)
        {

            string mshFile = System.IO.Path.ChangeExtension(filePath, "clmsh");
            Progress.IsEnabled = true;
            StatusBlock.Text += "\r\n";
            StatusBlock.Text += "\r\nLoading obj-file";
            ObjObject obj = ObjFileHandler.LoadObjFromText(File.ReadAllLines(filePath));
            StatusBlock.Text += "\r\nObject loaded!";
            StatusBlock.Text += "\r\nConverting object to CelestialMesh!";
            Progress.Value += 25;
            CLMSH mesh = CLMSHLoader.ConvertOBJ(obj);
            StatusBlock.Text += "\r\nObject converted!";
            StatusBlock.Text += "\r\nSaving file to:";
            StatusBlock.Text += "\r\n"+mshFile;
            Progress.Value += 50;
            CLMSHFileHandler.ExportMesh(mshFile, mesh);
            StatusBlock.Text += "\r\n";
            StatusBlock.Text += "\r\n!!File Converted!!";
            Progress.Value += 25;
            ConvertButton.IsEnabled = false;


        }
    }
}
