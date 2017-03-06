# FSE

## Setting up:

For setting up a project with FSE, see the Readme of / use the template project.

## Doc to third party libraries

- [SFML](http://www.sfml-dev.org/tutorials/2.4/)
- [Box2D](http://box2d.org/documentation/) and the doc inside the prebuilt Box2D folder of the project template
- [RTTR](http://www.rttr.org/doc/master/tutorial_page.html)
- [DearImgui:](https://github.com/ocornut/imgui) Dear ImGui's documentation is included here
- LTBL2's doc is included  here


## General things to know:

Except for SFML entities, FSE, as well as Box2D always expect MKS  (Meters, Kilos, Seconds), except otherwise noted.
SFML entities expect pixel values by default.
You can learn more about why in Box2D's manual.

You should use the defines *FSE_RADTODEG, FSE_PXMRATIO, FSE_PIXELS_PER_METER, FSE_METERS_PER_PIXEL*, present in Scene.h to convert between values.

#### FSEObjects, Scenes

Every FSEObject has an Update() and a Draw() method.
The Scene takes care of updating and drawing physics and your objects.

This occurs in the following order:
- Update physics of **all** physics bodys
- Update every FSEObject present in the scene, **higher** Z-Order objects get updated **first**
- Process any pending object spawns / creations or despawns / destructions
- Resort Z-Orders if necessary
- Draw every FSEObject present in the scene, **lower** Z-Order objects get drawn **first**
- Draw physics debug overlay if enabled.

If a scene is **paused**, FSEObjects still receive every call to update and draw, however, the deltaTime param of the update function is **always 0.0f** in this case.

**Z-Order**

Z-Order is an int attribute of the FSEObject, the default Z-Order, unless otherwise noted, is 0.
A lower Z-Order means the object is further away from the screen, a higher Z-Order means it is nearer to the screen and thus is rendered "above" lower ordered objects.

**Scenes**

You can, create multiple scenes, this is useful if you want to place your menu and loading-code in a separate scene from the one of your games level.

By default, your application class has a "*root_scene_*" you can use to start with.

### FSEObjects, Raw-Pointers and Smart-Pointers

If you look into the rest of the doc, you will quickly notice that every FSEObject in a Scene is represented by an std::unique_ptr, yet you still do get quite a lot of Raw-Pointers in most of the Object related callbacks.

I shied away from using std::shared_ptrs for a couple of reasons:
- A scene is, and will always be, the sole "owner" of an Object.
- Objects should be able to handle themselves, without an all knowing "god-objects"
- The Raw-Pointers of callbacks are to use just in the callback
  They are not meant for you to store any information about any FSEObject
  You can, and should, always rely on Physics interactions and corresponding callbacks if you want to observe or manipulate other Objects 
  **OR** use the Signal - Slot system present.
  With signals you do get a connection-item that tells you if the Signal / Slot is still connected, and thus, if connected Objects still exist.

If you keep those things in mind, you won't run in to any problems regarding FSEObject pointers.
If you, however, need to store a Raw-Pointer for any odd reason, you have to guarantee that the Object you stored the pointer to will outlive you.

You also **could** query every currently alive FSEObject of your scene.
Do keep in mind, however, that when you have a lot of objects, this will slow down your application significantly and thus is only really useful in rare-cases, such as debugging tools.

### Auto-cleanup, RAII, Texture handling, Lights

Almost all SFML and FSE classes are using RAII to ensure auto-cleanup.

LTBL2 and Box2D entities don't follow this behavior so strictly and thus, you need to clean them up manually using the factory methods provided from the Lightsystem and/or Physicsworld.

Note: Avoid use of the "new" or "delete" keywords. They shouldn't be needed in almost any case.

sf::Textures are neatly wrapped by fse::Texture with a bit of help from the fse::AssetLoader.

You can, and should always use fse::Textures where possible as the AssetLoader ensures automatic cleanup if they are not needed anymore.
You can, however, get a direct reference to the raw sf::Texture from the AssetLoader if needed. Do keep in mind however, that you have to manually unload them if you don't need them anymore.

Texture based light emissions are also neatly wrapped by fse::Lights and also auto cleanup themselves
Basic pointlights and spotlights are included with FSE. Take a look at those for more information on how to create your own.

### Physics

Most of the handling of the *physics world* is taken care of for you by FSE.
For *physics bodies*, *fixtures* and *joints* you can refer to the Box2D manual.

However, for the Engines callbacks to work properly it is needed that you set a this pointer of your FSEObject in the physics bodies userdata.
For fixtures and joints, this is not needed.
```
MyAwesomeObject::MyAwesomeObject(fse::Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
{
    b2BodyDef physBodyDef;
    physBodyDef.type = b2_dynamicBody;
    physBodyDef.position.Set(spawnPos.x, spawnPos.y);
    physBodyDef.angularDamping = 0.01f;
    physBodyDef.linearDamping = 0.0f;
    physBodyDef.fixedRotation = true;

    phys_body_ = scene_->getPhysWorld()->CreateBody(&physBodyDef);
    phys_body_->SetUserData(this);  // <--- This is the important call!
}
```

As previously noted, you are responsible for any cleanup of physics Bodies, Fixtures, joints, or any other Box2D entity that you create.
```
scene_->getPhysWorld()->DestroyBody(phys_body_);
```




---


