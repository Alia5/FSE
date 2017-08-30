# FSE #

FSE is a (work in progress) cross platform 2D-Toolkit built on top of SFML and Box2D which could be used for Games and alike.

FSE is mainly created as a side project out of personal interest.

I only have limited knowledge about game- / engine-programming and thus, this is probably not the most well though out code there is. 
Hence, this is licensed under WTFPL 

FSE supports networking which in testing performed pretty well under pretty bad network conditions (~200ms Ping with some packet loss) with a few 100 objects

I'm quite happy with with the results so far, considering this essentially being a little "Sunday project"

It also sports a pretty neat lighting system incl. normal and specular-maps
![Lighting1](https://github.com/Alia5/FSE/blob/master/lighting1.gif?raw=true "Lighting1") ![Lighting2](https://github.com/Alia5/FSE/blob/master/lighting2.gif?raw=true "Lighting2")

*Work in Progress*

## License

```
Copyright © 2016 Peter Repukat - FlatspotSoftware
This work is free.
It comes without any warranty, to the extent permitted by applicable law.
You can redistribute it and/or modify it under the terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the license.txt file for more details.
```
**Licenses to included third party libs, _including modifications_, still apply!**

--
**Third party libraries used**

[SFML](http://www.sfml-dev.org) (2.4.2)

[Box2D](https://github.com/erincatto/Box2D/) (71a6079)

[LTBL2](https://github.com/Cmdu76/LTBL2/tree/d2b51608efa2aa7e3e2d632746b018d6e2add3be) (d2b5160) which is further modified

All of the above are licensed under the [zLib-License](https://en.wikipedia.org/wiki/Zlib_License)

[Dear ImGui](https://github.com/ocornut/imgui) (1.51)

[ImGui SFML](https://github.com/eliasdaler/imgui-sfml) (0e92b6d)

[RTTR (RunTimeTypeReflection)](http://www.rttr.org) (c37d0e3)

[RapidJSON](https://github.com/miloyip/rapidjson) (1.1.0)

All four of which are licensed under the [MIT-License](https://opensource.org/licenses/MIT)