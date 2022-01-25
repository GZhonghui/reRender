#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include <QStackedWidget>
#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QFileDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDialog>
#include <QPixmap>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QLine>

#include "MathAIO.h"
#include "ToolAIO.h"

#include "waInput.h"
#include "waButton.h"

#include "colorButton.h"

#include "glView.h"

#include "engineCore.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private: // Tools
    void loadStyle(const QString &qssFile);

protected: // UI
    // Import Button
    waInput* m_AToolsImportMenuInput;
    waButton* m_AToolsImportMenuSelect;

    // Add Button
    waInput* m_AToolsAddMenuInput;
    waButton* m_AToolsAddMenuCamera;
    waButton* m_AToolsAddMenuCube;
    waButton* m_AToolsAddMenuSphere;

    // Left List
    QListWidget* m_AList;
    
    // Use is Skybox Tab, Order = F B T D L R
    QLabel* m_BBTabSkyboxImages[6];

    // Use in Camera Tab
    QComboBox* m_BBTabCameraSelectWhich;
    QSpinBox* m_BBTabCameraResolutionWidth;
    QSpinBox* m_BBTabCameraResolutionHeight;
    QDoubleSpinBox* m_BBTabCameraFOV;

    // Use in Material Tab
    QComboBox* m_BBTabMaterialSelectType;
    // Defferent Widgets for Different Materials
    QStackedWidget* m_BBTabMaterialStacked;
    // Material: Diffuse
    colorButton* m_BBTabMaterialStackedDiffuseBaseColor;
    // Material: Metal
    colorButton* m_BBTabMaterialStackedMetalBaseColor;
    // Material: Glass
    // Material: Emit
    colorButton* m_BBTabMaterialStackedEmitBaseColor;

    // Use in Render Tab
    QProgressBar* m_RenderProgress;

    // ID and Type of Selected Obj in Left List
    QLineEdit* m_BAPropertySelectedID;
    QLineEdit* m_BAPropertySelectedType;

    // Input Box of Property Group
    QDoubleSpinBox* m_BAPropertyLocation[3];
    QDoubleSpinBox* m_BAPropertyRotation[3];
    QDoubleSpinBox* m_BAPropertyScale[3];

    QDoubleSpinBox* m_BAPropertyTarget[3];

    // 3 Views in Scenes
    glView* m_BAScenesEditView;
    QLabel* m_BAScenesCameraView;
    QLabel* m_BAScenesRenderView;

protected: // Use to connect with signal
    // List solts, Important
    void changeListItem(QListWidgetItem* Current);

    // Import Button solts
    void importIDChange(const QString& newText);
    void pushImportMesh();

    // Add Button solts
    void addIDChange(const QString& newText);
    void pushAddCamera();
    void pushAddCube();
    void pushAddSphere();

    // Delete Button
    void pushDelete();

    // Sort Button
    void pushSort();

    // About Button
    void showAbout();

    // Tab: Skybox, Select Buttons solts
    void selectSkyboxF(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'F'); }
    void selectSkyboxB(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'B'); }
    void selectSkyboxT(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'T'); }
    void selectSkyboxD(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'D'); }
    void selectSkyboxL(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'L'); }
    void selectSkyboxR(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'R'); }
    void selectSkybox(const QString& filePath, char Which);

    // Change Camera
    void cameraChanged(int Index);

    // Tab Material, Change Type
    void materialTypeChanged(int Index);

    // Render Button in Render Tab
    void pushRender();
};
#endif // MAINWINDOW_H
