export abstract class ScriptComponent implements IScriptComponent {
    public shutup: void;
    public classname: string;
    public native: fse.ScriptComponentNative;
    public constructor() {
        this.native = new fse.ScriptComponentNative();
        this.classname = this.constructor.name;
        this.native.extend(this);
    }

}

export interface IScriptComponent {
    shutup: void;
    update?: (deltaTimer: number) => void;
}