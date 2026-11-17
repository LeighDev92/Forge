#include "framework.h"
#include "Application.h"
#include "Mesh.h"

using namespace Forge;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize App
    Application app(hInstance, nCmdShow);
    
    Mesh bunny(GetResourcePath("bunny.obj").string());
    bunny.SetScale({ 20.f, 20.f, 20.f });
    bunny.SetPosition({ 0.f,0.f,-2.f });
    bunny.SetRotation({ 0.f, 0.f, 180.f });

    app.AddMesh(&bunny);

    return app.Run();
}