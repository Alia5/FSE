export abstract class ScriptComponent {
    public classname: string;
    public native: fse.ScriptComponentNative;
    public constructor() {
        this.native = new fse.ScriptComponentNative();
        this.classname = this.constructor.name;
        this.native.extend(this);
    }

}