# FSE #

FSE is a (work in progress) cross platform 2D-Toolkit built on top of SFML and Box2D which could be used for Games and alike.

FSE is mainly created as a side project out of personal interest.

I only have limited knowledge about game- / engine-programming and thus, this is probably not the most well though out code there is. 
I wouldn't recommend using it. If you'd really like to however, it's licensed under MIT, so feel free.

FSE supports networking which in testing performed pretty well under pretty bad network conditions (~200ms Ping with some packet loss) with a few 100 objects

I'm quite happy with with the results so far, considering this essentially being a little "Sunday project"

It also sports a pretty neat lighting system incl. normal and specular-maps
![Lighting1](https://github.com/Alia5/FSE/blob/master/lighting1.gif?raw=true "Lighting1") ![Lighting2](https://github.com/Alia5/FSE/blob/master/lighting2.gif?raw=true "Lighting2")

FSE can also trace any sprite (with alpha channel) and output triangulated convex polys for use with the physics engine.

*Work in Progress*

## License

```
Copyright © 2016 Peter Repukat - FlatspotSoftware

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
**Licenses to included third party libs, _including modifications_, still apply!**

--
**Third party libraries used**

[SFML](http://www.sfml-dev.org) (2.4.2)

[Box2D](https://github.com/erincatto/Box2D/) (71a6079)

[LTBL2](https://github.com/Cmdu76/LTBL2/tree/d2b51608efa2aa7e3e2d632746b018d6e2add3be) (d2b5160) which is further modified

All of the above are licensed under the [zLib-License](https://en.wikipedia.org/wiki/Zlib_License)

[Dear ImGui](https://github.com/ocornut/imgui) (1.51)

[ImGui SFML](https://github.com/eliasdaler/imgui-sfml) (141cd93)

[RTTR (RunTimeTypeReflection)](http://www.rttr.org) (c37d0e3)

[RapidJSON](https://github.com/miloyip/rapidjson) (1.1.0)

All four of which are licensed under the [MIT-License](https://opensource.org/licenses/MIT)

poly2tri

Licensed under the BSD 3-Clause "Revised" License