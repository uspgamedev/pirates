
#include "world/ship.h"
#include "base/game.h"
#include "world/utils/navigator.h"
#include "genericAsyncTask.h"
#include "pnmImage.h"
#include "texture.h"
#include <string>

typedef AsyncTask::DoneStatus (*TaskFunc) (GenericAsyncTask*, void*);

static AsyncTask::DoneStatus moveShipAux ( GenericAsyncTask* task, void* data ) {
    pirates::world::Ship* ship = static_cast <pirates::world::Ship*> (data);
    return ship->moveShip(task);
}

namespace pirates {

using base::Game;

namespace world {

Ship::Ship() : WorldActor("lonely ship", GAME()->window()->load_model(GAME()->framework().get_models(), "data/king")) {
    PandaFramework& framework = Game::reference()->framework();
    WindowFramework* window = Game::reference()->window();
    //ship_node_ = window->load_model(framework.get_models(), "data/king");
    ship_node_.set_scale(0.5f);
    ship_node_.reparent_to(window->get_render());
    ship_node_.set_color((rand() / (float)(RAND_MAX))/10.0f+0.6f, (rand() / (float)(RAND_MAX))/10.0f+0.6f, (rand() / (float)(RAND_MAX))/10.0f+0.6f,1);
    ship_node_.set_pos(0.0f, 0.0f, 50.0f);

    PNMImage texImage = PNMImage();
    texImage.read(Filename("./data/blank.png"));
    Texture* ship_texture = new Texture("ship_texture");
    ship_texture->load(texImage);

    texture_blend_stage_ = new TextureStage("ts");
    texture_blend_stage_->set_mode(TextureStage::M_blend);
    ship_node_.set_texture( texture_blend_stage_, ship_texture, 1 );

    LVector3f dir(1.0f,0.0f,0.0f); // lol.
    LPoint3f ship_pos = ship_node_.get_pos();
    navigator_ = new utils::Navigator(this, ship_pos, dir);

    new_route_method_ = 0;
    anchored_ = false;
    matiz_ = 0.0f;
}

AsyncTask::DoneStatus Ship::moveShip ( GenericAsyncTask* task ) {
/*

    THE POWER OF GREEN INK
    MHWAHHAHAHAHHAHAHAHAHAHA

    Planet *planet = Game::reference()->planet();
    //static LVector3f spd( 0, -0.1, 0);
    static double last = task->get_elapsed_time();
    float dt = task->get_elapsed_time() - last, time = task->get_elapsed_time();
    float scalar_vel = vel.length();
    float vel_penalty_from_curve = 0.0f;

    float red = 1.0f;
    float green = 0.0f;
    float blue = 0.0f;
    float matiz_mod = (float)((int)(matiz_)%2) + (matiz_ - (float)((int)(matiz_)));
    float chroma_ctrl = (1 - fabs(matiz_mod - 1));
        // função triângulo (slopes lineares) : f(0)=0, f(1)=1, f(2)=f(0)=0, etc...
    int matiz_ctrl = 0;

    LVector3f old_tangent = this->new_tangent;
    if(!anchored_) {
        this->navigator_->get_next_pt(scalar_vel, dt, this->new_point, this->new_tangent);
        this->matiz_ = this->matiz_+(scalar_vel/planet->height_at(this->new_point)); //pq sim lol.
        matiz_ = (float)((int)(matiz_)%6) + ( matiz_ - (float)((int)(matiz_)) );
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
        texture_blend_stage_->set_color(LVector4f(red, green, blue, 1.0f));
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
        case Ship::DEST_ONLY:
            if( scalar_vel < 0.1f ) {
                scalar_vel = 0.1f;
            }
            this->navigator_->trace_new_route(this->new_point, scalar_vel, new_tg_norm, this->new_route_dest_pos_);
            new_route_method_ = 0;
            anchored_ = false;
        break;
        case Ship::DEST_AND_SPEED:
            if( scalar_vel < 0.1f )
                scalar_vel = 0.1f;
            this->navigator_->trace_new_route(this->new_point, scalar_vel, new_tg_norm, this->new_route_dest_pos_, this->new_route_dest_vel_);
            new_route_method_ = 0;
            anchored_ = false;
        break;
        case Ship::DONT_TRACE:
        default:
        break;
    }
    last = time;

    if( scalar_vel <= 0.00000001f && !anchored_) {
        puts("Lower the anchors!!!");
        anchored_ = true;
    }

    LPoint3f ship_sphere_pos = this->new_point/(this->new_point.length()/planet->height_at(this->new_point));
    LVector3f ship_look_at = ship_sphere_pos + this->new_tangent - this->new_tangent.project(ship_sphere_pos);
    this->ship_node_.set_pos(ship_sphere_pos);
    this->ship_node_.look_at(ship_look_at, planet->normal_at(this->new_point) );
*/
    return AsyncTask::DS_cont;
}

void Ship::taskInitialize ( AsyncTaskManager& taskMgr ) {
    GenericAsyncTask *task = new GenericAsyncTask(string("Ainda nao sei"),
        (TaskFunc)&moveShipAux, 
        (void*) this);
    taskMgr.add(task);
}

void Ship::set_new_route_dest(LPoint3f& dest) {
/*    new_route_dest_pos_ = dest;
    new_route_method_ = 1;
    if(anchored_)
        puts("Raise the anchors!!!1!!!one!"); */
}

void Ship::set_new_route_dest(LPoint3f& dest_pos, LVector3f& dest_vel) {
/*    new_route_dest_pos_ = dest_pos;
    new_route_dest_vel_ = dest_vel;
    new_route_method_ = 2;
    if(anchored_)
        puts("Raise the anchors!!!!!!"); */
}


} // Namespace world
} // Namespace pirates
