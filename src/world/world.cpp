
#include "world/world.h"
#include "base/game.h"
#include "genericAsyncTask.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geom.h"
#include "geomNode.h"
#include "geomVertexWriter.h"
#include "geomTristrips.h"
#include "geomTriangles.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveWorldAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::World* World = static_cast <pirates::world::World*> (data);
    return World->moveWorld(task);
}

#define PI 3.1415926535897932384626433832795

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

    int num_tri_per_line = 16;
    int num_lines = 16;
    float sphere_radius = 10.0f;

    int num_vertex = 0;

    // Top One
    vertex.add_data3f(0, 0, 10.0f);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 0, 1);

    ++num_vertex;

    for(int j = 1; j < num_lines - 1; ++j) {
        float vangle = (0.5f - (float)(j) / num_lines) * PI;
        float z = sin(vangle);
        float cosv = cos(vangle);
        for(int i = 0; i < num_tri_per_line; ++i) {
            float xangle = ((float)(i) / num_tri_per_line) * 2.0f * PI;
            float x = cos(xangle) * cosv;
            float y = sin(xangle) * cosv;
            vertex.add_data3f(sphere_radius * x, sphere_radius * y, sphere_radius * z);
            normal.add_data3f(x, y, z);
            color.add_data4f(0, 0, 0.5f * (1.1f - z*z), 1);

            ++num_vertex;
        }
    }

    vertex.add_data3f(0, 0, -10.0f);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 0, 1);

    ++num_vertex;

    // Making the primitive
    //PT(GeomTristrips) prim;
    PT(GeomTriangles) prim;
    //prim = new GeomTristrips(Geom::UH_static);
    prim = new GeomTriangles(Geom::UH_static);
    for(int i = 0; i < num_tri_per_line; ++i)
        prim->add_vertices(0, i + 1, (i + 1) % num_tri_per_line + 1);

    for(int j = 1; j < num_lines - 2; ++j) {
        int start = (j - 1) * num_tri_per_line + 1;
        int next = j * num_tri_per_line + 1;
        for(int i = 0; i < num_tri_per_line; ++i) {
            int proxi = (i + 1) % num_tri_per_line;
            prim->add_vertices(start + i, next + i, next + proxi);
            prim->add_vertices(start + i, next + proxi, start + proxi);
        }
    }

    int last_line = (num_lines - 3) * num_tri_per_line + 1;
    printf("%d %d\n", last_line, num_vertex);
    for(int i = 0; i < num_tri_per_line; ++i) {
        prim->add_vertices(last_line + (i + 1) % num_tri_per_line, last_line + i, num_vertex - 1);
    }

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
