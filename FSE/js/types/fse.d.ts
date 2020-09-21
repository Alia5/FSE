interface dictionary {
    [key: string]: any|undefined
}
interface Process {
    argc: number;
    argv: string[];
    env: dictionary;
    [key: string]: any|undefined
}

declare function require(path: string): any;

declare namespace console {
    function log(...args: any): void;
}
declare const __filename: string;
declare const __dirname: string;
declare let process: Process;

declare var global: object;

declare function setTimeout(callback: () => void, millis: number): void;

/// <reference path="sf.d.ts" />
/// <reference path="lf.d.ts" />

 declare namespace fse {

    abstract class FSEObject {
        public getID(): number;
        public getPosition(): {x: number, y:number};
        public setPosition(position: {x: number, y:number}): void;
        public GetAABBs(): any;
        public destroy(): boolean;
        public attachComponent(component: Component): void;
        public getScene(): fse.Scene;
    }

    abstract class Component {
        public update(deltaTime: number): void;
	    public onAttach(): void;
		public onDetach(): void;
		public getAttachedObject(): FSEObject;
		public attachToObject(object: FSEObject): void;
		public isAttached(): boolean;
		public detach(): void;
		public BeginContact(otherObject: FSEObject, contact: lf.Contact): void;
		public EndContact(otherObject: FSEObject, contact: lf.Contact): void;
		public PreSolve(otherObject: FSEObject, contact: lf.Contact, oldManifold: any): void;
		public PostSolve(otherObject: FSEObject, contact: lf.Contact, impulse: any): void;
    }

    class Scene {
        public isPaused(): boolean;
        public setPaused(paused: boolean): void;
		public getPhysDrawDebug(): boolean;
		public setPhysDrawDebug(draw: boolean): void;
        public physDrawDebug: boolean;
		public getLightWorld(): any;
        public getPhysWorld(): lf.World;
        public getPixelsPerMeter(): number;
        public getMetersPerPixel(): number;
        public spawnObject(object: FSEObject): FSEObject;
        public getRenderTarget(): sf.RenderTarget;
    }

    class Input {
        public isKeyPressed(keyName: string):boolean;
        public wasKeyPressed(keyName: string):boolean;
    }

    class ScriptObjectNative extends FSEObject {
        public extend(object: object ): void;
        public update(deltaTime: number): void;
        public BeginContact(args: any): any;
        public EndContact(args: any): any;
        public PreSolve(args: any): any;
        public PostSolve(args: any): any;
        public BeginContactComponents(args: any): void;
        public EndContactComponents(args: any): void;
        public PreSolveComponents(args: any): void;
        public PostSolveComponents(args: any): void;
        public jsObject(): ScriptObject;
    }

    class ScriptObjectSuper {        
        public update(deltaTime: number): void;
        public getPosition(): {x: number, y:number};
        public setPosition(position: {x: number, y:number}): void;
        public GetAABBs(): any;
        public BeginContact(otherObject: FSEObject, contact: lf.Contact): void;
        public EndContact(otherObject: FSEObject, contact: lf.Contact): void;
        public PreSolve(args: any): any;
        public PostSolve(args: any): any;
        public destroy(): boolean;
        public BeginContactComponents(otherObject: FSEObject, contact: lf.Contact): void;
        public EndContactComponents(otherObject: FSEObject, contact: lf.Contact): void;
        public PreSolveComponents(args: any): void;
        public PostSolveComponents(args: any): void;
    }

    class ScriptComponentNative extends Component {        
        public extend(object: object): void;
    }

    interface Vector2 {
        x: number;
        y: number;
    }

    namespace fmath {
        function lerp(a: number,b: number,c: number): number;
        function lerpVector(a: Vector2,b: Vector2,c: number): Vector2;
		function sqrtVec(vec: Vector2): number
        function angle(vec: Vector2): number;
        function angleDegree(vec: Vector2): number;
    }

    class Application {
        public getWindow(): sf.Window;
        public getInput(): Input;
        public getNetworkHandler(): any; // TODO
        public getAssetLoader(): any; // TODO
        public getRootScene(): Scene;
        public setServerClientType(type: number): void;
        public isServer(): boolean;
    }

    namespace App {
        function get(): Application;
    }


}

declare var rootScene: fse.Scene;
declare var Input: fse.Input;