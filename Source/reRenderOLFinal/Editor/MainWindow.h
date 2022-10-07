#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include <QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QStackedWidget>

#include <QTabWidget>
#include <QListWidget>

#include <QDoubleSpinBox>
#include <QProgressBar>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>

#include <QColorDialog>
#include <QFileDialog>
#include <QDialog>

#include <QFileInfo>
#include <QPixmap>

#include "ToolAIO.h"
#include "MathAIO.h"

#include "GlView.h"
#include "GlMaterial.h"

#include "RenderDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void BuildUI();

// From ToolBar
public slots:
    void PushSave();
    void PushOpen();
    void PushLoad();
    void PushSphere();
    void PushDelete();
    void PushMode();
    void PushRender();
    void PushDenoise();
    void PushExport();
    void PushAbout();

protected:
    std::vector<uint8_t> RenderResultBuffer;
    int RenderResultWidth;
    int RenderResultHeight;

protected:
    QLineEdit* InputMeshName;

signals:
    void LoadMesh(QString BaseName, QString FilePath);

protected:
    GlView* RenderPreview = nullptr;
    QLabel* RenderResult = nullptr;
    QListWidget* ObjectList = nullptr;

public:
    void SelectedObjectChanged(const QString &currentText);

// Right Panel Properties
// Objects
protected:
    QDoubleSpinBox* ObjectLocation[3];
    QDoubleSpinBox* ObjectRotation[3];
    QDoubleSpinBox* ObjectScale[3];
    QComboBox* ObjectMaterial;

public:
    void UpdateTransform(double Value);
    void AttachMaterial(const QString &text);

// Materials
protected:
    QLineEdit* InputMaterialName;
    QComboBox* SelectMaterial;
    QComboBox* SelectMaterialType;
    QStackedWidget* MatParameter;
    QWidget* MatDiffuseParameter;
    QWidget* MatMetalParameter;
    QDoubleSpinBox* MatMetalFuzzy;
    QWidget* MatGlassParameter;
    QDoubleSpinBox* MatGlassIOR;
    QWidget* MatEmitParameter;
    QDoubleSpinBox* MatEmitIntensity;
    QWidget* MatFogParameter;

public:
    QColor DiffuseColor;
    void SelectDiffuseColor();

    QColor MetalColor;
    void SelectMetalColor();

    QColor EmitColor;
    void SelectEmitColor();

public:
    void PushAddMaterial();
    void SelectedMaterialChanged(const QString &text);
    void SelectedMaterialTypeChanged(const QString &text);
    void UpdateMaterialParameter();

// Data
private:
    // Change to set
    std::unordered_set<std::string> ObjectsName;
    std::unordered_set<std::string> MaterialsName;

// For Qss
private:
    void LoadStyle(const QString &QssFile);
};
#endif // MAINWINDOW_H
