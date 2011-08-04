
#include "world/world.h"
#include "base/game.h"
#include "genericAsyncTask.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geom.h"
#include "geomNode.h"
#include "geomVertexWriter.h"
#include "geomTristrips.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveWorldAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::World* World = static_cast <pirates::world::World*> (data);
    return World->moveWorld(task);
}


namespace pirates {

using base::Game;

namespace world {

//LVector3f World::dir(0.1, 0, 0);

World::World () : dir(0, 0, 0) {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();


    // Creating the vertex data
    PT(GeomVertexData) vdata;
    vdata = new GeomVertexData("triangle", GeomVertexFormat::get_v3n3c4(), Geom::UH_static);
    GeomVertexWriter vertex, normal, color, texcoord;
    vertex = GeomVertexWriter(vdata, "vertex");
    normal = GeomVertexWriter(vdata, "normal");
    color = GeomVertexWriter(vdata, "color");

    vertex.add_data3f(-50, -50, .05);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 0, 1);
     
    vertex.add_data3f(50, -50, .05);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 0.3, 1);
     
    vertex.add_data3f(-50, 50, .05);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 0.6, 1);
     
    vertex.add_data3f(50, 50, .05);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 1, 1);


    // Making the primitive
    PT(GeomTristrips) prim;
    prim = new GeomTristrips(Geom::UH_static);
    prim->add_vertices(0, 1, 2, 3);

    // THE GEOM!
    PT(Geom) geom;
    geom = new Geom(vdata);
    geom->add_primitive(prim);
     
    // DA NODE!
    PT(GeomNode) node;
    node = new GeomNode("gnode");
    node->add_geom(geom);
     

    world_node_ = window->get_render().attach_new_node(node);


    /* world_node_ = window->load_model(framework.get_models(), "data/king");
    world_node_.set_scale(5);
    world_node_.reparent_to(window->get_render());
    world_node_.set_color(0,0,0,1);
    world_node_.set_pos(4, 0, 0); */
}

AsyncTask::DoneStatus World::moveWorld ( GenericAsyncTask* task ) {
    //static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time() + last;
    double dt = task->get_elapsed_time() - last;
    //LVector3f dir = ( 0, 1, 0 );
    this->world_node_.set_pos(this->world_node_.get_pos()+0.1*dt*dir);

    return AsyncTask::DS_cont;
}

void World::taskInitialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveWorldAux, 
        (void*) this);
    taskMgr.add(task);
}


} // Namespace world
} // Namespace pirates
