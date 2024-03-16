#ifndef GL_VIEWWIDGET_H
#define GL_VIEWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QPixmap>

#include "ToolAIO.h"
#include "MathAIO.h"

#include "GlCamera.h"
#include "GlMaterial.h"
#include "GlRenderable.h"

#include "RenderMessage.pb.h"

class GlManager
{
public:
    GlManager(QOpenGLFunctions_3_3_Core* F, GlCamera* C) : m_F(F), mainCamera(C),
        m_Skybox(F), m_Grid(F), m_Axis(F), m_Outline(F), m_ShaderManager(F)
    {
        setLightDir(Direction(1,1,1));
        setLightColor(Color(1,1,1));
        setLightPower(2.0);
    }
    virtual ~GlManager() = default;

private:
    QOpenGLFunctions_3_3_Core* m_F;
    GlCamera* mainCamera;

private:
    const double NearZ  = 0.100;
    const double FarZ   = 300.0;

protected:
    uint32_t m_SceneFBO;
    uint32_t m_SceneTextureID;
    uint32_t m_DepthTextureID;

public:
    uint32_t getSceneTextureID() const { return m_SceneTextureID; }

protected:
    // Update Every Frame
    uint32_t m_ViewWidth;
    uint32_t m_ViewHeight;
    
    Point     m_CameraPos;
    Direction m_CameraDir;
    glm::mat4 m_P;

protected:
    Direction m_LightDir;
    Color     m_LightColor;
    double    m_LightPower;

public:
    Direction getLightDir() const noexcept         { return m_LightDir;       }
    void      setLightDir(const Direction& newDir) { m_LightDir = newDir;     }

    Color     getLightColor() const noexcept       { return m_LightColor;     }
    void      setLightColor(const Color& newColor) { m_LightColor = newColor; }

    double    getLightPower() const noexcept       { return m_LightPower;     }
    void      setLightPower(double newPower)       { m_LightPower = newPower; }

protected:
    class Skybox
    {
    public:
        Skybox(QOpenGLFunctions_3_3_Core* F) : m_F(F)
        {
            m_NowSkybox = 0;
        }
        virtual ~Skybox() = default;

    private:
        QOpenGLFunctions_3_3_Core* m_F;

    public:
        uint32_t m_SkyboxTextureID;
        uint32_t m_SkyboxShaderProgramID;
        uint32_t m_SkyboxVAOID;
        uint32_t m_SkyboxVBOID;

        const char* m_SkyboxsChar;
        std::vector<std::string> m_Skyboxs;
        std::vector<std::string> m_SkyboxsPath;
        std::unordered_map<std::string, int> m_RevSkyboxs;

        int m_NowSkybox;

    public:
        void Init();
        void Destroy();
        void Render(glm::mat4* VP);
        void Change(int Which);
    }m_Skybox;

public:
    void RenderSkybox()
    {
        glm::mat4 VP[2];
        
        mainCamera->updateView(VP[0]);

        VP[1] = m_P;

        m_Skybox.Render(VP);
    }

    void ChangeSkybox(int Which)
    {
        m_Skybox.Change(Which);
    }

    unsigned int getNowSkybox()   const { return m_Skybox.m_NowSkybox;      }
    const char*  getSkyboxList()  const { return m_Skybox.m_SkyboxsChar;    }
    unsigned int getSkyboxCount() const { return m_Skybox.m_Skyboxs.size(); }
    std::string  getSkyboxAt(int Idx)   { return m_Skybox.m_Skyboxs[Idx];   }
    int          getIdxOfSkybox(const std::string& inStr)
    {
        if (!m_Skybox.m_RevSkyboxs.count(inStr)) return -1;
        return m_Skybox.m_RevSkyboxs[inStr];
    }

protected:
    class Grid
    {
    public:
        Grid(QOpenGLFunctions_3_3_Core* F) : m_F(F) {}
        virtual ~Grid() = default;

    private:
        QOpenGLFunctions_3_3_Core* m_F;

    public:
        uint32_t m_GridShaderProgramID;
        uint32_t m_GridVAOID;
        uint32_t m_GridVBOID;

        unsigned int m_GridVerticesCount;

    public:
        void Init();
        void Destroy();
        void Render(glm::mat4* VP);
    }m_Grid;

public:
    void RenderGrid()
    {
        glm::mat4 VP[2];

        mainCamera->updateView(VP[0]);

        VP[1] = m_P;

        m_Grid.Render(VP);
    }

protected:
    class Axis
    {
    public:
        Axis(QOpenGLFunctions_3_3_Core* F) : m_F(F) {}
        virtual ~Axis() = default;

    private:
        QOpenGLFunctions_3_3_Core* m_F;

    public:
        uint32_t m_AxisEndShaderProgramID;
        uint32_t m_AxisEndVAOID;
        uint32_t m_AxisEndVBOID;

        uint32_t m_AxisLineShaderProgramID;
        uint32_t m_AxisLineVAOID;
        uint32_t m_AxisLineVBOID;

    public:
        void Init();
        void Destroy();
        void Render(double Length, double Size, glm::mat4* MVP);
    }m_Axis;

protected:
    class Outline
    {
    public:
        Outline(QOpenGLFunctions_3_3_Core* F) : m_F(F) {}
        virtual ~Outline() = default;

    private:
        QOpenGLFunctions_3_3_Core* m_F;

    public:
        uint32_t m_OutlineShaderID;

    public:
        void Init();
        void Destroy();
    }m_Outline;

protected:
    class ShaderManager
    {
    public:
        ShaderManager(QOpenGLFunctions_3_3_Core* F) : m_F(F) {}
        virtual ~ShaderManager() = default;

    private:
        QOpenGLFunctions_3_3_Core* m_F;

    public:
        uint32_t m_DefaultShaderProgramID;
        uint32_t m_GlassShaderProgramID;
        uint32_t m_MetalShaderProgramID;
        uint32_t m_CartoonShaderProgramID;

        const char* m_SupportShadersChar;
        std::vector<std::string> m_SupportShaders;
        std::vector<std::string> m_SupportShadersPath;
        std::unordered_map<std::string, int> m_RevSupportShaders;

    public:
        void Init();
        void Destroy();
    }m_ShaderManager;

public:
    const char*  getSupportShaderList()  const { return m_ShaderManager.m_SupportShadersChar;    }
    unsigned int getSupportShaderCount() const { return m_ShaderManager.m_SupportShaders.size(); }
    std::string  getSupportShaderAt(int Idx)   { return m_ShaderManager.m_SupportShaders[Idx];   }
    int          getIdxOfSupportShader(const std::string& inStr)
    {
        if (!m_ShaderManager.m_RevSupportShaders.count(inStr)) return 0;
        return m_ShaderManager.m_RevSupportShaders[inStr];
    }

public:
    bool Init();
    bool Destroy();

public:
    void BeginRenderEditor(uint32_t viewW, uint32_t viewH);
    void EndRenderEditor();

    void BeginRenderGame(uint32_t viewW, uint32_t viewH);
    void EndRenderGame();

public:
    void Render
    (
        GlRenderable* renderObj,
        const Transform& ObjTransform, bool Selected, bool LineMode
    );
};

class SceneObject
{
public:
    SceneObject() = default;
    virtual ~SceneObject() = default;

protected:
    std::string Name;
    GlRenderable Renderable;
    Transform Trans;
    std::string MaterialName;

public:
    friend class GlView;
};

class GlView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GlView(QWidget *parent = nullptr);
    virtual ~GlView() { Destroy(); }

private:
    void Destroy();

private:
    GlCamera Camera;
    GlManager Manager;

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;

public:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
    bool KW = false, KS = false, KA = false, KD = false;

public:
    void Tick();

private:
    QTimer TickTimer;

protected:
    std::unordered_map<std::string, std::shared_ptr<SceneObject>> m_NameToObjectPtr;

    std::unordered_map<std::string, std::shared_ptr<GlMaterial>> m_NameToMaterialPtr;

// MainWindow Interface
public:
    void LoadMesh(QString BaseName, QString FilePath);
    void LoadSphere(QString BaseName);

    void UpdateTransform(const Transform& NewTransform);
    Transform GetTransform();

    void AttachMaterial(const std::string& MaterialName);
    std::string GetMaterial();

    std::queue<std::pair<std::string, std::string>> MeshToLoad;
    std::queue<std::string> SphereToLoad;

    std::string SelectedObjectName;

public:
    void AddMaterial(const std::string& MaterialName);

public:
    void PackageScene(PSceneRequest& SceneData);

public:
    bool LineMode = false;

public:
    friend class MainWindow;
};

#endif // GL_VIEWWIDGET_H
