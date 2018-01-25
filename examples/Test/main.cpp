#include "Yage.h"

using namespace ye;

class TestGame : public Game
{
public:
    void Init(RenderingEngine const& renderer, Window const& window) override
    {
        Material bricks(renderer, "bricks", Texture(renderer, "./resources/textures/bricks.jpg"));

        AddToScene((new Entity())
            ->AddComponent(new CameraComponent(Matrix4f().InitPersective(
                YE_TORADIAN(70.0f), window.GetAspect(), 0.1f, 1000.0f))));

        AddToScene((new Entity(Vector3f(0.0f, 0.0f, 0.0f)))
            ->AddComponent(new MeshRenderer(Mesh(renderer, "./resources/models/cube.obj"),
                                            Material("bricks"))));
    }
};

int main()
{
    TestGame game;
    Window window(400, 300, "Yage Game");
    RenderingEngine renderer(window);

    CoreEngine coreEngine(5000, &window, &renderer, &game);
    coreEngine.Start();

    return 0;
}
