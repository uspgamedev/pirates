
#include "base/game.h"
#include "base/inputmanager.h"
#include "base/runnable.h"
#include "world/inputhandler.h"
#include "world/ship.h"
#include "world/planet.h"
#include "world/utils/worlddata.h"
#include "load_prc_file.h"

#include "ambientLight.h"
#include "spotlight.h"
#include "perspectiveLens.h"
#include "mouseWatcher.h"

namespace pirates {

using world::Ship;
using world::Planet;
using world::utils::WorldData;

namespace base {

Game* Game::reference_ = NULL;

void Game::Init (int &argc, char **&argv) {
    framework_.open_framework(argc, argv);
    framework_.set_window_title("Pirates by USPGameDev");
    window_ = framework_.open_window();
    load_prc_file_data("", "model-path .");
    camera_node_ = window_->get_camera_group();
    camera_node_.set_pos(60,-120,150);
    camera_node_.look_at(0,0,0);

    planet_ = new Planet();
    InputManager::reference()->Init();

    const string ship_name = "ship";
    const string ship_model = "data/king";
    ship_ = new Ship(ship_name,ship_model);
    ship_->Initialize();

    MouseWatcher *mouseWatcher = static_cast<MouseWatcher*>(window_->get_mouse().node());
    pirates::world::InputHandler *handler =
        new pirates::world::InputHandler(Game::reference(), mouseWatcher, ship_);
    handler->Setup();
    InputManager::reference()->set_current_handler(handler);

    RunnableManager::reference()->Initialize();

    Spotlight* light = new Spotlight("the_light");
    light->set_color(Colorf(1, 1, 1, 1));
    light->set_lens(new PerspectiveLens());
    NodePath slnp = window_->get_render().attach_new_node(light);
    slnp.set_pos(20, 0, 300);
    slnp.look_at(20, 0, 0);
    window_->get_render().set_light(slnp);

    AmbientLight *amb_light = new AmbientLight("ambient_light");
    amb_light->set_color(Colorf(0.2f, 0.2f, 0.2f, 1));
    NodePath alnp = window_->get_render().attach_new_node(amb_light);
    window_->get_render().set_light(alnp);

	WorldData* data = new WorldData();
	data->Load("data/3fort.pwd");
}

} // namespace base
} // namespace pirates
