# FSE CHAISCRIPT CHEATSHEET
    
This cheatsheet is FSEs addition to the regular Chaiscript cheatsheet 
    
## Basic Funs
    
- int **random**(int from, int to)
- float **random**(float from, float to)
- double **random**(double from, double to)
      
- **exit**() closes ingame console 
- **clear**() clears ingame console 
    
**puts** and **print** print to the ingame console
    
## TYPES

#### Section: BaseLib

#### Vector2f

##### CTORS
- **Vector2f**()
- **Vector2f**(float, float)
- **Vector2f**(Vector2f)

##### Funcs
- :attr: **x**
- :attr: **y**
- ? **=**(Vector2f rhs)
- ? **+**(Vector2f rhs)
- ? **-**(Vector2f rhs)
- ? *****(Vector2f rhs)
- ? **/**(Vector2f rhs)
- ? *****(float f)
- ? **/**(float f)
- ? **==**(Vector2f rhs)
- ? **{ x:**()
#### Vector2i

##### CTORS
- **Vector2i**()
- **Vector2i**(float, float)
- **Vector2i**(Vector2i)

##### Funcs
- :attr: **x**
- :attr: **y**
- ? **=**(Vector2i rhs)
- ? **+**(Vector2i rhs)
- ? **-**(Vector2i rhs)
- ? **==**(Vector2i rhs)
- ? **{ x:**()
#### IntRect

##### CTORS
- **IntRect**()
- **IntRect**(int, int, int, int)
- **IntRect**(Vector2i, Vector2i)
- **IntRect**(IntRect)

##### Funcs
- :attr: **top**
- :attr: **left**
- :attr: **width**
- :attr: **height**
- ? **contains**(int x, int y)
- ? **contains**(Vector2i vec)
- ? **intersects**(IntRect other)
#### FloatRect

##### CTORS
- **FloatRect**()
- **FloatRect**(float, float, float, float)
- **FloatRect**(Vector2f, Vector2f)
- **FloatRect**(FloatRect)

##### Funcs
- :attr: **top**
- :attr: **left**
- :attr: **width**
- :attr: **height**
- ? **contains**(float x, float y)
- ? **contains**(Vector2f vec)
- ? **intersects**(FloatRect other)
#### Color

##### CTORS
- **Color**()
- **Color**(int, int, int, int)
- **Color**(int, int, int)
- **Color**(Color)

##### Funcs
- :attr: **r**
- :attr: **g**
- :attr: **b**
- :attr: **a**
- ? **=**(Color rhs)
- ? **==**(Color rhs)
#### Section: FSEObject

#### FPSCounter

##### Parent: [FSEObject](#fseobject)  

##### Funcs
- :attr: **detailed**
#### FSEObject

##### Funcs
- int **getID**()
- int **getZOrder**()
- void **setZOrder**(int)
- Vector2f **getPosition**()
- void **setPosition**(Vector2f position)
- ? **setPosition**(float x, float y)
- FloatRect **getAABBs**()
- bool **destroy**()
- Scene **getScene**()
- ? **getTypeName**()
- ? **type_name**()
#### KillVolume

##### Parent: [FSEObject](#fseobject)  

##### CTORS
- **KillVolume**()

##### Funcs
- Vector2f **getSize**()
- void **setSize**(Vector2f size)
#### Timer

##### Parent: [FSEObject](#fseobject)  

##### Funcs
- :attr: **active**
- :attr: **singleShot**
- :attr: **interval**
- :attr: **stop**
- ? **start**(function<void()
- ? **startSingleShotTimer**(Scene scene, int msecs, function<void()
#### Section: Lights

#### FSELightWorld

##### Parent: [FSEObject](#fseobject)  

##### Funcs
- :attr: **lighting**
- bool **getBloom**()
- void **setBloom**(bool)
- Color **getAmbientColor**()
- void **setAmbientColor**(Color color)
- ? **getLights**()
#### Light

##### CTORS
- **Light**()
- **Light**(Light)
- **Light**(Scene, Vector2f, string, bool)

##### Funcs
- void **setPosition**(Vector2f)
- Vector2f **getPosition**()
- void **setZPosition**(float)
- float **getZPosition**()
- void **setScale**(Vector2f)
- Vector2f **getScale**()
- void **setColor**(Color)
- Color **getColor**()
- void **setRotation**(float)
- float **getRotation**()
- void **rotate**(float)
- void **setTurnedOn**(bool)
- bool **isTurnedOn**()
- void **toggle**()
- Light **=**()
#### PointLight

##### Parent: [Light](#light)  

##### CTORS
- **PointLight**()
- **PointLight**(Scene, Vector2f)

##### Funcs
- void **setRadius**(float)
- float **getRadius**()
#### SpotLight

##### Parent: [Light](#light)  

##### CTORS
- **SpotLight**()
- **SpotLight**(Scene, Vector2f)

##### Funcs
- void **setLenght**(float)
- float **getLenght**()
- void **setAngle**(float)
- float **getAngle**()
#### Section: FSE

#### Scene

##### Funcs
- bool **isPaused**()
- void **setPaused**(bool)
- bool **getPhysDrawDebug**()
- void **setPhysDrawDebug**(bool)
- FSELightWorld **getLightWorld**()
- b2World **getPhysWorld**()
- void **spawnObject**(shared_ptr<FSEObject>)
- ? **getObjects**()
- shared_ptr<FSEObject>> **getObjectWithId**()
