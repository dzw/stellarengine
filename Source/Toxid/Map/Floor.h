#ifndef TOXID_FLOOR_H
#define TOXID_FLOOR_H

namespace Toxid {

class Floor :
  public Storage::Persistable {
  public:
    virtual ~Floor() {}
    
  //
  // Floor implementation
  //
  public:
    virtual bool isStatic() const { return true; }
    virtual const shared_ptr<Scene::Model> &getModel() const = 0;
        
    virtual void onEnter(const shared_ptr<Actor> &spActor) {}
    virtual void onLeave(const shared_ptr<Actor> &spActor) {}
    
    virtual void renderFrame(const shared_ptr<VideoRenderer> &spVR) = 0;
    virtual void timeFrame(float fDelta) {}
        
  private:
};

}

#endif // TOXID_FLOOR_H