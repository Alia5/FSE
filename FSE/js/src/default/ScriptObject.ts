export abstract class ScriptObject {
    public classname: string;
    public native: fse.ScriptObjectNative;
    public super: fse.ScriptObjectSuper;
    public constructor() {
        this.native = new fse.ScriptObjectNative();
        this.classname = this.constructor.name;
        this.native.extend(this);
    }

    get position(): fse.Vector2 {
        return this.super.getPosition();
    }
    
    set position(position: fse.Vector2) {
        this.super.setPosition(position);
    }
}