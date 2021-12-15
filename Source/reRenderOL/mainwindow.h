#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_set>
#include <unordered_map>

#include <QtGui>
#include <QtCore>
#include <QtWidgets>

#include <QDoubleSpinBox>
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

#include "wa_input.h"
#include "wa_button.h"

#include "gl_viewwidget.h"

#include "engine_core.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private: // Tools
    void loadStyle(const QString &qssFile);

private: // Only One
    void showAbout();

protected: // UI
    QWidget* m_MainWidget;
    QHBoxLayout* m_MainLayout;
    QVBoxLayout* m_ALayout;
    QVBoxLayout* m_BLayout;

    QHBoxLayout* m_BALayout;

    QGridLayout* m_AToolsLayout;
    QPushButton* m_AToolsAboutButton;
    QPushButton* m_AToolsImportButton;
    QPushButton* m_AToolsDeleteButton;
    QPushButton* m_AToolsAddButton;
    QPushButton* m_AToolsSortButton;

    QMenu* m_AToolsImportMenu;
    WA_Input* m_AToolsImportMenuInput;
    WA_Button* m_AToolsImportMenuSelect;

    QMenu* m_AToolsAddMenu;
    WA_Input* m_AToolsAddMenuInput;
    WA_Button* m_AToolsAddMenuCamera;
    WA_Button* m_AToolsAddMenuCube;
    WA_Button* m_AToolsAddMenuSphere;

    QListWidget* m_AList;

    QTabWidget* m_BBTabWidget;
    QWidget* m_BBTabSkybox;
    QWidget* m_BBTabCamera;
    QWidget* m_BBTabMaterial;
    QWidget* m_BBTabRender;

    QLabel* m_BBTabSkyboxImages[6];
    QPushButton* m_BBTabSkyboxSelect[6];

    QComboBox* m_BBTabMaterialSelectType;
    QLabel* m_BBTabMaterialDiffuseTextureImage;
    QPushButton* m_BBTabMaterialDiffuseTextureSelect;
    QLabel* m_BBTabMaterialNormalTextureImage;
    QPushButton* m_BBTabMaterialNormalTextureSelect;

    QGroupBox* m_BAProperty;
    QVBoxLayout* m_BAPropertyLayout;
    QLineEdit* m_BAPropertySelectedID;
    QLineEdit* m_BAPropertySelectedType;

    QDoubleSpinBox* m_BAPropertyLocation[3];
    QDoubleSpinBox* m_BAPropertyRotation[3];
    QDoubleSpinBox* m_BAPropertyScale[3];

    QDoubleSpinBox* m_BAPropertyTarget[3];

    QTabWidget* m_BAScenes;
    QWidget* m_BAScenesEdit;
    QWidget* m_BAScenesCamera;
    QWidget* m_BAScenesRender;

    QVBoxLayout* m_BAScenesEditLayout;
    QVBoxLayout* m_BAScenesCameraLayout;
    QVBoxLayout* m_BAScenesRenderLayout;

    GL_ViewWidget* m_BAScenesEditView;
    GL_ViewWidget* m_BAScenesCameraView;
    QLabel* m_BAScenesRenderView;

protected: // Use to connect with signal
    void changeListItem(QListWidgetItem* Current);

    void importIDChange(const QString& newText);
    void pushImportMesh();

    void addIDChange(const QString& newText);
    void pushAddCamera();
    void pushAddCube();
    void pushAddSphere();

    void pushDelete();

    void pushSort();

    void selectSkyboxF(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'F'); }
    void selectSkyboxB(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'B'); }
    void selectSkyboxT(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'T'); }
    void selectSkyboxD(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'D'); }
    void selectSkyboxL(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'L'); }
    void selectSkyboxR(){ auto File = QFileDialog::getOpenFileName(); selectSkybox(File,'R'); }
    void selectSkybox(const QString& filePath, char Which);
};
#endif // MAINWINDOW_H
