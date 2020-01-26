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
    }

    class Input {
        public isKeyPressed(keyName: string):boolean;
        public wasKeyPressed(keyName: string):boolean;
    }

    class ScriptObjectNative extends FSEObject {
        public extend(object: any): void;
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

    class ScriptObject extends ScriptObjectSuper {        
        classname: string;
        native: fse.ScriptObjectNative;
        super: fse.ScriptObjectSuper;
        constructor();
        get position(): fse.Vector2;
        set position(position: fse.Vector2);
        public draw(renderTarget: sf.RenderTarget): void;
        public spawned(): void;
    }


    interface Vector2 {
        x: number;
        y: number;
    }

}

declare var rootScene: fse.Scene;
declare var Input: fse.Input;