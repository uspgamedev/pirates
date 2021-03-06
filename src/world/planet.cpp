
#include "world/planet.h"
#include "base/game.h"
#include "geomVertexData.h"
#include "geomVertexFormat.h"
#include "geom.h"
#include "geomNode.h"
#include "geomVertexWriter.h"
#include "geomTristrips.h"
#include "geomTriangles.h"
#include <string>

#define PI 3.1415926535897932384626433832795

namespace pirates {

using base::Game;

namespace world {

Planet::Planet () : /*WorldActor(),*/ center_(LPoint3f::zero()) {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();

    // Creating the vertex data
    PT(GeomVertexData) vdata;
    vdata = new GeomVertexData("triangle", GeomVertexFormat::get_v3n3c4(), Geom::UH_static);
    GeomVertexWriter vertex, normal, color, texcoord;
    vertex = GeomVertexWriter(vdata, "vertex");
    normal = GeomVertexWriter(vdata, "normal");
    color = GeomVertexWriter(vdata, "color");

    int num_tri_per_line = 32;
    int num_lines = 32;

    int num_vertex = 0;

    // Top One
    vertex.add_data3f(0, 0, 40.0f);
    normal.add_data3f(0, 0, 1);
    color.add_data4f(0, 0, 1.0f, 1);

    ++num_vertex;

    num_lines--;
    for(int j = 1; j < num_lines; ++j) {
        float vangle = (0.5f - (float)(j) / num_lines) * PI;
        float z = sin(vangle);
        float cosv = cos(vangle);
        for(int i = 0; i < num_tri_per_line; ++i) {
            float xangle = ((float)(i) / num_tri_per_line) * 2.0f * PI;
            float x = cos(xangle) * cosv;
            float y = sin(xangle) * cosv;
            LPoint3f geom_point(x, y, z);
            float sphere_radius = height_at(geom_point);
            vertex.add_data3f(sphere_radius * x, sphere_radius * y, sphere_radius * z);
            normal.add_data3f(x, y, z);
            color.add_data4f(0.0f, 0.0f, 1.0f, 0.0f);

            ++num_vertex;
        }
    }

    vertex.add_data3f(0, 0, -40.0f);
    normal.add_data3f(0, 0, -1);
    color.add_data4f(0, 0, 1.0f, 1);

    ++num_vertex;

    // Making the primitive
    //PT(GeomTristrips) prim;
    PT(GeomTriangles) prim;
    //prim = new GeomTristrips(Geom::UH_static);
    prim = new GeomTriangles(Geom::UH_static);
    for(int i = 0; i < num_tri_per_line; ++i)
        prim->add_vertices(0, i + 1, (i + 1) % num_tri_per_line + 1);

    for(int j = 1; j < num_lines - 1; ++j) {
        int start = (j - 1) * num_tri_per_line + 1;
        int next = j * num_tri_per_line + 1;
        for(int i = 0; i < num_tri_per_line; ++i) {
            int proxi = (i + 1) % num_tri_per_line;
            prim->add_vertices(start + i, next + i, next + proxi);
            prim->add_vertices(start + i, next + proxi, start + proxi);
        }
    }

    int last_line = (num_lines - 2) * num_tri_per_line + 1;
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

    planet_node_ = window->get_render().attach_new_node(node);

    LPoint3f pos(1.0f, 0.0f, 0.0f);
    collision_ = new CollisionSphere(0.0f, 0.0f, 0.0f, height_at(pos));
}

LVector3f Planet::normal_at(const LPoint3f& pos) const {
    return pos/pos.length();
}
const float Planet::height_at(const LPoint3f& pos) const {
    return 50.0f;
}

} // Namespace world
} // Namespace pirates
