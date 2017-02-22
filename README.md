# FSE #

FSE (FlatSpot-Engine) is a (work in progress) cross platform 2D-Toolkit built on top of SFML and Box2D which could be used for Games and alike.

FSE is mainly created as a side project out of personal interest.

I only have limited knowledge about game- / engine-programming and thus, this is probably not the most well though out code there is. 
Hence, this is licensed under WTFPL 

FSE supports networking which in testing performed pretty well under pretty bad network conditions (~200ms Ping with some packetloss) with a few 100 objects

I'm quite happy with with the results so far, considering this essentially being a little "sunday project"

It also sports a pretty neat lighting system incl. normal and specular maps
![Lighting1](https://github.com/Alia5/FSE/blob/master/lighting1.gif?raw=true "Lighting1") ![Lighting2](https://github.com/Alia5/FSE/blob/master/lighting2.gif?raw=true "Lighting2")

*Work in Progress*

## License

```
Copyright © 2016 Peter Repukat - FlatspotSoftware
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the license.txt file for more details.
```

--
**Third party libraries used**

[SFML](http://www.sfml-dev.org) (2.4.1)

[Box2D](https://github.com/erincatto/Box2D/tree/374664b2a4ce2e7c24fbad6e1ed34bebcc9ab6bc) (374664b)

[LTBL2](https://github.com/Cmdu76/LTBL2/tree/d2b51608efa2aa7e3e2d632746b018d6e2add3be) (d2b5160) which is further modified

All of the above are licensed under the [zLib-License](https://en.wikipedia.org/wiki/Zlib_License)

[Dear ImGui] (https://github.com/ocornut/imgui) (1.49)

[ImGui SFML] (https://github.com/eliasdaler/imgui-sfml) (ca738ad)

Both of which are licensed under the [MIT-License](https://opensource.org/licenses/MIT)