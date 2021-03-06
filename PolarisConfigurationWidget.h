#ifndef POLARISCONFIGURATIONWIDGET_H
#define POLARISCONFIGURATIONWIDGET_H

#include <QWidget>
#include <QString>

namespace Ui {
class PolarisConfigurationWidget;
}

//!Tracker Configuration
/*!
  This class is a user interface to select the requiere configuration files for the tracked 
  wireless tools and the tracker port used in the Scene3D.h class to configure a polaris tracker.
*/
class PolarisConfigurationWidget : public QWidget
{
    Q_OBJECT
    
public:

	/** \brief Class constructor*/
    explicit PolarisConfigurationWidget(QWidget *parent = 0);
    
	/** \brief Class destructor*/
	~PolarisConfigurationWidget();

	/** \brief Indicates if the aplication the user has quit the widget*/
	bool hasQuitted();

	/** 
	\brief Returns the QStrings of the rom and calibration files 
	[out] std::vector containing the 7 filenames
	*/
	std::vector<QString> getFiles();

	/** 
	\brief Returns the port where the tracker is connected
	[out] int indicating the igstk port
	*/
	int getPort();
    
private:
    
	Ui::PolarisConfigurationWidget *ui; ///<Tge user interface

	QString referenceRomFile; ///<String with the rom filename of the reference
	QString probeRomFile; ///<String with the rom filename of the us probe
	QString needleRomFile; ///<String with the rom filename of the needle
	QString pointerRomFile; ///<String with the rom filename of the pointer

	QString probeCalibrationFile; ///<String with the calibration filename of the us probe
	QString needleCalibrationFile; ///<String with the calibration filename of the needle
	QString pointerCalibrationFile; ///<String with the calibration filename of the pointer

	int port;

	bool quit;

private slots:

	/** \brief Choose the reference rom file */
    void chooseReferenceRom();

	/** \brief Choose the us probe rom file */
    void chooseProbeRom();

	/** \brief Choose the needle rom file */
    void chooseNeedleRom();

	/** \brief Choose the pointer rom file */
    void choosePointerRom();

	/** \brief Choose the us probe calibration file */
    void chooseProbeCalibration();

	/** \brief Choose the needle calibration file */
    void chooseNeedleCalibration();

	/** \brief Choose the pointer calibration file */
    void choosePointerCalibration();

	/** \brief configuration ok and close the widget */
	void ok();
};

#endif // POLARISCONFIGURATIONWIDGET_H
