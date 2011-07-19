//--------------------------------------------------------------------------------------------------
// Panda3D prototype program.
//--------------------------------------------------------------------------------------------------

#include <cstdlib>

// For organization's sake, let's divide our program in parts.

// Initializes whatever needs to be initializes. The argc and argv parameters are needed by the
// Panda3D framework.
void init (int argc, char *argv[]);

// Loads the 3D models.
void load_models ();

// Loads the collider objects, so that we can "click" them.
void load_colliders ();

// Loads the sounds and music.
void load_sound ();

// Sets up the tasks to be done.
void set_tasks ();

// Sets up the possible events.
void set_events ();

// Runs the prototype.
void run ();

// Finishes everything before closing the program.
void finish ();

// Here is the main function. It pretty much only calls the previous functions in order.
int main(int argc, char *argv[]) {
    init(argc, argv);   
    load_models();
    load_colliders();
    load_sound();
    set_tasks();
    set_events();
    run();
    finish();
    return EXIT_SUCCESS;
}

// Now let's analyze each of those parts individually.
// For the init function, we will be needing the following Panda headers:
#include "pandaFramework.h"
#include "mouseWatcher.h"

// Since this is just a prototype, we will be abusing of global variables.
// Here are the first ones.

// The Panda framework object. It is the base of the engine and thus is needed to do anything on it.
static PandaFramework framework;
// A pointer to the window of the program. All the most important objects of the engine are in here.
static WindowFramework *window;
// The node path of the camera. This is needed in order to have control over the camera.
static NodePath camera;
// This is a panda pointer to the mouse watcher object. It is responsible for providing data about
// the mouse.
static PT(MouseWatcher) mouseWatcher;

// Some notes.
// NodePath: It is the representation of a panda object in the scene graph. It stores the panda
//  object it represents so we can always get it if we want to, but we cannot do the opposite:
//  given a panda object, there is no way of getting its representant on the scene graph, since
//  there can actually be more than one of those.
// PT(<class>): A special pointer type from the Panda3D engine. It should be used on panda classes
//  that inherit from the ReferenceCount class. It is made so that we do not need to worry about
//  deleting these objects from memory, because that will be done automatically.

// Now for the init function.
void init (int argc, char **argv) {
    // This opens a new window framework. It recquires the argc and argv parameter received by the
    // main function, so that we may pass some cinfguration values to the Panda engine.
    framework.open_framework(argc, argv);
    // Sets the window title.
    framework.set_window_title("Geddan Smiley-Chaser Panda");
    // Gets the window object and keeps it.
    window = framework.open_window();
    // Errr... Enables the MOUSE. >.<
    window->enable_keyboard();
    // Gets the camera and stores it.
    camera = window->get_camera_group();
    // Gets the mouse watcher object and stores it.
    // Note here that window->get_mouse() returns a NodePath, so we need to get the panda object
    // it represents and cast it into the proper class type. This is mane using the node() method
    // from the NodePath class and the static_cast reserved word from C++.
    mouseWatcher = static_cast<MouseWatcher*>(window->get_mouse().node());
    // This sets up the movement of the camera to 'trackball' type. See the Panda documentation to
    // know how to move the camera in this mode.
    window->setup_trackball();
    // This is needed for... geddan.
    srand(time(NULL));
}

// For the load_models function, we won't be needing any more headers, but we will have these three
// global variables, representing each model.
static NodePath environment;    // the scenery
static NodePath pandaActor;     // the smiley-chasing panda 
static NodePath smiley;         // the poor smiley

// Here goes the load_models function.
void load_models () {
    // Loads the environmentment model and stores it.
    // The load_model method from the window object needs the parent node that the model shall be
    // attached to and the path to the model file (.egg or .bam).
    environment = window->load_model(framework.get_models(), "models/environment");
    // Reparents the model to render. This makes it visible.
    environment.reparent_to(window->get_render());
    // Applies scale and position transforms to the model.
    environment.set_scale(0.25, 0.25, 0.25);
    environment.set_pos(-8, 42, 0);
    environment.set_tag("pickable", "");
    // Loads our panda.
    pandaActor = window->load_model(framework.get_models(), "panda-model");
    // More transforms.
    pandaActor.set_scale(0.005);
    pandaActor.reparent_to(window->get_render());
    pandaActor.set_pos(0, 10, 0);
    // Loads the walk animation, parenting it to the panda node.
    window->load_model(pandaActor, "panda-walk4");
    // Tells the window to loop all animations indefinetly.
    window->loop_animations(0);
    // Loads the smiley and reparents it to the render graph to make it visible too.
    smiley = window->load_model(framework.get_models(), "smiley");
    smiley.reparent_to(window->get_render());
}

// For the load_colliders functions, we will be needing the following Panda headers.
// Basically headers to the Collision* classes used.
#include "collisionRay.h"
#include "collisionTraverser.h"
#include "collisionHandlerQueue.h"
#include "collisionNode.h"
#include "collisionPlane.h"
#include "plane.h"

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

// Let's check out the load_colliders function.
void load_colliders () {
    // Makes a collision node to store the ray. Since it will come from the mouse, let's call it
    // "mouseRay".
    pickerNode = new CollisionNode("mouseRay");
    // As the camera is a NodePath object, we may attatch other nodes to it. In this case, we
    // attatch the pickerNode because it moves together with the camera.
    pickerNodePath = camera.attach_new_node(pickerNode);
    // TODO...
    pickerNode->set_from_collide_mask(CollisionNode::get_default_collide_mask());
    pickerRay = new CollisionRay();
    pickerNode->add_solid(pickerRay);
    pickerHandler = new CollisionHandlerQueue();
    trav.add_collider(pickerNodePath, pickerHandler);
    // Floor
    floorNode = new CollisionNode("floor");
    floorNodePath = environment.attach_new_node(floorNode);
    floorPlane = new CollisionPlane(Planef(LVector3f(0, 0, 1), LPoint3f(0, 0, 0)));
    floorNode->add_solid(floorPlane);
}

#include "audioManager.h"
#include "audioSound.h"

// Audio stuff
PT(AudioManager) AM;
PT(AudioSound) music, geddanMusic;

void load_sound () {
    AM = AudioManager::create_AudioManager();
    music = AM->get_sound("p0ss-oga-manifest.mp3");
    geddanMusic = AM->get_sound("promise.mp3");
    music->set_loop(true);
    music->play();
}

#include <cstdio>
#include "pandaSystem.h"

#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
 
#include "mouseButton.h"

#include "load_prc_file.h"
 
// The global task manager
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr(); 

bool GEDDAN = false;

float rand_angle () {
    return 360.0f*((1.0f*rand())/RAND_MAX);
}
 
AsyncTask::DoneStatus movePanda(GenericAsyncTask* task, void* data) {
    static LVector3f spd(-1, 0, 0);
    static double last = task->get_elapsed_time();
    double dt = task->get_elapsed_time() - last;
    LVector3f dir = smiley.get_pos() - pandaActor.get_pos();
    dir.normalize();
    dir.set_z(0.0);
    pandaActor.set_pos(pandaActor.get_pos()+dt*spd);
    if (!GEDDAN)
        pandaActor.look_at(pandaActor.get_pos()-spd);
    spd += 0.5*dt*dir;
    spd.normalize();
    last = task->get_elapsed_time();
    // Tell the task manager to continue this task the next frame.
    return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus hoverSmiley(GenericAsyncTask* task, void* data) {
    double time = task->get_elapsed_time();
    LPoint3f pos = smiley.get_pos();
    smiley.set_z(4.5+sin(2*time));
    return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus soundTask(GenericAsyncTask* task, void* data) {
    AM->update();
    return AsyncTask::DS_cont;
}

AsyncTask::DoneStatus geddan (GenericAsyncTask* task, void* data) {
    if (GEDDAN) {
        pandaActor.set_hpr(rand_angle(), rand_angle(), rand_angle());
        pandaActor.set_z(rand()%3);
    }
    return GEDDAN ? AsyncTask::DS_again : AsyncTask::DS_done;
}

AsyncTask::DoneStatus preGeddan(GenericAsyncTask* task, void* data) {
    AsyncTask *the_geddan = new GenericAsyncTask("Geddan", &geddan, (void*) NULL);
    the_geddan->set_delay(0.1);
    taskMgr->add(the_geddan);
    return AsyncTask::DS_done;
}



void set_tasks () {
    // Add our task.
    // If we specify custom data instead of NULL, it will be passed as the second argument
    // to the task function.
    taskMgr->add(new GenericAsyncTask("Hovers the smiley", &hoverSmiley, (void*) NULL));
    taskMgr->add(new GenericAsyncTask("Circle the Panda!!! HUZAHH!", &movePanda, (void*) NULL));
    taskMgr->add(new GenericAsyncTask("Play the music, DJ!", &soundTask, (void*) NULL));
}

CollisionEntry* pick (LPoint2f pos) {
    pickerRay->set_from_lens(window->get_camera(0), pos.get_x(), pos.get_y());
    trav.traverse(window->get_render());
    if (pickerHandler->get_num_entries() > 0) {
        pickerHandler->sort_entries();
        return pickerHandler->get_entry(0);
    }
    else {
        return NULL;
    }
}

void clickEvent (const Event* e, void *data) {
    puts("Mouse click detected!");
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
    }
}

double pause_time = 0.0;

void geddanEvent (const Event* e, void *data) {
    GEDDAN = !GEDDAN;
    if (GEDDAN) {
        pause_time = music->get_time();
        music->stop();
        geddanMusic->set_time(72.0);
        geddanMusic->play();
        if (geddanMusic->status() == AudioSound::BAD) {
            geddanMusic->stop();
            GEDDAN = false;
            music->set_time(pause_time);
            music->play();
        } else {
            AsyncTask* pre_geddan_task = new GenericAsyncTask("Pre-geddan", &preGeddan, (void*) NULL);
            pre_geddan_task->set_delay(1.25);
            taskMgr->add(pre_geddan_task);
        }
    } else {
        geddanMusic->stop();
        music->set_time(pause_time);
        music->play();
        pandaActor.set_z(0);
    }
}

void set_events () {
    framework.define_key ( "control-mouse1", "Mouse 1 Action", &clickEvent, NULL );
    framework.define_key("escape", "GEDDAN TIME", &geddanEvent, NULL);
}

void run () {
    framework.main_loop();
}

void finish () {
    // Shutdown the AudioManager.
    AM->shutdown();
    //close the window framework
    framework.close_framework();
}

