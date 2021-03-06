#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "ProbeCalibrationWidget.h"
#include "CheckCalibrationErrorWidget.h"
#include "VolumeReconstructionWidget.h"
#include "CropImagesWidget.h"
#include "Scene3D.h"
#include "PivotCalibration.h"

#include <QVBoxLayout>
#include <vtkEventQtSlotConnect.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setAttribute(Qt::WA_DeleteOnClose);

  this->displayWidget = new QVTKImageWidget();

  QVBoxLayout *layout = new QVBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->QLayout::addWidget(displayWidget);
  ui->imageWidget->setLayout(layout);

  // hide the slider andonly show if an image stack is load
  ui->imageSlider->hide();
  // create the connections 
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();

}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::addImages()
{

	std::cout<<"Loading 2D Images"<<std::endl;
  this->imagesFilenames = QFileDialog::getOpenFileNames(this, tr("Open Images"),
	  QDir::currentPath(),tr("Image Files (*.png *.jpg *.bmp)"));
  if (!imagesFilenames.isEmpty())
    {
      if (imagesFilenames.size() == 1)
        {
          ui->imageSlider->hide();

          addLogText("Loading: <b>" + imagesFilenames.first() + "</b>");
          displayWidget->setAndDisplayImage(imagesFilenames.first());

          addLogText("Image Type: <b>" + displayWidget->getImageType() + "</b>");
          addLogText("Pixel Type: <b>" + displayWidget->getPixelType() + "</b>");
          addLogText("Num of Dimensions: <b>" + displayWidget->getNumOfDimesions() + "</b>");
        }
      else
        {
          // if selected multiple files
          ui->imageSlider->show();
          ui->imageSlider->setTickInterval(1);
          ui->imageSlider->setRange(0, imagesFilenames.size() - 1);

          this->displayWidget->setAndDisplayMultipleImages(imagesFilenames);
          for (int i = 0; i < imagesFilenames.size(); i++)
            {
              addLogText("Loading: <b>" + imagesFilenames.at(i) + "</b>");
            }
        }
    }
  else
    {
      return;
    }
}

void MainWindow::openVolumeData()
{

	std::cout<<"Loading Volume Data"<<std::endl;

    this->volumeImagesFilenames = QFileDialog::getOpenFileNames(this, tr("Open Volume Images"), 
		QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));
	
    this->volumeRotationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Rotation Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
	
    this->volumeTranslationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Translation Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
	
	this->volumeCalibrationData  = QFileDialog::getOpenFileName(this, tr("Open Volume Calibration Data"), 
		QDir::currentPath(), tr("Txt (*.txt *.doc)"));
    
	if (!volumeImagesFilenames.isEmpty())
      {
            this->displayWidget->setAndDisplayVolumeImages(volumeImagesFilenames, 
				volumeRotationData, volumeTranslationData, volumeCalibrationData);

      }
    else
      {
        return;
      }

}

void MainWindow::openVolume()
{


	std::cout<<"Loading Volume"<<std::endl;

    this->volumeFilename = QFileDialog::getOpenFileName(this, tr("Open Volume .mhd"),
        QDir::currentPath(), tr("Volume Files (*.mhd)"));
    
	this->displayWidget->setAndDisplayVolume(volumeFilename);

}

void MainWindow::probeCalibration()
{

	std::cout<<"Probe Calibration"<<std::endl;

	if (!displayWidget->getImageStack().empty())
    {
      ProbeCalibrationWidget* probeCalibration = new ProbeCalibrationWidget();

      if (displayWidget->isImageStackLoaded)
        probeCalibration->setImageStack(displayWidget->getImageStack());
      else
        probeCalibration->setImage(displayWidget->getImageViewer()->GetInput());

	  displayWidget->setProbeFlag(true);
      // get left mouse pressed with high priority
      Connections->Connect(displayWidget->getQVTKWidget()->GetRenderWindow()->GetInteractor(),
                           vtkCommand::LeftButtonPressEvent,
                           probeCalibration,
                           SLOT(getCoordinates()));

      probeCalibration->setMainWindow(this);
      probeCalibration->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load an images before calibrate the probe");
      errorMessage.exec();
    }
}

void MainWindow::pivotCalibration()
{
	std::cout<<"Pivot Calibration"<<std::endl;

	PivotCalibration *app = new PivotCalibration();
	app->Show();

	while( !app->HasQuitted() )
	{
		Fl::wait(0.001);
		igstk::PulseGenerator::CheckTimeouts();
	}

}

void MainWindow::checkCalibrationError()
{

	std::cout<<"Check Calibration Error"<<std::endl<<std::endl;

	if (!displayWidget->getImageStack().empty())
    {
      CheckCalibrationErrorWidget* checkCalibrationWidget = new CheckCalibrationErrorWidget();

      if (displayWidget->isImageStackLoaded)
        checkCalibrationWidget->setImageStack(displayWidget->getImageStack());
      else
        checkCalibrationWidget->setImage(displayWidget->getImageViewer()->GetInput());

	    displayWidget->setCalibrationErrorWidget(checkCalibrationWidget);
	  	displayWidget->setProbeFlag(false);
		displayWidget->startTracer();
	  
      checkCalibrationWidget->setMainWindow(this); 
      checkCalibrationWidget->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load an images before checking the calibration");
      errorMessage.exec();
    }
}

void MainWindow::volumeReconstruction()
{

	std::cout<<"VOLUME RECONSTRUCTION"<<std::endl<<std::endl;
    if (!displayWidget->getVolumeImageStack().empty())
      {
       VolumeReconstructionWidget * volumeReconstruction = new VolumeReconstructionWidget();
		
		if (displayWidget->isVolumeImageStackLoaded){
           volumeReconstruction->setVolumeImageStack(displayWidget->getVolumeImageStack());
		   volumeReconstruction->setTransformStack(displayWidget->getTransformStack());
		}

        volumeReconstruction->setMainWindow(this);
        volumeReconstruction->show();
      }
    else
      {
        QErrorMessage errorMessage;
        errorMessage.showMessage(
            "No volume data loaded, </ br> please data before reconstruct the volume");
        errorMessage.exec();
      }
	
}

void MainWindow::display3DScene()
{

	Scene3D* scene3D = new Scene3D();
	scene3D->init3DScene();
}


void MainWindow::cropImages()
{
	std::cout<<"Image Cropping"<<std::endl;

	if (!displayWidget->getImageStack().empty())
    {
      CropImagesWidget* cropImages = new CropImagesWidget();

      if (displayWidget->isImageStackLoaded)
        cropImages->setImageStack(displayWidget->getImageStack());
      else
        cropImages->setImage(displayWidget->getImageViewer()->GetInput());

      cropImages->setMainWindow(this);
      cropImages->show();
    }
  else
    {
      QErrorMessage errorMessage;
      errorMessage.showMessage(
		  "No images loaded, </ br> please load an images before crop images");
      errorMessage.exec();
    }
}


void MainWindow::displaySelectedImage(int idx)
{
  this->displayWidget->displaySelectedImage(idx);
}


QVTKImageWidget* MainWindow::getDisplayWidget()
{
  return this->displayWidget;
}


void MainWindow::addLogText(QString str)
{
  ui->textEdit->append(str);

  // move the cursor to the end of the last line
  QTextCursor cursor = ui->textEdit->textCursor();
  cursor.movePosition(QTextCursor::End);
  ui->textEdit->setTextCursor(cursor);
}


void MainWindow::print()
{
  this->addLogText("estoy cachando el evento");
}

