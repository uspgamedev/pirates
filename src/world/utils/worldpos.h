#ifndef PIRATES_WORLD_UTILS_WORLDPOS_H_
#define PIRATES_WORLD_UTILS_WORLDPOS_H_

namespace pirates {
namespace world {
namespace utils {

class WorldPos {
  public:
    WorldPos() : theta_(0.0f), phi_(0.0f), r_(-1.0f) {}
    WorldPos(float& theta, float& phi) : theta_(theta), phi_(phi), r_(-1.0f) {}
    WorldPos(float& theta, float& phi, float& r) : theta_(theta), phi_(phi), r_(r) {}
    
    // getters
    float radius() const { return r_; }
    float theta() const { return theta_; }
    float phi() const { return phi_; }

    // setters
    void set_radius(float& r) { r_ = r; }
    void set_theta(float& theta) { theta_ = theta; }
    void set_phi(float& phi) { phi_ = phi; } 

  private:
	union {
		struct { float x_, y_, z_; };
		struct { float theta_, phi_, r_; };
		struct { float val_[3]; };
	};
};

}
}
}

#endif

