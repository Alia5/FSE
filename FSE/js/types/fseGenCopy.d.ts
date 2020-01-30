declare namespace fse {
    abstract class ScriptObject {
        classname: string;
        native: fse.ScriptObjectNative;
        super: fse.ScriptObjectSuper;
        constructor();
        get position(): fse.Vector2;
        set position(position: fse.Vector2);
    }
    
    abstract class ScriptComponent {
        classname: string;
        native: fse.ScriptComponentNative;
        constructor();
    }
}