
#include "base/inputmanager.h"
#include "base/game.h"
#include "world/arrow.h"
#include "mouseWatcher.h"
#include "collisionRay.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionNode.h"
#include "collisionPlane.h"
#include "plane.h"

namespace pirates {

using world::Arrow;

namespace base {

// More global variables.

// The collision ray object used to pick.
static PT(CollisionRay) pickerRay;
// The collision plane object that represents collision with the floor.
static PT(CollisionPlane) floorPlane;
// The collision traverser object. It is responsible for detecting the collisions. The "ctraverser"
// string is its name.
static CollisionTraverser trav("ctraverser");
// The collision handler object. It handles the collisions detected. In this case we are using
// the queue handler, which is the simplest.
static PT(CollisionHandlerQueue) pickerHandler;
// The collision nodes that will have the collision objects attatched to them.
static PT(CollisionNode) pickerNode;
static PT(CollisionNode) floorNode;
// The NopePaths to the afore mentioned nodes.
static NodePath pickerNodePath;
static NodePath floorNodePath;
static NodePath environment;

static MouseWatcher* mouseWatcher = NULL;

// Let's check out the load_colliders function.
void load_colliders () {
    // Loads the environmentment model and stores it.
    // The load_model method from the window object needs the parent node that the model shall be
    // attached to and the path to the model file (.egg or .bam).
    environment = Game::reference()->window()->load_model(Game::reference()->framework().get_models(), "models/environment");
    // Reparents the model to render. This makes it visible.
    //environment.reparent_to(Game::reference()->window()->get_render());
    // Applies scale and position transforms to the model.
    //environment.set_color(0,0,1,1);
    environment.set_scale(0.25, 0.25, 0.25);
    environment.set_pos(-8, 42, 0);
    environment.set_tag("pickable", "");
    // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
    // "mouseRay
    pickerNode = new CollisionNode("mouseRay");
    // As the camera is a NodePath object, we may attatch other nodes to it. In this case, we
    // attatch the pickerNode because it moves together with the camera.
    pickerNodePath = Game::reference()->camera().attach_new_node(pickerNode);
    // TODO...
    pickerNode->set_from_collide_mask(CollisionNode::get_default_collide_mask());
    pickerRay = new CollisionRay();
    pickerNode->add_solid(pickerRay);
    pickerHandler = new CollisionHandlerQueue();
    trav.add_collider(pickerNodePath, pickerHandler);
    // Floor
    floorNode = new CollisionNode("floor");
    floorNodePath = environment.attach_new_node(floorNode);
    floorNodePath.set_pos(0, 0, 0);
    floorPlane = new CollisionPlane(Planef(LVector3f(0, 0, 1), LPoint3f(0, 0, 0)));
    floorNode->add_solid(floorPlane);
}

LPoint3f hit_pos;

AsyncTask::DoneStatus createVector(GenericAsyncTask* task, void* data) {
    return AsyncTask::DS_done;
}

CollisionEntry* pick (LPoint2f pos) {
    pickerRay->set_from_lens(Game::reference()->window()->get_camera(0), pos.get_x(), pos.get_y());
    /*
    LPoint3f origin = pickerRay->get_origin();
    LVector3f dir = pickerRay->get_direction();
    printf("origin: %f %f %f\n", origin.get_x(), origin.get_y(), origin.get_z());
    printf("dir: %f %f %f\n", dir.get_x(), dir.get_y(), dir.get_z());
    */
    trav.traverse(Game::reference()->window()->get_render());
    if (pickerHandler->get_num_entries() > 0) {
        pickerHandler->sort_entries();
        return pickerHandler->get_entry(0);
    }
    else {
        puts("No collision.");
        return NULL;
    }
}

void clickDownEvent (const Event* e, void *data) {
    puts("Mouse click detected!");
    if (mouseWatcher->has_mouse()){
        LPoint2f pos = mouseWatcher->get_mouse();
        //printf("Click at (%f, %f)\n", pos.get_x(), pos.get_y());
        PT(CollisionEntry) entry = pick(pos);
        if (entry) {
            NodePath pickedObj = entry->get_into_node_path();
            hit_pos = entry->get_surface_point(Game::reference()->window()->get_render());
            InputManager::reference()->get_arrow()->set_pos(hit_pos);
            //smiley.set_x(hit_pos.get_x());
            //smiley.set_y(hit_pos.get_y());

			//taskMgr->add(new GenericAsyncTask("Create Vector", &createVector, (void*) NULL));
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }
}

void clickUpEvent (const Event* e, void *data) {
    puts("Mouse UP detected!");
    if (mouseWatcher->has_mouse()){
        LPoint2f pos = mouseWatcher->get_mouse();
        //printf("Click at (%f, %f)\n", pos.get_x(), pos.get_y());
        PT(CollisionEntry) entry = pick(pos);
        if (entry) {
            NodePath pickedObj = entry->get_into_node_path();
            LPoint3f vector_end = entry->get_surface_point(Game::reference()->window()->get_render());
			LPoint3f vector = vector_end - hit_pos;
            printf("Down: %f, %f, %f \n", hit_pos.get_x(), hit_pos.get_y(), hit_pos.get_z() );
            printf("Up: %f, %f, %f \n", vector_end.get_x(), vector_end.get_y(), vector_end.get_z() );
            puts("Vector obtained:  " );//+ hit_pos + vector_end);
            printf("%f, %f, %f \n", vector.get_x(), vector.get_y(), vector.get_z() );
            InputManager::reference()->get_arrow()->look_at(vector_end);
			//smiley.set_x(hit_pos.get_x());
            //smiley.set_y(hit_pos.get_y());
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }
}

InputManager* InputManager::reference_ = NULL;

InputManager::InputManager() : game_(Game::reference()) {
    arrow_ = new Arrow;
}

void InputManager::Init() {
    Game::reference()->window()->enable_keyboard();
    mouseWatcher = static_cast<MouseWatcher*>(Game::reference()->window()->get_mouse().node());
    load_colliders();
    Game::reference()->framework().define_key("mouse1", "mouse_down", &clickDownEvent, NULL);
    Game::reference()->framework().define_key("mouse1-up", "mouse_up", &clickUpEvent, NULL);
}

//void InputManager::ClickDownEvent (const Event* e, void *data) {
    /*puts("Mouse click detected!");
    if (mouseWatcher->has_mouse()){
        LPoint2f pos = mouseWatcher->get_mouse();
        //printf("Click at (%f, %f)\n", pos.get_x(), pos.get_y());
        PT(CollisionEntry) entry = pick(pos);

        if (entry) {
            NodePath pickedObj = entry->get_into_node_path();
            LPoint3f hit_pos = entry->get_surface_point(window->get_render());
            smiley.set_x(hit_pos.get_x());
            smiley.set_y(hit_pos.get_y());
        }
    }
    else {
        puts("Mouse outsize window! How did you do that!?");
    }*/
//}

} // namespace base
} // namespace pirates

