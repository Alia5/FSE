// interface dictionary {
//     [key: string]: any|undefined
// }
// interface Process {
//     argc: number;
//     argv: string[];
//     env: dictionary;
//     [key: string]: any|undefined
// }

// declare function require(path: string): any;

// declare namespace console {
//     function log(...args: any): void;
// }
// declare const __filename: string;
// declare const __dirname: string;
// declare let process: Process;

// declare var global: object;

/// <reference path="sf.d.ts" />

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
		public BeginContact(otherObject: FSEObject, contact: any): void;
		public EndContact(otherObject: FSEObject, contact: any): void;
		public PreSolve(otherObject: FSEObject, contact: any, oldManifold: any): void;
		public PostSolve(otherObject: FSEObject, contact: any, impulse: any): void;
    }

    class Scene {
        public isPaused(): boolean;
        public setPaused(paused: boolean): void;
		public getPhysDrawDebug(): boolean;
		public setPhysDrawDebug(draw: boolean): void;
		public getLightWorld(): any;
        public getPhysWorld(): any;
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
    }

    class ScriptObjectSuper {        
        public update(deltaTime: number): void;
        public getPosition(): {x: number, y:number};
        public setPosition(position: {x: number, y:number}): void;
        public GetAABBs(): any;
        public BeginContact(args: any): any;
        public EndContact(args: any): any;
        public PreSolve(args: any): any;
        public PostSolve(args: any): any;
        public destroy(): boolean;
        public BeginContactComponents(args: any): void;
        public EndContactComponents(args: any): void;
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

}

declare var rootScene: fse.Scene;
declare var Input: fse.Input;