
#include "world/ship.h"
#include "base/game.h"
#include "genericAsyncTask.h"
#include "world/utils/routetracer.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveShipAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::Ship* ship = static_cast <pirates::world::Ship*> (data);
    return ship->moveShip(task);
}

namespace pirates {

using base::Game;

namespace world {

Ship::Ship () {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();
    ship_node_ = window->load_model(framework.get_models(), "data/king");
    ship_node_.set_scale(5);
    ship_node_.set_p(-90);
    ship_node_.reparent_to(window->get_render());
    ship_node_.set_color((rand() / (float)(RAND_MAX))/10.0f+0.6f, (rand() / (float)(RAND_MAX))/10.0f+0.6f, (rand() / (float)(RAND_MAX))/10.0f+0.6f,1);
    ship_node_.set_pos(0, 0, 0);

    vel.set(1.0f,1.0f,0.0f); // lol.
    vel = vel/vel.length();
    LPoint3f ship_pos = ship_node_.get_pos();
    LVector3f ship_vel_norm = vel/vel.length();
    route_tracer_ = new utils::RouteTracer(ship_pos, vel.length(), ship_vel_norm);

    new_route_method_ = 0;
    anchored_ = false;
    matiz_ = 0.0f;
}

AsyncTask::DoneStatus Ship::moveShip ( GenericAsyncTask* task ) {
    //static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time();
    float dt = task->get_elapsed_time() - last, time = task->get_elapsed_time();
    float scalar_vel = vel.length();
    float vel_penalty_from_curve = 0.0f;

    float red = 0.72f;
    float green = 0.0f;
    float blue = 0.0f;
    float matiz_mod = (float)((int)(matiz_)%2) + (matiz_ - (float)((int)(matiz_))); 
    float chroma_ctrl = (1 - fabs(matiz_mod - 1));
    int matiz_ctrl = 0;

    LVector3f old_tangent = this->new_tangent;
    if(!anchored_) {
        this->route_tracer_->get_next_pt(scalar_vel, dt, this->new_point, this->new_tangent);
        this->matiz_ = this->matiz_+(scalar_vel/40.0f); //pq sim lol.
        matiz_ = (float)((int)(matiz_)%6) + (matiz_ - (float)((int)(matiz_)));
        matiz_ctrl = floor(matiz_);
        switch(matiz_ctrl){
            case 0:
                red = 1.0f;
                green = chroma_ctrl;
                blue = 0.0f;
            break;
            case 1:
                red = chroma_ctrl;
                green = 1.0f;
                blue = 0.0f;
            break;
            case 2:
                red = 0.0f;
                green = 1.0f;
                blue = chroma_ctrl;
            break;
            case 3:
                red = 0.0f;
                green = chroma_ctrl;
                blue = 1.0f;
            break;
            case 4:
                red = chroma_ctrl;
                green = 0.0f;
                blue = 1.0f;
            break;
            case 5:
                red = 1.0f;
                green = 0.0f;
                blue = chroma_ctrl;
            break;
            default:
                red = 0.0f;
                green = 0.0f;
                blue = 0.0f;
            break;
        }
        ship_node_.set_color(red, green, blue, 1);
        vel_penalty_from_curve = (this->new_tangent/new_tangent.length() - old_tangent/old_tangent.length()).length()/(2.0*dt);
    }

    this->vel = this->new_tangent/2.0f; // porque o knot vector avança de 2 em 2.
    scalar_vel = vel.length();
    if(vel_penalty_from_curve <= 1.0f && scalar_vel > 0.1f)
        this->vel = this->vel*(1.0f-vel_penalty_from_curve);
    else if(scalar_vel > 0.1f)
        this->vel = 0.0f;
    scalar_vel = vel.length();

    LVector3f new_tg_norm;
    if(new_tangent.length() >= 0.00001f)
        new_tg_norm = this->new_tangent/this->new_tangent.length();
    else
        new_tg_norm = new_route_dest_pos_/new_route_dest_pos_.length();
    //this->ship_node_.set_pos(this->ship_node_.get_pos()+this->vel*dt);
    switch(new_route_method_) {
        case 1:
            if( scalar_vel < 0.1f ) {
                scalar_vel = 0.1f;
            }
            this->route_tracer_->trace_new_route(this->new_point, scalar_vel, new_tg_norm, this->new_route_dest_pos_);
            new_route_method_ = 0;
            anchored_ = false;
        break;
        case 2:
            if( scalar_vel < 0.1f )
                scalar_vel = 0.1f;
            this->route_tracer_->trace_new_route(this->new_point, scalar_vel, new_tg_norm, this->new_route_dest_pos_, this->new_route_dest_vel_);
            new_route_method_ = 0;
            anchored_ = false;
        break;
        case 0:
        default:
        break;
    }
    last = time;

    if( scalar_vel <= 0.00000001f && !anchored_) {
        puts("Lower the anchors!!!");
        anchored_ = true;
    }

    this->ship_node_.set_pos(this->new_point);
    this->ship_node_.look_at(this->new_point + new_tangent);
    this->ship_node_.set_p(-90);

    return AsyncTask::DS_cont;
}

void Ship::taskInitialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveShipAux, 
        (void*) this);
    taskMgr.add(task);
}

void Ship::set_new_route_dest(LPoint3f& dest) {
    new_route_dest_pos_ = dest;
    new_route_method_ = 1;
    if(anchored_)
        puts("Raise the anchors!!!1!!!one!");
}

void Ship::set_new_route_dest(LPoint3f& dest_pos, LVector3f& dest_vel) {
    new_route_dest_pos_ = dest_pos;
    new_route_dest_vel_ = dest_vel;
    new_route_method_ = 2;
    if(anchored_)
        puts("Raise the anchors!!!!!!");
}


} // Namespace world
} // Namespace pirates
